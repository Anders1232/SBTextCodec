//#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include"Base64.h"
//Sugestao de nome: SBTextCodec
#define TESTE1

int main(void)
{
#ifdef TESTE1
	char formato[]= "%X\t%X\t%X\t%X\n";
	ElementoBase64 e;
	e.byte[0] = 'M';
	e.byte[1] = '\n';
	e.byte[2] = 'n';
	e.byte[3] = '\0';
	ImprimirTextoCodificado64(stdout, formato, e, 3);
	int inte;
	uint32_t temp= Base64_CodificarTexto(e, 3);
	printf("%s\n",  (char*)(Base64_DecodificarTexto(temp, &inte).byte));
	
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
