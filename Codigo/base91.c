#include "base91.h"

void base91EncoderDecoder(FILE *fileInput, FILE *fileOutput, int encodeSelected) {
	struct Base91 b91Struct;
	initBase91(&b91Struct);
	int multiplier;
	if (encodeSelected)
		multiplier = 3;
	else
		multiplier = 1;
	char *inputBuffer, *outputBuffer;
	size_t inputSize, inputBufferSize = 65536;
	inputBuffer = (char*) calloc(inputBufferSize * multiplier, sizeof(char));
	outputBuffer = (char*) calloc(inputBufferSize * multiplier, sizeof(char));
	while ((inputSize = fread(inputBuffer, 1, inputBufferSize, fileInput)) > 0) {
		inputSize = doWork(inputBuffer, outputBuffer, inputSize, &b91Struct, encodeSelected);
		fwrite(outputBuffer, 1, inputSize, fileOutput);
	}
	inputSize = finishWork(outputBuffer, &b91Struct, encodeSelected);
	fwrite(outputBuffer, 1, inputSize, fileOutput);
	free(inputBuffer);
	free(outputBuffer);
}

void initBase91(struct Base91 *b91Struct){
	b91Struct -> bitNumber = 0;
	b91Struct -> value = -1;
	b91Struct -> queue = 0;
}

size_t doWork(const unsigned char *inputBuffer, unsigned char *outputBuffer, size_t inputSize, struct Base91 *b91Struct, int encodeSelected) {
	unsigned char decoded;
	size_t outputSize = 0;
	unsigned int position, bitNumber;
	if (encodeSelected) {
		while (inputSize != 0) {
			bitNumber = b91Struct -> bitNumber;
			b91Struct -> queue |= *inputBuffer++ << bitNumber;
			b91Struct -> bitNumber += 8;
			if (b91Struct -> bitNumber > 13) {
				position = b91Struct -> queue & 8191;
				if (position > 88) {
					b91Struct -> queue >>= 13;
					b91Struct -> bitNumber -= 13;
				} else {
					position = b91Struct -> queue & 16383;
					b91Struct -> queue >>= 14;
					b91Struct -> bitNumber -= 14;
				}
				outputBuffer[outputSize] = encodingTable[position % 91];
				outputSize++;
				outputBuffer[outputSize] = encodingTable[position / 91];
				outputSize++;
			}
			inputSize--;
		}
	} else {
		for(int i = 0; i < inputSize; i++) {
			decoded = decodingTable[inputBuffer[i]];
			if (decoded == 91)
				continue;
			if (b91Struct -> value == -1)
				b91Struct -> value = decoded;
			else {
				b91Struct -> value += decoded * 91;
				b91Struct -> queue |= b91Struct -> value << b91Struct -> bitNumber;
				b91Struct -> bitNumber += (b91Struct -> value & 8191) > 88 ? 13 : 14;
				while(b91Struct -> bitNumber > 7) {
					outputBuffer[outputSize] = b91Struct -> queue;
					outputSize++;
					b91Struct -> queue >>= 8;
					b91Struct -> bitNumber -= 8;
				}
				b91Struct -> value = -1;
			}
		}
	}
	return outputSize;
}

size_t finishWork(unsigned char *outputBuffer, struct Base91 *b91Struct, int encodeSelected) {
	size_t outputSize = 0;
	if (encodeSelected) {
		if (b91Struct -> bitNumber) {
			outputBuffer[outputSize] = encodingTable[b91Struct -> queue % 91];
			outputSize++;
			if (b91Struct -> bitNumber >= 8 || b91Struct -> queue >= 91) {
				outputBuffer[outputSize] = encodingTable[b91Struct -> queue / 91];
				outputSize++;
			}
		}
	} else {
		if (b91Struct -> value != -1)
			outputBuffer[outputSize++] = b91Struct -> queue | b91Struct -> value << b91Struct -> bitNumber;	
	}
	initBase91(b91Struct);
	return outputSize;
}