
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_codigo.h"

static void prologo(unsigned char code[], int *i)
{
    code[(*i)++] = 0x55;

    code[(*i)++] = 0x48;
    code[(*i)++] = 0x89;
    code[(*i)++] = 0xE5;

    code[(*i)++] = 0x48;
    code[(*i)++] = 0x83;
    code[(*i)++] = 0xEC;
    code[(*i)++] = 0x20;
}

static void epilogo(unsigned char code[], int *i)
{
    code[(*i)++] = 0xC9;
    code[(*i)++] = 0xC3;
}


void gera_codigo(FILE *f, unsigned char code[], funcp *entry)
{
    char linha[100];
    char comando[20];
    int i = 0;
    int valor;
    int var;
    int param_salvo = 0;
    int deslocamento;
    *entry = NULL;
    while (fgets(linha, sizeof(linha), f) != NULL)
    {
        if (sscanf(linha, "%19s", comando) != 1)
        {
            continue;
        }
        if (strcmp(comando, "function") == 0)
        {
            *entry = (funcp) &code[i];

            param_salvo = 0;

            prologo(code, &i);
        }
        else if (sscanf(linha, "v%d = p0 + $%d", &var, &valor) == 2)
        {
            deslocamento = -8 - 4 * var;
            if (!param_salvo)
            {
                code[i++] = 0x89;
                code[i++] = 0x7D;
                code[i++] = 0xFC;

                param_salvo = 1;
            }
            code[i++] = 0x8B;
            code[i++] = 0x45;
            code[i++] = 0xFC;

            if (valor >= -128 && valor <= 127)
            {
                code[i++] = 0x83;
                code[i++] = 0xC0;
                code[i++] = valor & 0xFF;
            }
            else
            {
                code[i++] = 0x81;
                code[i++] = 0xC0;

                code[i++] = valor & 0xFF;
                code[i++] = (valor >> 8) & 0xFF;
                code[i++] = (valor >> 16) & 0xFF;
                code[i++] = (valor >> 24) & 0xFF;
            }
            code[i++] = 0x89;
            code[i++] = 0x45;
            code[i++] = deslocamento & 0xFF;
        }
        else if (sscanf(linha, "ret $%d", &valor) == 1)
        {
            code[i++] = 0xB8;

            code[i++] = valor & 0xFF;
            code[i++] = (valor >> 8) & 0xFF;
            code[i++] = (valor >> 16) & 0xFF;
            code[i++] = (valor >> 24) & 0xFF;
        }
        else if (sscanf(linha, "ret v%d", &var) == 1)
        {
            deslocamento = -8 - 4 * var;
            code[i++] = 0x8B;
            code[i++] = 0x45;
            code[i++] = deslocamento & 0xFF;
        }
        else if (strcmp(comando, "end") == 0)
        {
            epilogo(code, &i);
        }
    }
}