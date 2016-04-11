#include "base91.h"

void base91EncoderDecoder(FILE *fileInput, FILE *fileOutput, int encodeSelected) {
	size_t inputSize, inputBufferSize = 65536;
	char *inputBuffer = NULL, *outputBuffer = NULL;
	struct Base91 b91Struct;
	initBase91(&b91Struct);
	int multiplier;
	if (encodeSelected)
		multiplier = 3;
	else
		multiplier = 1;
	inputBuffer = (char*) calloc(inputBufferSize * multiplier, sizeof(char));
	outputBuffer = (char*) calloc(inputBufferSize * 3, sizeof(char));
	while ((inputSize = fread(inputBuffer, 1, inputBufferSize, fileInput)) > 0) {
		inputSize = doWork(&b91Struct, inputBuffer, inputSize, outputBuffer, encodeSelected);
		fwrite(outputBuffer, 1, inputSize, fileOutput);
	}
	inputSize = finishWork(&b91Struct, outputBuffer, encodeSelected);
	fwrite(outputBuffer, 1, inputSize, fileOutput);
	free(inputBuffer);
	free(outputBuffer);
}

void initBase91(struct Base91 *b91Struct){
	b91Struct->list = 0;
	b91Struct->bitNumber = 0;
	b91Struct->value = -1;
}

size_t doWork(struct Base91 *b91Struct, const unsigned char *inputBuffer, size_t inputSize, unsigned char *outputBuffer, int encodeSelected) {
	if (encodeSelected)
		return doEncoding(b91Struct, inputBuffer, inputSize, outputBuffer);
	return doDecoding(b91Struct, inputBuffer, inputSize, outputBuffer);
}

size_t doEncoding(struct Base91 *b91Struct, const unsigned char *inputBuffer, size_t inputSize, unsigned char *outputBuffer){
	size_t outputSize = 0;
	unsigned int holder = 0;
	for(int i = 0; i < inputSize; i++){
		b91Struct->list |= inputBuffer[i] << b91Struct->bitNumber;
		b91Struct->bitNumber += 8;
		if (b91Struct->bitNumber > 13) {	
			holder = b91Struct->list & 8191;
			if (holder > 88) {
				b91Struct->list >>= 13;
				b91Struct->bitNumber -= 13;
			} else {
				holder = b91Struct->list & 16383;
				b91Struct->list >>= 14;
				b91Struct->bitNumber -= 14;
			}
			outputBuffer[outputSize] = encodingTable[holder % 91];
			outputSize++;
			outputBuffer[outputSize] = encodingTable[holder / 91];
			outputSize++;
		}
	}
	return outputSize;
}

size_t doDecoding(struct Base91 *b91Struct, const unsigned char *inputBuffer, size_t inputSize, unsigned char *outputBuffer){
	size_t outputSize = 0;
	unsigned char symbol;
	for(int i = 0; i < inputSize; i++){
		symbol = decodingTable[inputBuffer[i]];
		if (symbol == 91)
			continue;
		if (b91Struct->value == -1)
			b91Struct->value = symbol;
		else {
			b91Struct->value += symbol * 91;
			b91Struct->list |= b91Struct->value << b91Struct->bitNumber;
			b91Struct->bitNumber += (b91Struct->value & 8191) > 88 ? 13 : 14;
			while(b91Struct->bitNumber > 7) {
				outputBuffer[outputSize] = b91Struct->list;
				outputSize++;
				b91Struct->list >>= 8;
				b91Struct->bitNumber -= 8;
			}
			b91Struct->value = -1;
		}
	}
	return outputSize;
}

size_t finishWork(struct Base91 *b91Struct, unsigned char *outputBuffer, int encodeSelected) {
	if (encodeSelected)
		return finishEncoding(b91Struct, outputBuffer);
	return finishDecoding(b91Struct, outputBuffer);
}

size_t finishEncoding(struct Base91 *b91Struct, unsigned char *outputBuffer){
	size_t outputSize = 0;
	if (b91Struct->bitNumber) {
		outputBuffer[outputSize] = encodingTable[b91Struct->list % 91];
		outputSize++;
		if (b91Struct->bitNumber >= 8 || b91Struct->list >= 91){
			outputBuffer[outputSize] = encodingTable[b91Struct->list / 91];
			outputSize++;
		}
	}
	initBase91(b91Struct);
	return outputSize;
}

size_t finishDecoding(struct Base91 *b91Struct, unsigned char *outputBuffer){
	size_t outputSize = 0;
	if (b91Struct->value != -1)
		outputBuffer[outputSize++] = b91Struct->list | b91Struct->value << b91Struct->bitNumber;
	initBase91(b91Struct);
	return outputSize;
}