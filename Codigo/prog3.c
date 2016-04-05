#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include"Base64.h"

#define TAM_MAX_NOME_ARQUIVO 150

int main(int argc, char** argv){
char nomeArq[TAM_MAX_NOME_ARQUIVO+1];
char codOuDec[4];
int base;
	switch(argc){
		case (1):{
			printf("Insira o nome do arquivo de entrada:\n");
			scanf("%s", nomeArq);
		}
		case (2):{
			printf("Queres codificar ou decodificar o arquivo? Digite cod para codificar e dec para decodificar\n");
			scanf("%s", codOuDec);
			if(argc ==2){
				strcpy(nomeArq, argv[1]);
			}
		}
		case (3):{
			printf("Qual base deseja utilizar para codificar?\n");
			scanf(" %d", &base);
			if(argc ==3){
				strcpy(nomeArq, argv[1]);
				strcpy(codOuDec, argv[2]);
			}
		}
		case(4):{
			if(argc == 4){
				strcpy(nomeArq, argv[1]);
				strcpy(codOuDec, argv[2]);
				base = atoi(argv[3]);
			}
		}
	}
	FILE *arqEntrada= fopen(nomeArq, "rb");
	char nomeSaida[55];
	if(!strcmp(codOuDec, "cod")){
		sprintf(nomeSaida, "%s.b%d", nomeArq, base);
	}else{
		snprintf(nomeSaida, strlen(nomeArq)-2, "_%s", nomeArq);
	}
	FILE *arqSaida= fopen(nomeSaida, "wb");

	//checando se deu certo a abertura

	if(arqEntrada == NULL){
		printf("[ERRO] Nao foi possivel abrir o arquivo de entrada.");
	}
	if(arqSaida == NULL){
		printf("[ERRO] Nao foi possivel abrir o arquivo de saida");
	}
	ElementoBase64 elemento;
	int auxAnaliseLeitura;
	int umPraCod0PraDecod= !strcmp("cod", argv[2]);
	if(umPraCod0PraDecod){
		while(!feof(arqEntrada)){
			auxAnaliseLeitura= fread(elemento.byte, 1, 3, arqEntrada);
			if(auxAnaliseLeitura != 3){
				if(auxAnaliseLeitura == 2 ){
					elemento.byte[2]= 0;
				}
				if(auxAnaliseLeitura == 1){
					elemento.byte[1]= 0;
					elemento.byte[2]= 0;
				}
				if(auxAnaliseLeitura == 0){
					break;
				}
			}
			ImprimirTextoCodificado64(arqSaida, "%c%c%c%c", elemento, auxAnaliseLeitura);
		}
	}else{
		{
			uint32_t numPraDecodificar;
			ElementoBase64 elementoDecodificado;
			int bytesValidos;//para o caso dos = no final do arquivo não se deve imprimir alguns bytes
			while(!feof(arqEntrada)){
				fread(&numPraDecodificar, 4, 1, arqEntrada);
				elementoDecodificado = Base64_DecodificarTexto(numPraDecodificar, &bytesValidos);
				fprintf(stdout, "valor no arq %c \n",(elementoDecodificado.byte[0]<<2)|(elementoDecodificado.byte[1]>>4));
				fprintf(arqSaida, "%c",(elementoDecodificado.byte[0]<<2)|(elementoDecodificado.byte[1]>>4) );
				if(bytesValidos >=2){
					fprintf(stdout, "valor no arq %c \n",(elementoDecodificado.byte[1]<<4) | (elementoDecodificado.byte[2]>>2));
					fprintf(arqSaida, "%c",(elementoDecodificado.byte[1]<<4) | (elementoDecodificado.byte[2]>>2));
				}
				if(bytesValidos ==3){
					fprintf(stdout, "valor no arq %c \n",(elementoDecodificado.byte[2]<<6) | (elementoDecodificado.byte[3]));
					fprintf(arqSaida, "%c", (elementoDecodificado.byte[2]<<6) | (elementoDecodificado.byte[3]));
					
				}
			}
		}
	}

	fclose(arqEntrada);
	fclose(arqSaida);
	return 0;
}
