#include<stdint.h>
#include<stdio.h>

#ifndef BASE64_H
#define BASE64_H

struct elementoBase64
{
	uint8_t byte[4];
};
typedef struct elementoBase64 ElementoBase64;

char Base64_ObterCaractereCodificado(uint8_t codigo)
{
	if(codigo < 26)
	{
		return codigo +'A';
	}
	if(codigo < 52)
	{
		return codigo + 71;
	}
	if(codigo < 62)
	{
		return codigo - 4;
	}
	if(codigo == 62)
	{
		return '+';
	}
	if(codigo == 63)
	{
		return '/';
	}
	else
	{
		return '?';
	}
}

static inline uint32_t Base64_CodificarTexto(ElementoBase64 elemento)
{
	uint32_t retorno;
	char* aux= (char *)&retorno;
	aux[0]= (elemento.byte[0])>>2;
	aux[1]= ( (elemento.byte[0])<<4 | elemento.byte[1]>> 4 ) & 0x3F;
	aux[2]= ( (elemento.byte[1])<<2 | elemento.byte[2]>> 6 ) & 0x3F;
	aux[3]= (elemento.byte[2]) & 0x3F;
	return retorno;
}

void ImprimirTextoCodificado64(FILE *onde, const char* como, ElementoBase64 oQue)
{
	int32_t valorCodificado= Base64_CodificarTexto(oQue);
	char* aux= (char *)&valorCodificado;
	fprintf(
		onde,
		como,
		Base64_ObterCaractereCodificado(aux[0]),
		Base64_ObterCaractereCodificado(aux[1]),
		Base64_ObterCaractereCodificado(aux[2]),
		Base64_ObterCaractereCodificado(aux[3])
	);
}

static inline ElementoBase64 Base64_DecodificarTexto(uint32_t texto)
{
	char* aux= (char*)&texto;
	ElementoBase64 retorno;
//	strcpy(retorno.byte, "\0\0\0\0");
	retorno.byte[0]= (aux[0]<<2)|(aux[1]>>4);
	retorno.byte[1]= aux[1]<<4 | aux[2]>>2;
	retorno.byte[2]= aux[2]<<6 | aux[3];
	retorno.byte[3]= '\0';
	return retorno;
}


#endif
