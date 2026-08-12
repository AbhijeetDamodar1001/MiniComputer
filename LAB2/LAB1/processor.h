#ifndef PROCESSOR_H
#define PROCESSOR_H

int Register[256];  // CPU ke andar ke register jo value store karega

int PC;  // Program counter jo instruction ka adress store karega jo execute karna hai

int opcode; // konsa operation karna hai uska opcode rahta hai jaise read ka 5 hai
int dest;
int src1;
int src2;

int end_of_simulation;

void reset();
void fetch();
void decode();
void execute();

#endif