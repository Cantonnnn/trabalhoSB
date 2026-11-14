/*Eduardo Canton Ferreira e Silva 2410837 3WC*/
/**/
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
    int i = 0; //function
    int valor; //zret
    int var; //vN = call NUM vM
    int origem; //vN = p0 + $valor
    int num_func; //vN = vM - $valor
    int param_salvo = 0; //ret $valor
    int function_start[50]; //ret vN
    int num_functions = 0; //end
    int deslocamento_pilha;
    *entry = NULL;
    while (fgets(linha, sizeof(linha), f) != NULL)
    {
        if (sscanf(linha, "%19s", comando) != 1)
        {
            continue;
        }
        if (strcmp(comando, "function") == 0)
        {
            function_start[num_functions] = i;
            num_functions++;
            *entry = (funcp) &code[i];

            param_salvo = 0;
            prologo(code, &i);
        }
        else if (sscanf(linha, "zret p0 $%d", &valor) == 1)
        {
            int salto;
            if (!param_salvo)
            {
                code[i++] = 0x89;
                code[i++] = 0x7D;
                code[i++] = 0xFC;
                param_salvo = 1;
            }
            code[i++] = 0x83;
            code[i++] = 0x7D;
            code[i++] = 0xFC;
            code[i++] = 0x00;

            code[i++] = 0x75;
            salto = i;
            code[i++] = 0x00;

            code[i++] = 0xB8;
            code[i++] = valor & 0xFF;
            code[i++] = (valor >> 8) & 0xFF;
            code[i++] = (valor >> 16) & 0xFF;
            code[i++] = (valor >> 24) & 0xFF;

            epilogo(code, &i);
            code[salto] = (unsigned char)(i - (salto + 1));
        }
        else if (sscanf(linha, "v%d = p0 + $%d", &var, &valor) == 2)
        {
            deslocamento_pilha = -8 - 4 * var;
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
            code[i++] = deslocamento_pilha & 0xFF;
        }
        else if (sscanf(linha, "v%d = p0 * p0", &var) == 1)
        {
            deslocamento_pilha = -8 - 4 * var;

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

            code[i++] = 0x0F;
            code[i++] = 0xAF;
            code[i++] = 0x45;
            code[i++] = 0xFC;

            code[i++] = 0x89;
            code[i++] = 0x45;
            code[i++] = deslocamento_pilha & 0xFF;
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
            deslocamento_pilha = -8 - 4 * var;
            code[i++] = 0x8B;
            code[i++] = 0x45;
            code[i++] = deslocamento_pilha & 0xFF;
        }

        else if (sscanf(linha, "v%d = v%d - $%d", &var, &origem, &valor) == 3)
        {
            int desloDestino = -8 - 4 * var;
            int desloOrigem = -8 - 4 * origem;

            code[i++] = 0x8B;
            code[i++] = 0x45;
            code[i++] = desloOrigem & 0xFF;

            code[i++] = 0x83;
            code[i++] = 0xE8;
            code[i++] = valor & 0xFF;

            code[i++] = 0x89;
            code[i++] = 0x45;
            code[i++] = desloDestino & 0xFF;
        }

        else if (sscanf(linha, "v%d = call %d v%d", &var, &num_func, &origem) == 3)
        {
            int desloDestino = -8 - 4 * var;
            int desloOrigem = -8 - 4 * origem;
            int deslocamento_call;
            unsigned int rel;

            code[i++] = 0x8B;
            code[i++] = 0x7D;
            code[i++] = desloOrigem & 0xFF;

            code[i++] = 0xE8;

            deslocamento_call = function_start[num_func] - (i + 4);
            rel = (unsigned int) deslocamento_call;

            code[i++] = rel & 0xFF;
            code[i++] = (rel >> 8) & 0xFF;
            code[i++] = (rel >> 16) & 0xFF;
            code[i++] = (rel >> 24) & 0xFF;

            code[i++] = 0x89;
            code[i++] = 0x45;
            code[i++] = desloDestino & 0xFF;
        }
        else if (strcmp(comando, "end") == 0)
        {
            epilogo(code, &i);
        }
    }
}