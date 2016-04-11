#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Base64.h"
#include "Base85.h"
#include "Base62.h"
#include "base91.h"

#define TAM_MAX_NOME_ARQUIVO 150

int main(int argc, char** argv){

char nomeArq[TAM_MAX_NOME_ARQUIVO+1];
char codOuDec[4];
int base;
	// Pede por informações que não foram passadas por linha de comando.
	switch(argc){
		case (1):{
			printf("Insira o nome do arquivo de entrada:\n");
			scanf("%s", nomeArq);
		}
		case (2):{
			printf("Queres codificar ou decodificar o arquivo? Digite cod para codificar e dec para decodificar:\n");
			scanf("%s", codOuDec);
			if(argc ==2){
				strcpy(nomeArq, argv[1]);
			}
		}
		case (3):{
			printf("Qual base deseja utilizar para codificar? (62, 64, 85, 91)\n");
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

	//Abre arquivos de entrada e saída e formata nome do arquivo de saída.
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

	int auxAnaliseLeitura = 1;
	int umPraCod0PraDecod= !strcmp("cod", argv[2]);

	// Executa dependendo da base desejada.
	if(base == 64){

		ElementoBase64 elemento;

		if(umPraCod0PraDecod){
			while(auxAnaliseLeitura){
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

				ImprimirTextoCodificado64(arqSaida, "%c", elemento, auxAnaliseLeitura);
			}
		}else{
			{
				uint32_t numPraDecodificar;
				ElementoBase64 elementoDecodificado;
				int bytesValidos;//para o caso dos = no final do arquivo não se deve imprimir alguns bytes
				while(auxAnaliseLeitura){
					auxAnaliseLeitura = fread(&numPraDecodificar, 4, 1, arqEntrada);
					elementoDecodificado = Base64_DecodificarTexto(numPraDecodificar, &bytesValidos);

					fprintf(arqSaida, "%c",(elementoDecodificado.byte[0]<<2)|(elementoDecodificado.byte[1]>>4) );

					if(bytesValidos >=2){

						fprintf(arqSaida, "%c",(elementoDecodificado.byte[1]<<4) | (elementoDecodificado.byte[2]>>2));

					}
					if(bytesValidos ==3){

						fprintf(arqSaida, "%c", (elementoDecodificado.byte[2]<<6) | (elementoDecodificado.byte[3]));

						
					}
				}
			}
		}
	}
	// Se passou 85 como argumento.
	if(base == 85){

		ElementoBase85 elemento;
		// Avalia se passou cod ou dec como argumento.
		if(umPraCod0PraDecod){
			// loop para leitura do arquivo até o final.
			while(!feof(arqEntrada)){
				auxAnaliseLeitura= fread(elemento.byte, 1, 4, arqEntrada);
				//Padding
				if(auxAnaliseLeitura != 4){
					if(auxAnaliseLeitura == 3 ){
						elemento.byte[3]= 0;
					}
					if(auxAnaliseLeitura == 2){
						elemento.byte[2]= 0;
						elemento.byte[3]= 0;
					}
					if(auxAnaliseLeitura == 1){
						elemento.byte[1]= 0;
						elemento.byte[2]= 0;
						elemento.byte[3]= 0;
					}
				}
				//Função para codificar e escrever no arquivo de saída.
				ImprimirTextoCodificado85(arqSaida, "%c", elemento, auxAnaliseLeitura);
			}
		}
		else{
			//Loop para leitura do arquivo até o final.
			while(!feof(arqEntrada)){
				auxAnaliseLeitura= fread(elemento.byte, 1, 5, arqEntrada);

				//Realizando o padding de acordo com a quantidade de bytes lidos.
				if(auxAnaliseLeitura != 5){
					if(auxAnaliseLeitura == 4 ){
						elemento.byte[4]= 117;
					}
					if(auxAnaliseLeitura == 3 ){
						elemento.byte[3]= 117;
						elemento.byte[4]= 117;
					}
					if(auxAnaliseLeitura == 2){
						elemento.byte[2]= 117;
						elemento.byte[3]= 117;
						elemento.byte[4]= 117;
					}
					if(auxAnaliseLeitura == 1){
						elemento.byte[1]= 117;
						elemento.byte[2]= 117;
						elemento.byte[3]= 117;
						elemento.byte[4]= 117;
					}
				}
				//Função para decodificar e escrever no arquivo de saída.	
				ImprimirTextoDecodificado85(arqSaida, "%c", elemento, auxAnaliseLeitura);
			}
		}

	}

	if(base == 62){;
        char cA, cB;
        int iA;

		if(umPraCod0PraDecod){
			int sobra = 0;

			while(!feof(arqEntrada)){
                iA = fgetc(arqEntrada);
                cA = iA;
                if(iA != EOF){
                    sobra = codificador_62(cA, cB, sobra,arqSaida);
                    cB = cA;
                }
                else{
                    if(sobra>0){
                        sobra = codificador_62(NULL, cB, sobra, arqSaida);
                    }
                }
			}
		}
		else{
            struct buffer buff62;
            buff62 = zeraBuffer(buff62);

            while(!feof(arqEntrada)){
                iA = fgetc(arqEntrada);
                cA = iA;

                if(iA != EOF){
                    buff62 = decodificador_62(cA, buff62, arqSaida);
//printf("%d", buff62.contador);
                }
                else{
                    if(buff62.contador>0){
                        fim(buff62,arqSaida);
                    }
                }
            }
		}
	}
	if(base == 91) {
		base91EncoderDecoder(arqEntrada, arqSaida, umPraCod0PraDecod);
	}
	//Fecha arquivos de entrada e saida.
	fclose(arqEntrada);
	fclose(arqSaida);

	return 0;
}
