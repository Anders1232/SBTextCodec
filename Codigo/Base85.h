#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Struct para auxiliar na leitura do arquivo e manipulação dos bits.
struct elementoBase85{
	uint8_t byte[5];
};
typedef struct elementoBase85 ElementoBase85;

//Concatena os bits lidos em uma var de 32bits para codificação.
uint32_t ConcatenaBits(ElementoBase85 bits){
	uint32_t result;
	result = bits.byte[0];
	result <<= 8;
	result = result | bits.byte[1];
	result <<= 8;
	result = result | bits.byte[2];
	result <<= 8;
	result = result | bits.byte[3];
	return result;
}

// Codifica os 32 bits lidos do arquivo e salva cada caracter codificado em um vetor.
static inline void Base85_CodificarTexto(uint32_t aux,uint8_t* coded){
	int i;
	div_t output;
	for(i = 0; i<5; i++){
		output = div(aux,85);
		aux = output.quot;
		coded[i] = (output.rem + 33);
	}
}

// Codifica bits e aplica lógica para escrita no arquivo de saída.
void ImprimirTextoCodificado85(FILE *onde, const char* como, ElementoBase85 oQue, int quantBytesValidos){

	int i;
	uint8_t coded[5];
	uint32_t aux = ConcatenaBits(oQue);
	Base85_CodificarTexto(aux,coded);

	// Se leu todos os 4 bytes corretamente salva tudo no arquivo
	// Se leu menos de 4 bytes deve realizar o padding.
	// Sendo que o padding é omitido do arquivo de saída.
	// Percorre ao contrário pois o tipo int salva na mem como little.
	if(quantBytesValidos == 4){
		for(i = 4; i >= 0; i--){
			fprintf(onde, como,coded[i]);
		}
	}

	if(quantBytesValidos == 3){
		for(i = 4; i >= 1; i--){
			fprintf(onde, como,coded[i]);
		}
	}

	if(quantBytesValidos == 2){
		for(i = 4; i >= 2; i--){
			fprintf(onde, como,coded[i]);
		}
	}

	if(quantBytesValidos == 1){
		for(i = 4; i >= 3; i--){
			fprintf(onde, como,coded[i]);
		}
	}

}

static inline uint32_t Base85_DecodificarTexto(ElementoBase85 bits){
	uint32_t result;
	result = (bits.byte[0] * pow(85,4)) + (bits.byte[1] * pow(85,3)) + (bits.byte[2] * pow(85,2)) + (bits.byte[3] * pow(85,1)) + bits.byte[4];
	return result;
}

static inline void Base85_FormatarTexto(uint32_t result,uint8_t* coded){
	coded[0] = result >> 24;
	coded[1] = (result >> 16) & 0xFF;
	coded[2] = (result >> 8) & 0xFF;
	coded[3] = result & 0xFF;
}

void ImprimirTextoDecodificado85(FILE *onde, const char* como, ElementoBase85 oQue, int quantBytesValidos){

	int i;
	for(i = 0; i < 5; i++)
		oQue.byte[i] -= 33;

	uint32_t result;
	result = Base85_DecodificarTexto(oQue);

	uint8_t coded[4];
	Base85_FormatarTexto(result,coded);

	if(quantBytesValidos == 5){
		for(i = 0; i <= 3; i++){
			fprintf(onde, como,coded[i]);
		}
	}

	if(quantBytesValidos == 4){
		for(i = 0; i <= 2; i++){
			fprintf(onde, como,coded[i]);
		}
	}

	if(quantBytesValidos == 3){
		for(i = 0; i <= 1; i++){
			fprintf(onde, como,coded[i]);
		}
	}

	if(quantBytesValidos == 2){
		for(i = 0; i <= 0; i++){
			fprintf(onde, como,coded[i]);
		}
	}

	
}
