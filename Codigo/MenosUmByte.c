#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv){
	char c1= 'A';
	FILE *arq1, *arq2;
	arq1= fopen(argv[1], "rb");
	arq2= fopen("_carai", "wb");
	if(arq1 ==NULL || arq2 == NULL){
		printf("Deu pau!\n");
		return -1;
	}
	do{
		fscanf(arq1, "%c", &c1);
		if(c1 != EOF){
			fprintf(arq2, "%c", c1);;
		}else{
			break;
		}
	}while(1);

	fclose(arq1);
	fclose(arq2);

	return 0;
}
