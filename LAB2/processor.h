#ifndef PROCESSOR_H
#define PROCESSOR_H

extern int Register[256];

extern int PC;
extern int opcode;
extern int dest;
extern int src1;
extern int src2;

extern int Z;
extern int N;
extern int C;
extern int V;

extern int end_of_simulation;

void reset(void);
void fetch(void);
void decode(void);
void execute(void);

#endif
