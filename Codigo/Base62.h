#include <stdint.h>
#include <stdio.h>
#ifndef BASE62_H
#define BASE62_H


/////////////////////////////////////
///////////CODIFICADOR///////////////
/////////////////////////////////////

char alphanumeric_62(int resto){
    char tabela62[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    return tabela62[resto];
}

int codificador_62(char cA,char cB,int sobra, FILE *saida){ //Caracter novo, caracter antigo, sobra util do caracter antigo
    int resto, quo = 2; //Variável q avisa se o quociente deve ser somado;
    lldiv_t divisao;

//printf("A: %c\n", cA);
//printf("%d%d %d%d%d %d%d%d\n", (cA & 0x80 ? 1 : 0), (cA & 0x40 ? 1 : 0), (cA & 0x20 ? 1 : 0), (cA & 0x10 ? 1 : 0), (cA & 0x08 ? 1 : 0), (cA & 0x04 ? 1 : 0), (cA & 0x02 ? 1 : 0), (cA & 0x01 ? 1 : 0));

    //Pega os próximos 6 bits
    switch(sobra){
        case (0):{ //Não houve sobra do caracter anterior
            cB = ((cA)>>2) & 0x3F;
            sobra = 2;
            break;
        }
        case (1):{ //Caso sobre 1 bits no caracter antigo (cB) 00 BAA AAA
            cB = ((cB)<<5 | (((cA)>>3) & 0x1F))& 0x3F;
            sobra = 3;
            break;
        }
        case (2):{ //Caso sobre 2 bits no caracter antigo (cB) 00 BBA AAA
            cB = ((cB)<<4 | (((cA)>>4) & 0x0F))& 0x3F;
            sobra = 4;
            break;
        }
        case (3):{ //Caso sobre 3 bits no caracter antigo (cB) 00 BBB AAA
//printf("\n caso 3\n");
//printf("%d%d %d%d%d %d%d%d\n", (cA>>5 & 0x80 ? 1 : 0), (cA>>5 & 0x40 ? 1 : 0), (cA>>5 & 0x20 ? 1 : 0), (cA>>5 & 0x10 ? 1 : 0), (cA>>5 & 0x08 ? 1 : 0), (cA>>5 & 0x04 ? 1 : 0), (cA>>5 & 0x02 ? 1 : 0), (cA>>5 & 0x01 ? 1 : 0));
//printf("%d%d %d%d%d %d%d%d\n", (cB<<3 & 0x80 ? 1 : 0), (cB<<3 & 0x40 ? 1 : 0), (cB<<3 & 0x20 ? 1 : 0), (cB<<3 & 0x10 ? 1 : 0), (cB<<3 & 0x08 ? 1 : 0), (cB<<3 & 0x04 ? 1 : 0), (cB<<3 & 0x02 ? 1 : 0), (cB<<3 & 0x01 ? 1 : 0));
            cB = ((cB)<<3 | (((cA)>>5) & 0x07))& 0x3F;
//printf("%d%d %d%d%d %d%d%d\n\n", (cB & 0x80 ? 1 : 0), (cB & 0x40 ? 1 : 0), (cB & 0x20 ? 1 : 0), (cB & 0x10 ? 1 : 0), (cB & 0x08 ? 1 : 0), (cB & 0x04 ? 1 : 0), (cB & 0x02 ? 1 : 0), (cB & 0x01 ? 1 : 0));
            sobra = 5;
            break;
        }
        case (4):{ //Caso sobre 4 bits no caracter antigo (cB) 00 BBB BAA
            cB = ((cB)<<2 | (((cA)>>6) & 0x03))& 0x3F;
            sobra = 6;
            break;
        }
        case (5):{ //Caso sobre 5 bits no caracter antigo (cB) 00 BBB BBA
            cB = ((cB)<<1 | (((cA)>>7) & 0x01))& 0x3F;
            sobra = 7;
            break;
        }
    }

    //Caso na ultima codificação tenha sobrado um valor
    if(quo != 2){
        if(quo == 0){
            cB = (cB & 0xFE); //00 BBB BB0
            sobra++;
//printf("quo = 0, sobra = %d\n", sobra);
        }
        else if(quo == 1){
            cB = (cB & 0xFE); //00 BBB BB0
            cB = (cB | 0x01); //00 BBB BB1
            sobra++;
//printf("quo = 0, sobra = %d\n", sobra);
        }
        else{
            printf("[ERRO] O valor do quociente está em %d\n", quo);
            system("pause");
        }
        quo = 2;
    }

//printf("B: %c %d\n", cB, cB);
//printf("%d%d %d%d%d %d%d%d\n", (cB & 0x80 ? 1 : 0), (cB & 0x40 ? 1 : 0), (cB & 0x20 ? 1 : 0), (cB & 0x10 ? 1 : 0), (cB & 0x08 ? 1 : 0), (cB & 0x04 ? 1 : 0), (cB & 0x02 ? 1 : 0), (cB & 0x01 ? 1 : 0));

    //Codificando
    divisao = lldiv(cB, 62); //Divisão do character gerado pela base 62
    resto = divisao.rem;
//printf("resto: %d, Caracter: %c\n", resto, alphanumeric_62(resto));
    fprintf(saida, "%c",  alphanumeric_62(resto));

    //Verificando divisão "overflow"
    if(resto == 0 || resto == 1){
        quo = divisao.quot;
//printf("quo = %d\n", quo);
    }

    ////////////////////////////////////////
    //Sobra maior que 5? já pode codificar//
    ////////////////////////////////////////

    if(sobra > 5){
//printf("A: %c\n", cA);
//printf("%d%d %d%d%d %d%d%d\n", (cA & 0x80 ? 1 : 0), (cA & 0x40 ? 1 : 0), (cA & 0x20 ? 1 : 0), (cA & 0x10 ? 1 : 0), (cA & 0x08 ? 1 : 0), (cA & 0x04 ? 1 : 0), (cA & 0x02 ? 1 : 0), (cA & 0x01 ? 1 : 0));

        if(sobra > 8){
            printf("[ERRO] Sobra de bits igual %d\n", sobra);
        }
        switch(sobra){
            case (6):{
                cB = (cA) & 0x3F;
                sobra = 0;
                break;
            }
            case (7):{
                cB = ((cA)>>1) & 0x3F;
                sobra = 1;
                break;
            }
        }
        //Caso na ultima codificação tenha sobrado um valor
        if(quo != 2){
            if(quo == 0){
                cB = (cB & 0xFE); //00 BBB BB0
                sobra++;
//printf("quo = 0, sobra = %d\n", sobra);
            }
            else if(quo == 1){
                cB = (cB & 0xFE); //00 BBB BB0
                cB = (cB | 0x01); //00 BBB BB1
                sobra++;
//printf("quo = 1, sobra = %d\n", sobra);
            }
            else{
                printf("[ERRO] O valor do quociente está em %d\n", quo);
                system("pause");
            }
            quo = 2;
        }
//printf("B: %c %d\n", cB, cB);
//printf("%d%d %d%d%d %d%d%d\n", (cB & 0x80 ? 1 : 0), (cB & 0x40 ? 1 : 0), (cB & 0x20 ? 1 : 0), (cB & 0x10 ? 1 : 0), (cB & 0x08 ? 1 : 0), (cB & 0x04 ? 1 : 0), (cB & 0x02 ? 1 : 0), (cB & 0x01 ? 1 : 0));

        //Codificando
        divisao = lldiv(cB, 62); //Divisão do character gerado pela base 62
        resto = divisao.rem;
//printf("resto: %d, Caracter: %c\n", resto, alphanumeric_62(resto));
        fprintf(saida, "%c",  alphanumeric_62(resto));

        //Verificando divisão "overflow"
        if(resto == 0 || resto == 1){
            quo = divisao.quot;
        }
    }

//printf("%d\n", sobra);
//system("pause");
//printf("\n");

    return sobra;
}

/////////////////////////////////////
/////////DECODIFICADOR///////////////
/////////////////////////////////////
struct buffer {
    int contador;
    int vetor[26];
    int resto;
};

struct buffer zeraBuffer(struct buffer a){
    int i;
    a.contador = 0;
    a.resto = 2;
    for (i = 0; i < 26; i++){
        a.vetor[i] = 0;
    }
    return a;
}

int deTabela (char c){
    char tabela62[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int i;
    for(i=0; i<62; i++){
        if(tabela62[i] == c){
            return i;
        }
    }
    printf("[ERRO] Caracter lido fora da tabela de conversao base62. %c %d\n\n", c, c);
    exit(1);
}

struct buffer decodificador_62(char cA, struct buffer buff62, FILE *saida){
    int iA, k, i, vetorBits[6];
    iA = deTabela(cA); //Trás o valor decimal correspondente do caracter(entre 0 e 62)
//printf("\n\n");
//printf("caracter: %c \tnumero: %i\n", cA, iA);
    if(buff62.resto != 2){
        for(i=0; i<6; i++){ //Salva a sequencia binário em um vetor
            k = iA >> i;
            if(k & 1){
                vetorBits[5-i] = 1;
            }
            else{
                vetorBits[5-i] = 0;
            }
        }
//printf("%d - %d%d%d%d%d%d\n",iA, vetorBits[0], vetorBits[1], vetorBits[2], vetorBits[3], vetorBits[4], vetorBits[5]);

        //Salva o byte anterior
        for(i=0; i<5; i++){//Passa os 5 bits de valor 1 ou 0, de acordo com o quosciente
            buff62.vetor[buff62.contador] = vetorBits[5];
            buff62.contador++;
        }
        buff62.vetor[buff62.contador] = buff62.resto;
        buff62.contador++;
        buff62.resto = 2;
//printf("cont: %d\nBuffVetorAnterior: ", buff62.contador);
//for(i=0; i<(buff62.contador); i++){
//printf("%d ", buff62.vetor[i]);
//}
//printf("\n");
        //Salva o byte atual
        for(i=0; i<5; i++){//Passa os 5 bits do vetor para o buffer completar 8 bits e poder gravar um byte.
            buff62.vetor[buff62.contador] = vetorBits[i];
            buff62.contador++;
        }
//printf("cont: %d\nBuffVetor+5: ", buff62.contador);
//for(i=0; i<(buff62.contador); i++){
//printf("%d ", buff62.vetor[i]);
//}
//printf("\n");

    }
    else{
        if(iA == 1 || iA == 0 ){
            buff62.resto = iA;
//printf("\niA = %d\n\n",iA);
            return buff62;
        }

        for(i=0; i<6; i++){ //Salva a sequencia binário em um vetor
            k = iA >> i;
            if(k & 1){
                vetorBits[5-i] = 1;
            }
            else{
                vetorBits[5-i] = 0;
            }
        }
//printf("%d - %d%d%d%d%d%d\n",iA, vetorBits[0], vetorBits[1], vetorBits[2], vetorBits[3], vetorBits[4], vetorBits[5]);

        for(i=0; i<6; i++){//Passa os 6 bits do vetor para o buffer completar 8 bits e poder gravar um byte.
            buff62.vetor[buff62.contador] = vetorBits[i];
            buff62.contador++;
        }

//printf("BuffVetor: ");
//for(i=0; i<(buff62.contador); i++){
//printf("%d ", buff62.vetor[i]);
//}
//printf("\n");
    }

    while(buff62.contador > 7) { //Caso o contador esteja em 6 ou mais, temos pelo menos um byte (8 bits)
//printf("\nContador: %d\n", buff62.contador);
        cA = 0;
        for(i=0; i<8; i++){ //Tranforma o numero 1110111 em um decimal e coloca no caracter
            cA |= (buff62.vetor[i] == 1) << (7 - i);
        }
//printf("char = %c %d\n",cA, cA);

        for(i=0; i<(26-8); i++){ //Esvazia o buffer e arruma os bits
            buff62.vetor[i] = buff62.vetor[i+8];
        }
        buff62.contador = buff62.contador - 8; //diminui o contador, pelos bits tirados
        fprintf(saida, "%c",  cA);

//printf("cont: %d\nBuffVetorNovo: ", buff62.contador);
//for(i=0; i<(buff62.contador); i++){
//printf("%d ", buff62.vetor[i]);
//}
//printf("\n");
    }

//system("pause");
    return buff62;
}

void fim(struct buffer buff62, FILE *saida){
    char cA;
    int i;
    switch(buff62.contador){
        case (1):{
            buff62.vetor[buff62.contador] = 0;
        buff62.contador++;
        }
        case (2):{
        buff62.vetor[buff62.contador] = 0;
        buff62.contador++;
        }
        case (3):{
        buff62.vetor[buff62.contador] = 0;
        buff62.contador++;
        }
        case (4):{
        buff62.vetor[buff62.contador] = 0;
        buff62.contador++;
        }
        case (5):{
        buff62.vetor[buff62.contador] = 0;
        buff62.contador++;
        }
        case (6):{
        buff62.vetor[buff62.contador] = 0;
        buff62.contador++;
        }
        case (7):{
        buff62.vetor[buff62.contador] = 0;
        buff62.contador++;
        }
    }
//printf("buffcont = %d", buff62.contador);

while(buff62.contador > 7) { //Caso o contador esteja em 6 ou mais, temos pelo menos um byte (8 bits)
//printf("\nContador: %d\n", buff62.contador);
        cA = 0;
        for(i=0; i<8; i++){ //Tranforma o numero 1110111 em um decimal e coloca no caracter
            cA |= (buff62.vetor[i] == 1) << (7 - i);
        }
//printf("char = %c %d\n",cA, cA);

        for(i=0; i<(26-8); i++){ //Esvazia o buffer e arruma os bits
            buff62.vetor[i] = buff62.vetor[i+8];
        }
        buff62.contador = buff62.contador - 8; //diminui o contador, pelos bits tirados
        fprintf(saida, "%c",  cA);

//printf("cont: %d\nBuffVetorNovo: ", buff62.contador);
//for(i=0; i<(buff62.contador); i++){
//printf("%d ", buff62.vetor[i]);
//}
//printf("\n");
    }
}
#endif

