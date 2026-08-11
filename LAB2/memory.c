#include <stdio.h>
#include "memory.h"

unsigned char Instruction[256];
unsigned char Data[4096];

void initialize(void)
{
    FILE *program;
    FILE *data;
    unsigned int value;
    int i;

    for(i = 0; i < 256; i++)
        Instruction[i] = 0;

    for(i = 0; i < 4096; i++)
        Data[i] = 0;

    program = fopen("program.byte", "r");

    if(program != NULL)
    {
        i = 0;

        while(i < 256 && fscanf(program, "%x", &value) == 1)
        {
            Instruction[i] = (unsigned char)value;
            i++;
        }

        fclose(program);
    }

    data = fopen("data.byte", "r");

    if(data != NULL)
    {
        i = 0;

        while(i < 4096 && fscanf(data, "%x", &value) == 1)
        {
            Data[i] = (unsigned char)value;
            i++;
        }

        fclose(data);
    }
}

void finalize(void)
{
    FILE *data;
    int i;

    data = fopen("data.byte", "w");

    if(data == NULL)
        return;

    for(i = 0; i < 4096; i += 4)
    {
        fprintf(data, "%02X %02X %02X %02X\n",
                Data[i], Data[i + 1],
                Data[i + 2], Data[i + 3]);
    }

    fclose(data);
}
