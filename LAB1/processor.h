#ifndef PROCESSOR_H
#define PROCESSOR_H

int Register[256]; 

int PC;

int opcode; 
int dest;
int src1;
int src2;

int end_of_simulation;

void reset();
void fetch();
void decode();
void execute();

#endif