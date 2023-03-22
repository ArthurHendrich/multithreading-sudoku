#include <stdio.h>
#include <stdlib.h>
#include "aux.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Invalid number of parameters");
        exit(1);
    }

    execThread(argv[1]);
    
    int final = globalResult;

    if (final == 0) {
        writeFile("SUCCESS");
    }
    else {
        writeFile("FAIL");
    }

    exit(1);
}