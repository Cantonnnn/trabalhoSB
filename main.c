#include <stdio.h>
#include <stdlib.h>
#include "gera_codigo.h"

int main(void)
{
    FILE *fp;
    unsigned char code[500];
    funcp funcLBS;
    int resultado;

    fp = fopen("programa", "r");
    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo");
        return 1;
    }
    gera_codigo(fp, code, &funcLBS);
    resultado = funcLBS(10);
    printf("Resultado: %d\n", resultado);
    fclose(fp);
    return 0;
}