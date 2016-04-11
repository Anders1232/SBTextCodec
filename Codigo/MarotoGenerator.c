#include<stdio.h>
#include<stdlib.h>

int main (void){
	FILE *arq= fopen("Maroto", "wb");
	fprintf(arq, "Man is distinguished, not only by his reason, but by this singular passion from\nother animals, which is a lust of the mind, that by a perseverance of delight\nin the continued and indefatigable generation of knowledge, exceeds the short\nvehemence of any carnal pleasure.");
	fclose(arq);
	return 0;
}
