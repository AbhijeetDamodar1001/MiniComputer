#ifndef MEMORY_H
#define MEMORY_H

char Instruction[256];  // program.byte isme aagegi
char Data[256];  // data.byte isme aayegi

void initialize();
void finalize();

#endif

/* program.byte  / data.byte

↓

Instruction[]    / Data[]

↓

Processor */