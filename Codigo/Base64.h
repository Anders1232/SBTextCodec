#include <stdint.h>
#include <stdio.h>

#ifndef BASE64_H
#define BASE64_H

#define CODIGO_IGUAL 64

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
	if(codigo == CODIGO_IGUAL)
	{
		return '=';
	}
	else
	{
		return '?';
	}
}

char Base64_ObterCaractereDecodificado(uint8_t codigo)
{
	if(codigo  >= '0' && codigo <= '9')
	{
		return codigo + 4;
	}
	if(codigo <= 'Z')
	{

		return codigo - 'A';
	}
	if(codigo <= 'z'  && codigo >= 'a')
	{
		return codigo - 71;
	}
	if( codigo == '+')
	{
		return 62;
	}
	if(codigo == '/')
	{
		return 63;
	}
	if(codigo == '=')
	{
		return CODIGO_IGUAL;
	}
	else
	{
		return '?';
	}
}


static inline uint32_t Base64_CodificarTexto(ElementoBase64 elemento, uint8_t quantidadeBytesValidos)
{
	uint32_t retorno;
	char* aux= (char *)&retorno;
	aux[0]= (elemento.byte[0])>>2;
	aux[1]= ( (elemento.byte[0])<<4 | elemento.byte[1]>> 4 ) & 0x3F;
	aux[2]= ( (elemento.byte[1])<<2 | elemento.byte[2]>> 6 ) & 0x3F;
	aux[3]= (elemento.byte[2]) & 0x3F;
	if(quantidadeBytesValidos != 3){
		if(quantidadeBytesValidos == 1){
			aux[2]= CODIGO_IGUAL;
			aux[3]= CODIGO_IGUAL;
		}
		if(quantidadeBytesValidos == 2){
			aux[3]= CODIGO_IGUAL;
		}
	}
	return retorno;
}

void ImprimirTextoCodificado64(FILE *onde, const char* como, ElementoBase64 oQue, int quantBytesValidos)
{
	int32_t valorCodificado= Base64_CodificarTexto(oQue, quantBytesValidos);
	char* aux= (char *)&valorCodificado;
#ifdef USAR_FPRINTF
	fprintf(
		onde,
		como,
		Base64_ObterCaractereCodificado(aux[0]),
		Base64_ObterCaractereCodificado(aux[1]),
		Base64_ObterCaractereCodificado(aux[2]),
		Base64_ObterCaractereCodificado(aux[3])
	);
#else 
		char codificado[5];
		sprintf(
			codificado,
			"%c%c%c%c",
	 		Base64_ObterCaractereCodificado(aux[0]),
			Base64_ObterCaractereCodificado(aux[1]),
			Base64_ObterCaractereCodificado(aux[2]),
			Base64_ObterCaractereCodificado(aux[3])
		);
		fwrite(codificado, 1, 4, onde);
#endif
}

static inline ElementoBase64 Base64_DecodificarTexto(uint32_t texto, int *bytesValidos/*para retorno por ponteiro*/)
{
	*bytesValidos=3;
	char* aux= (char*)&texto;
	if(aux[3] == '='){
		aux[3]=0;
		(*bytesValidos)--;		
	}
	if(aux[2] == '='){
		aux[2]=0;
		(*bytesValidos)--;		
	}
	ElementoBase64 retorno;

	int32_t concat;
	retorno.byte[0]= Base64_ObterCaractereDecodificado( aux[0] );
	retorno.byte[1]= Base64_ObterCaractereDecodificado( aux[1] );
	retorno.byte[2]= Base64_ObterCaractereDecodificado( aux[2] );
	retorno.byte[3]= Base64_ObterCaractereDecodificado( aux[3] );

	return retorno;
}


#endif
