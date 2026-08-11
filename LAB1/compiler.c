#include <stdio.h>    
#include "compiler.h" 

void compile()
{
    FILE *input;               
    FILE *output;
    char line[100];    
    int dest, src1, src2, value;  
    char op;  


    input = fopen("program.txt", "r");  
    if(input == NULL)  
    {
        printf("Cannot open program.txt\n");
        return;
    }
    output = fopen("program.byte", "w");  

    while(fgets(line, sizeof(line), input)) 
    {
        
        if(sscanf(line,"Read x%d,%d",&dest,&value)==2)  
        {
            fprintf(output,"5 %d %d 0\n",dest,value);   
        }
        
        else if(sscanf(line,"Write x%d,%d",&dest,&value)==2) 
        {
            fprintf(output,"6 %d %d 0\n",dest,value); 
        }
       
        else if(sscanf(line,"x%d = x%d %c x%d",&dest,&src1,&op,&src2)==4)
        {
            int opcode;

            if(op=='+')                   
                opcode=1;

            else if(op=='-')
                opcode=2;

            else if(op=='*')
                opcode=3;

            else
                opcode=4;

            fprintf(output,"%d %d %d %d\n",     
                    opcode,dest,src1,src2);    
        }
        
        else if(sscanf(line,"x%d = %d",&dest,&value)==2)  
        {
            fprintf(output,"7 %d %d 0\n",   
                    dest,value);
        }

    }
    fprintf(output,"0 0 0 0\n");
    fclose(input);  
    fclose(output);
}