#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_codigo.h"

void gera_codigo(FILE *f, unsigned char code[], funcp *entry)
{
    char palavra[20];
    int valor;
    unsigned int bits;

    fscanf(f, "%19s", palavra);
    fscanf(f, "%19s", palavra);
    fscanf(f, " $%d", &valor);
    bits = (unsigned int) valor;

    code[0] = 0xB8;
    code[1] = bits & 0xFF;
    code[2] = (bits >> 8) & 0xFF;
    code[3] = (bits >> 16) & 0xFF;
    code[4] = (bits >> 24) & 0xFF;
    code[5] = 0xC3;

    *entry = (funcp) code;
}
