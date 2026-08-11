#include <stdio.h>
#include "processor.h"
#include "memory.h"

void reset()  
{
    int i;
    for(i=0;i<256;i++)   
    {
        Register[i]=0;
    }
    PC=0;   
    end_of_simulation=0;
}

void fetch()
{    
    opcode = Instruction[PC]; 
    dest = Instruction[PC+1]; 
    src1 = Instruction[PC+2];
    src2 = Instruction[PC+3];
}
void decode()
{

}
void execute()
{
    if(opcode==0)
    {
        end_of_simulation=1;
        return;
    }
    else if(opcode==1)
    {
        Register[dest]=Register[src1]+Register[src2];
    }
    else if(opcode==2)
    {
        Register[dest]=Register[src1]-Register[src2];
    }
    else if(opcode==3) 
    {
        Register[dest]=Register[src1]*Register[src2];
    }
    else if(opcode==4)
    {
        Register[dest]=Register[src1]/Register[src2];
    }
    else if(opcode==5) 
    {
        Register[dest]=Data[src1];
    }
    else if(opcode==6)
    {
        Data[src1]=Register[dest];
    }
    else if(opcode==7) 
    {
        Register[dest]=src1;
    }
    PC=PC+4;
}