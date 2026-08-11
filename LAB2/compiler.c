#include <stdio.h>   // Standard library use karne ke liye like fopen() fclose()
#include <string.h>
#include "compiler.h"

char labelName[100][30];     
/* Ye teen variables milkar label table banate hain.
Imagine:
labelName             labelAddress

[0] ".start"          [0] 0
[1] ".loop"           [1] 12
[2] ".exit"           [2] 28

labelCount: kitne labels stored hain*/
int labelAddress[100];
int labelCount = 0;

//Compiler ko batadeta hai ki ye saare function exist karte hai
void removeComment(char line[]);
void addLabel(char name[], int address);
int findLabel(char name[]);
int branchCode(char name[]);
void pass1(void);
void pass2(void);

/* Ye Byte Code Write karta hai 
Eg :
%02X
X → hexadecimal
2 → minimum 2 digits
0 → leading zero

So:
1 → 01
10 → 0A
15 → 0F

& 255 : 255 decimal = FF hex
& 255 lower 8 bits retain karta hai.*/
void writeInstruction(FILE *out, int op, int d, int a, int b)
{
    fprintf(out, "%02X %02X %02X %02X\n",
            op & 255, d & 255, a & 255, b & 255);
}


// agar % mil gaya to parser read nhi karge i.e /0 string end kardege 
void removeComment(char line[])
{
    int i = 0;

    while(line[i] != '\0')
    {
        if(line[i] == '%')
        {
            line[i] = '\0';
            break;
        }
        i++;
    }
}


/* Label ka naam aur uska address table me store karna.
Suppose:
.loop
address: 12
Call logically:
addLabel(".loop", 12)

Then:
labelName[0] = ".loop"
labelAddress[0] = 12
labelCount = 1*/
void addLabel(char name[], int address)
{
    if(labelCount >= 100)
        return;

    strcpy(labelName[labelCount], name);
    labelAddress[labelCount] = address;
    labelCount++;
}

int findLabel(char name[])
{
    int i;

    for(i = 0; i < labelCount; i++)
    {
        if(strcmp(labelName[i], name) == 0)
            return labelAddress[i];
    }

    return -1;
}

int branchCode(char name[])
{
    if(strcmp(name, "BEQ") == 0) return 0;
    if(strcmp(name, "BNE") == 0) return 1;
    if(strcmp(name, "BCS") == 0) return 2;
    if(strcmp(name, "BCC") == 0) return 3;
    if(strcmp(name, "BMI") == 0) return 4;
    if(strcmp(name, "BPL") == 0) return 5;
    if(strcmp(name, "BVS") == 0) return 6;
    if(strcmp(name, "BVC") == 0) return 7;
    if(strcmp(name, "BHI") == 0) return 8;
    if(strcmp(name, "BLS") == 0) return 9;
    if(strcmp(name, "BGE") == 0) return 10;
    if(strcmp(name, "BLT") == 0) return 11;
    if(strcmp(name, "BGT") == 0) return 12;
    if(strcmp(name, "BLE") == 0) return 13;
    if(strcmp(name, "BAL") == 0) return 14;

    return -1;
}

void pass1(void)
{
    FILE *input;
    char line[100];
    char name[30];
    int address = 0;

    input = fopen("program.txt", "r");

    if(input == NULL)
        return;

    while(fgets(line, sizeof(line), input))
    {
        removeComment(line);

        if(line[0] == '\0' || line[0] == '\n')
            continue;

        if(line[0] == '.')
        {
            if(sscanf(line, "%29s", name) == 1)
                addLabel(name, address);

            continue;
        }

        address += 4;
    }

    fclose(input);
}

void pass2(void)
{
    FILE *input;
    FILE *output;
    char line[100];
    int address = 0;

    input = fopen("program.txt", "r");
    output = fopen("program.byte", "w");

    if(input == NULL || output == NULL)
    {
        if(input != NULL) fclose(input);
        if(output != NULL) fclose(output);
        return;
    }

    while(fgets(line, sizeof(line), input))
    {
        int d, a, b, value;
        char op;
        char command[10];
        char label[30];

        removeComment(line);

        if(line[0] == '\0' || line[0] == '\n')
            continue;

        if(line[0] == '.')
            continue;

        /* Branch */
        if(sscanf(line, " %9s %29s", command, label) == 2)
        {
            int code = branchCode(command);

            if(code != -1)
            {
                int target = findLabel(label);
                int offset = (target - address) / 4;

                writeInstruction(output, 0x10 + code, 0, 0, offset);
                address += 4;
                continue;
            }
        }

        /* Constant movement: x1 = 10 */
        if(sscanf(line, " x%d = %d", &d, &value) == 2)
        {
            writeInstruction(output, 0x0F, d, 0, value);
            address += 4;
            continue;
        }

        /* Register arithmetic: x1 = x2 + x3 */
        if(sscanf(line, " x%d = x%d %c x%d", &d, &a, &op, &b) == 4)
        {
            int code = 0;

            if(op == '+') code = 0x01;
            else if(op == '-') code = 0x02;
            else if(op == '*') code = 0x03;
            else if(op == '/') code = 0x04;

            if(code != 0)
            {
                writeInstruction(output, code, d, a, b);
                address += 4;
                continue;
            }
        }

        /* Constant arithmetic: x1 = x2 + 10 */
        if(sscanf(line, " x%d = x%d %c %d", &d, &a, &op, &value) == 4)
        {
            int code = 0;

            if(op == '+') code = 0x09;
            else if(op == '-') code = 0x0A;
            else if(op == '*') code = 0x0B;
            else if(op == '/') code = 0x0C;

            if(code != 0)
            {
                writeInstruction(output, code, d, a, value);
                address += 4;
                continue;
            }
        }

        /* x1 = [x2] */
        if(sscanf(line, " x%d = [x%d]", &d, &a) == 2)
        {
            writeInstruction(output, 0x05, d, a, 0);
            address += 4;
            continue;
        }

        /* x1 = [10] */
        if(sscanf(line, " x%d = [%d]", &d, &value) == 2)
        {
            writeInstruction(output, 0x0D, d, 0, value);
            address += 4;
            continue;
        }

        /* [x2] = x1 */
        if(sscanf(line, " [x%d] = x%d", &a, &b) == 2)
        {
            writeInstruction(output, 0x06, a, b, 0);
            address += 4;
            continue;
        }

        /* Legacy Read x1,10 */
        if(sscanf(line, " Read x%d,%d", &d, &value) == 2)
        {
            writeInstruction(output, 0x0D, d, 0, value);
            address += 4;
            continue;
        }

        /* Legacy Write x1,10 */
        if(sscanf(line, " Write x%d,%d", &d, &value) == 2)
        {
            writeInstruction(output, 0x0E, value, d, 0);
            address += 4;
            continue;
        }
    }

    writeInstruction(output, 0, 0, 0, 0);

    fclose(input);
    fclose(output);
}

void compile(void)
{
    labelCount = 0;
    pass1();
    pass2();
}
