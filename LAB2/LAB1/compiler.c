#include <stdio.h>    // likha kyunki hum fopen fclose FILE ye sab use karenge jo ki isme hote hai
#include "compiler.h" // kyunki iske andar coid compile declare hai to use use karne ke liye

void compile()
{
    FILE *input;                // FILE ek data type hai, ye input file ka pointer banaya
    FILE *output;
    char line[100];    // jo  bhi line like Read x1,0 hogi wo isme store hogi pahle (only single line will get store)
                        // Read x1,0 ye string hai integer nhi to use char aaray mein store kar rhe hai
    int dest, src1, src2, value;   // agar x1 = x2+x3 hai to destination = x1, source1 = x2, source2 = x3
                                   // aur agar x1 = 10 hai to destination = x1 and value = 10
    char op;   // isme opcode hoga like op = '+'


    input = fopen("program.txt", "r");  // program.txt ko open karo aur 'r' read karo file ka address input variable mein dalo
    if(input == NULL)  // agar abc.txt hai jo bani hi nhi to adress bhi nhi hoga to if input is 0 to handle kiya use
    {
        printf("Cannot open program.txt\n");
        return;
    }
    output = fopen("program.byte", "w");   // program.bytte ki nayi file banegi 

    while(fgets(line, sizeof(line), input)) /*jo array banaya tha char ka line usme store karna hai line ko jo ki maximum 
                                            100 character(size of char line) tak read karta hai kise ? jo program.text haina inpute mein use
                                            jaise Read x1,0 ye fget ke andar aata hai then dusri line then tisrio line aise*/
            /*fgets()
            Reads one complete line from the file.
            Stores it in the character array line.
            Returns a pointer on success and NULL at the end of the file.
            while(fgets(...)) keeps reading until the file ends.*/
    {
        // Read
        if(sscanf(line,"Read x%d,%d",&dest,&value)==2)  /*sscanf line ko tukdo mein divide karta hai
        eg Read x1,0 to Read then destination mein x1 and location.value mein 0 */

        /* ye 2 batata hai kitne %d hai i.e kitne value successfully read hui hai*/
        {
            fprintf(output,"5 %d %d 0\n",dest,value);    // phir use output file program.byte mein erite kar deta hai
        }
        // Write
        else if(sscanf(line,"Write x%d,%d",&dest,&value)==2) // same like read bss read kemjagah write hai
        {
            fprintf(output,"6 %d %d 0\n",dest,value); // print -> print on screen, fprint-> print on file
        }
        // Arithmetic
        else if(sscanf(line,"x%d = x%d %c x%d",&dest,&src1,&op,&src2)==4)
        {
            int opcode;

            if(op=='+')                  /* x1 = x2 + x3 hai to ye line array mein tore hoga then dest =1 
                                            src1 = 2, src2 = 3 mil jayege */
                                        /* phir yaha + hai to opcode 1 ho jayega*/  
                opcode=1;

            else if(op=='-')
                opcode=2;

            else if(op=='*')
                opcode=3;

            else
                opcode=4;

            fprintf(output,"%d %d %d %d\n",     // phit usko file mein as opcode dest, src1 src2 mein print kardege
                    opcode,dest,src1,src2);     // like 1 1 2 3
        }
        // Constant
        else if(sscanf(line,"x%d = %d",&dest,&value)==2)  // agar x1 = 4 hai to hum 2 value read kar rhe hai so 2
        {
            fprintf(output,"7 %d %d 0\n",   // phir usko 7 1 4 0 likhdo as 7  is opcode of = 
                    dest,value);
        }

    }
    fprintf(output,"0 0 0 0\n"); // opcode 0 dekhkar processor program ko end kardega
    fclose(input);    // file open ke baad close karne par memory free hoti hai
    fclose(output);
}