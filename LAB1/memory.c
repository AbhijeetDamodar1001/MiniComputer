#include <stdio.h>
#include "memory.h"

void initialize()
{
    FILE *program;  
    FILE *data;    

    int value;  
    int i;

    program = fopen("program.byte","r");
    for(i=0;i<256;i++)
    {
        Instruction[i]=0;
        Data[i]=0;
    }
    if(program!=NULL)
    {
        i=0;
        while(fscanf(program,"%d",&value)==1)
        {
            Instruction[i]=value;
            i++;
        }
        fclose(program);
    }

    data=fopen("data.byte","r");

    if(data!=NULL)
    {
        i=0;
        while(fscanf(data,"%d",&value)==1)
        {
            Data[i]=value;
            i++;
        }
        fclose(data);
    }

}
void finalize()
{
    FILE *data;
    int i;
    data=fopen("data.byte","w");
    for(i=0;i<256;i++)
    {
        fprintf(data,"%d\n",Data[i]);
    }
    fclose(data);
}