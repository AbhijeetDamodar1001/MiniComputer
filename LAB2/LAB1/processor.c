// Processor ka simple sa kaam hai 5 1 8 0 ye jo instructon hsi use Smjhkana aur Execuite karna
// Mmeory ne jo Instruction[] mein dala hai usko execute karta hai Processor/CPU

#include <stdio.h>
#include "processor.h"
#include "memory.h"

void reset()  // jaise computer restar kiya to sab reset hota haina like RAM reset sab reset ho jata haina
{
    for(int i=0;i<256;i++)   // agar last program mein R1[0]=80 hai to is program mein thodi use karenge so reset mein
                        // humne saare register mein 0 value daldi i.e register bhi reset kardiye
    {
        Register[i]=0;
    }
    PC=0;   // starting se instruction fetch karna hai
    end_of_simulation=0;// abhi program ko chalana hai
}
//Computer Architecture and Organization

void fetch()// ye ab instruction ko fetch/utha kar layega instructuon memory se 

{    // 5 1 8 0 
    opcode = Instruction[PC]; // to 5 ko opcode meindalo  // ye wohi jo memory mein Instruction[0....] banaya tha use acces karega
    dest = Instruction[PC+1]; // ab pc+1 karke 1 ko dest mein dalo
    src1 = Instruction[PC+2];// ab pc+2 karke 8 ko src1 mwin dalo
    src2 = Instruction[PC+3];// ab 0 ko src2 mein dalo
}
void decode()
{

}
void execute()
{
    if(opcode==0)  // agar opcode 0 hai like 0 0 0 0 to program khatam i.e ye halt instruction he
    {
        end_of_simulation=1;
        return;
    }
    else if(opcode==1) // agar opcode 1 hai to add karna hai
    {
        Register[dest]=Register[src1]+Register[src2];// to src1 + src1 ko dest par dal do register ke
    }
    else if(opcode==2)
    {
        Register[dest]=Register[src1]-Register[src2];
    }
    else if(opcode==3) // agar opcode 3 hai to multioly karna hai to
    {
        Register[dest]=Register[src1]*Register[src2]; //src1*src2 ko destination mein dal to register ke
    }
    else if(opcode==4)
    {
        Register[dest]=Register[src1]/Register[src2];
    }
    else if(opcode==5)  // read hai to bss value ko register mein daldo
    {
        Register[dest]=Data[src1];
    }
    else if(opcode==6)// write kanra hai to data update kardo
    {
        Data[src1]=Register[dest];
    }
    else if(opcode==7)  // bss assign kar0
    {
        Register[dest]=src1;
    }
    PC=PC+4;// as instructuon is 4 byte ki 4 se update kiya
}