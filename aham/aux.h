#include <stdio.h>

typedef struct
{
    int size;
    int subGradeSizeA;
    int subGradeSizeB;
    int **matrix;

} sudoku;

typedef struct
{
    int id;
    int init;
    int end;
    sudoku sudoku;
} datastruct;

extern int globalResult;

void writeFile(char *texto);
void printMatrix(int size, int **matrix);
void *execThread(char *fileName);