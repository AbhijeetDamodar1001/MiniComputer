#include <stdio.h>
#include "compiler.h"
#include "memory.h"
#include "processor.h"

int main(void)
{
    printf("================================\n");
    printf("       MINI COMPUTER LAB 2\n");
    printf("================================\n");

    printf("Compiling program...\n");
    compile();

    printf("Loading memory...\n");
    initialize();

    printf("Resetting processor...\n");
    reset();

    printf("Executing program...\n");

    while(!end_of_simulation)
    {
        fetch();
        decode();
        execute();
    }

    printf("Saving data memory...\n");
    finalize();

    printf("Program executed successfully!\n");

    return 0;
}
