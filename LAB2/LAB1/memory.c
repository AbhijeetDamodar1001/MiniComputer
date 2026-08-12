// CPU hamesha RAM/Memory ke instruction leta hai execute karne ke liyevisiliye hume memory module banana pada

/* Ye Program.byte hai

5 1 0 0

5 2 4 0

1 3 1 2

6 3 8 0

Ab processor ye file nahi padh sakta.

To memory kya karegi?

Ye file kholegi

↓

Ek-ek integer uthayegi

↓

Array me rakh degi.*/

/*5

1

0

0

5

2

4

0

Memory bolti hai

Instruction[0]=5

Instruction[1]=1

Instruction[2]=0

Instruction[3]=0

Instruction[4]=5

Instruction[5]=2

Instruction[6]=4

Instruction[7]=0

Ab processor file nahi padhega.

Processor sirf bolega

Instruction[0]

↓

5

Samajh aaya?

Data Memory bhi same.

Suppose

data.byte
10

0

0

0

20

Memory kya karegi?

Data[0]=10

Data[1]=0

Data[2]=0

Data[3]=0

Data[4]=20*/



#include <stdio.h>
#include "memory.h"

void initialize()
{
    FILE *program;   // ye pointer program.byte ke liye
    FILE *data;    // ye pointer data.byte ke liye

    int value;  /// program.byTe mein jo 5 1 8 0 ye one by one read hokar value mein store hogi uske baad Instruction[] mein jayegi
    int i;// loop ke liye

    program = fopen("program.byte","r");
    for(i=0;i<256;i++)
    {
        Instruction[i]=0;
        Data[i]=0;
    }
    if(program!=NULL)
    {
        i=0;
        while(fscanf(program,"%d",&value)==1)  // fscan ek-ek integer read karta hai jaise 5 then 1 then 0 then 0
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

/*Ab processor program.byte directly nahi padh sakta.

Isliye

program.byte

↓

Memory Module

↓

Instruction[]

Processor sirf

Instruction[0]

Instruction[1]

Instruction[2]

padhta hai.

Memory.h
char Instruction[256];

char Data[256];

Question.

Ye dono arrays kyun banayi?

Instruction[]

Ye RAM ka Instruction Memory hai.

Suppose

program.byte

5 1 0 0

5 2 4 0

1 3 1 2

Memory load karegi

Instruction[0]=5

Instruction[1]=1

Instruction[2]=0

Instruction[3]=0

Instruction[4]=5

Instruction[5]=2

Instruction[6]=4

Instruction[7]=0

Instruction[8]=1

Instruction[9]=3

Instruction[10]=1

Instruction[11]=2

Ab processor file nahi padhega.

Processor bolega

Instruction[PC]

Bas.*/