#include<stdint.h>
#include<stdio.h>

void VerificarEndian() {
    int a = 0x12345678;
    unsigned char *c = (unsigned char*)(&a);
    if (*c == 0x78) {
       printf("little-endian\n");
    } else {
       printf("big-endian\n");
    }
}

int main(void)
{
	VerificarEndian();
	uint32_t numero= 0x446F7441;
	char* ptr= &numero;
	printf("%d\n", '\n');
	printf("%c%c%c%c\n", ptr[0], ptr[1], ptr[2], ptr[3]);
	return 0;
}


