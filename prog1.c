//#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include"Base64.h"
//Sugestao de nome: SBTextCodec
#define TESTE1

int main(void)
{
#ifdef TESTE1
	char formato[]= "%c\t%c\t%c\t%c\n";
	ElementoBase64 e;
	e.byte[0] = 'M';
	e.byte[1] = 'a';
	e.byte[2] = 'n';
	e.byte[3] = '\0';
	ImprimirTextoCodificado64(stdout, formato, e);
	
	uint32_t temp= Base64_CodificarTexto(e);
	printf("%s\n",  (char*)(Base64_DecodificarTexto(temp).byte) );
	
#elif TESTE2
	uint8_t cont=0;
	do
	{
		printf("%u\t%c\n", cont, Base64_ObterCaractereCodificado(cont));
		cont++;
	}
	while(cont < 64);
#endif
	return 0;
}
