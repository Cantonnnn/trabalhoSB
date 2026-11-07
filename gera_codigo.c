#include <stdio.h>
#include "gera_codigo.h"

void gera_codigo(FILE *f, unsigned char code[], funcp *entry)
{
    unsigned char codigo[] ={0xB8, 0x64, 0x00, 0x00, 0x00, 0xC3};
    int i;
    for (i = 0; i < 6; i++)
    {
        code[i] = codigo[i];
    }
    *entry = (funcp) code;
}