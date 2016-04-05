#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

int main(int argc, char **argv){
	uint64_t totalBytesLidos=0;
	uint64_t bytesIguais=0;
	char c1, c2;
	FILE *arq1, *arq2;
	if(argc ==3){
		arq1= fopen(argv[1], "rb");
		arq2= fopen(argv[2], "rb");
		if(arq1 ==NULL || arq2 ==NULL){
			printf("Deu pau!\n");
			return -1;
		}
		while( !/*(*/feof(arq1) /*|| feof(arq2) )*/ ){
			fscanf(arq1, "%c", &c1);
			fscanf(arq2, "%c", &c2);
			totalBytesLidos++;
			if(c1 == c2){
				bytesIguais++;
			}
		}
		printf("%lu iguais de %lu\n", bytesIguais, totalBytesLidos);
		printf("%lf%%\n", ((double) bytesIguais/(double)totalBytesLidos)*100 );
	}
	return 0;
}
