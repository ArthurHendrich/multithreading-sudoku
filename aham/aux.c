#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

#define BUFFER_SIZE 20000

int globalResult = 0;

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

sudoku readFile(char *fileName)
{
    sudoku sudoku;
    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("Error to read file");
        exit(1);
    }

    char line[BUFFER_SIZE];
    int gradeFirstSize, gradeSecondSize;
    int subGradeFirstSize, subGradeSecondSize;

    if (fgets(line, BUFFER_SIZE, file) == NULL)
    {
        printf("File out of format");
        exit(1);
    }

    char *token = strtok(line, "x");
    if (token == NULL)
    {
        printf("File out of format");
        exit(1);
    }
    gradeFirstSize = atoi(token);
    token = strtok(NULL, "x");
    if (token == NULL)
    {
        printf("File out of format");
        exit(1);
    }
    gradeSecondSize = atoi(token);
    if (strtok(NULL, "x") != NULL)
    {
        printf("File out of format");
        exit(1);
    }

    if (gradeFirstSize != gradeSecondSize)
    {
        printf("File out of format");
        exit(1);
    }

    if (fgets(line, BUFFER_SIZE, file) == NULL)
    {
        printf("File out of format");
        exit(1);
    }

    token = strtok(line, "x");
    if (token == NULL)
    {
        printf("File out of format");
        exit(1);
    }
    subGradeFirstSize = atoi(token);
    token = strtok(NULL, "x");
    if (token == NULL)
    {
        printf("File out of format");
        exit(1);
    }
    subGradeSecondSize = atoi(token);
    if (strtok(NULL, "x") != NULL)
    {
        printf("File out of format");
        exit(1);
    }

    if ((subGradeFirstSize * subGradeSecondSize) != gradeFirstSize)
    {
        printf("File out of format");
        exit(1);
    }

    int size = gradeFirstSize;

    sudoku.size = size;
    sudoku.subGradeSizeA = subGradeFirstSize;
    sudoku.subGradeSizeB = subGradeSecondSize;

    sudoku.matrix = malloc(size * sizeof(int *));
    if (sudoku.matrix == NULL)
    {
        printf("Error to read file");
        exit(1);
    }

    int *matrixMemory = malloc(size * size * sizeof(int));
    if (matrixMemory == NULL)
    {
        printf("Error to read file");
        exit(1);
    }

    for (int i = 0; i < size; i++)
    {
        sudoku.matrix[i] = matrixMemory + i * size;
        if (sudoku.matrix[i] == NULL)
        {
            printf("Error to read file");
            exit(1);
        }
    }

    int i = 0;
    int lines = 0;

    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }   

    if (lines == size) {
        printf("File out of format");
        exit(1);
    }

    lines = 0;

    rewind(file);
    fgets(line, BUFFER_SIZE, file);
    fgets(line, BUFFER_SIZE, file);


    while (fgets(line, BUFFER_SIZE, file) != NULL)
    {
        // Checagem de espaço extra no início da linha
        int lineLength = strlen(line);
        if (line[lineLength - 1] == '\n')
        {
            line[lineLength - 1] = '\0';
            lineLength--;
        }

        // Checagem de espaço extra no final da linha
        if (line[lineLength - 1] == ' ')
        {
            printf("File out of format");
            exit(1);
        }
        token = strtok(line, " ");

        int j = 0;
        while (token != NULL && j < size)
        {
            // Verifica se há algum caractere inválido antes do primeiro número na linha
            for (int k = 0; token[k] != '\0'; k++)
            {
                // Verifica se o caractere é um número
                if (isdigit(token[k]) == 0 && token[k] != '\n') 
                {
                    printf("File out of format");
                    exit(1);
                }
            }
            sudoku.matrix[i][j] = atoi(token);

            if (sudoku.matrix[i][j] > size || sudoku.matrix[i][j] < 1)
            {
                printf("File out of format");
                exit(1);
            }

            j++;
            token = strtok(NULL, " ");
        }

        // Verifica se há algum caractere inválido após o último número na linha
        if (token != NULL && strcmp(token, "\n") != 0)
        {
            printf("File out of format");
            exit(1);
        }

        if (j != size)
        {
            printf("File out of format");
            exit(1);
        }

        lines++;
        i++;

        if (lines > size)
        {
            printf("File out of format");
            exit(1);
        }
    }

    if (lines < size)
    {
        printf("File out of format");
        exit(1);
    }

    if (fgets(line, BUFFER_SIZE, file) != NULL)
    {
        printf("File out of format");
        exit(1);
    }

    fclose(file);
    return sudoku;
}

void writeFile(char *texto)
{
    FILE *output;
    output = fopen("sudoku_aham.out", "w");
    if (output == NULL)
    {
        printf("Error to read file");
        return;
    }
    fprintf(output, "%s", texto);
    fclose(output);
}

void printMatrix(int size, int **matrix)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void *verifyRow(void *data)
{
    datastruct *rowData = (datastruct *)data;
    int repeatedNumber = 0;

    int size = rowData->sudoku.size;
    int **matrix = rowData->sudoku.matrix;

    int init = rowData->init;
    int end = rowData->end;

    for (int i = init; i < end; i++)
    {
        int aux[size];
        memset(aux, 0, size * sizeof(int));

        for (int j = 0; j < size; j++)
        {
            int number = matrix[i][j];

            if (aux[number - 1] == 1)
            {
                repeatedNumber += 1;
                break;
            }

            aux[number - 1] = 1;
        }
    }

    if (repeatedNumber > 0)
    {
        // Varias threads atualizam a variavel global sem causar condições de corrida
        __sync_fetch_and_add(&globalResult, 1);
        return NULL;
    }

    return NULL;
}

void *verifyColumn(void *data)
{
    datastruct *columnData = (datastruct *)data;
    int repeatedNumber = 0;

    int size = columnData->sudoku.size;
    int **matrix = columnData->sudoku.matrix;

    int init = columnData->init;
    int end = columnData->end;

    for (int j = init; j < end; j++)
    {
        int aux[size];
        memset(aux, 0, size * sizeof(int));

        for (int i = 0; i < size; i++)
        {
            int number = matrix[i][j];

            if (aux[number - 1] == 1)
            {
                repeatedNumber += 1;
                break;
            }

            aux[number - 1] = 1;
        }
    }

    if (repeatedNumber > 0)
    {
        __sync_fetch_and_add(&globalResult, 1);
        return NULL;
    }

    return NULL;
}

void *verifySubGrade(void *data)
{
    datastruct *matrix = (datastruct *)data;

    int size = matrix->sudoku.size;
    int gradeSizeA = matrix->sudoku.subGradeSizeA;
    int gradeSizeB = matrix->sudoku.subGradeSizeB;
    // int A = size / gradeSizeA;
    int B = size / gradeSizeB;
    // times = A * B

    int init = matrix->init;
    int end = matrix->end;

    int **sortMatrix = matrix->sudoku.matrix;

    while (init < end)
    {
        int i;
        int j;
        int x = 0;
        int subSize = (gradeSizeA * gradeSizeB) + 1;  // Tamanho da sub-grade
        int subGradeIndexA = (init / B) * gradeSizeA; // Índice da sub-grade na linha
        int subGradeIndexB = (init % B) * gradeSizeB; // Índice da sub-grade na coluna

        int aux[subSize];
        memset(aux, 0, sizeof(aux)); // Inicializa todos os elementos do vetor auxiliar com 0

        for (i = subGradeIndexA; i < subGradeIndexA + gradeSizeA; i++)
        {
            for (j = subGradeIndexB; j < subGradeIndexB + gradeSizeB; j++)
            {
                int value = sortMatrix[i][j]; // Valor do elemento atual
                if (value == 0 || value > size)
                { // Verifica se o valor está no intervalo correto
                    x = 1;
                    break;
                }
                else
                {
                    if (aux[value] != 0)
                    { // Verifica se o valor já apareceu na sub-grade
                        x = 1;
                        break;
                    }
                    else
                    {
                        aux[value] = value; // Se não apareceu, adiciona o valor no vetor auxiliar
                    }
                }
            }
            if (x == 1)
            {
                break;
            }
        }

        if (x != 0)
        {
            __sync_fetch_and_add(&globalResult, 1);
            return NULL;
        }

        init++;
    }

    return NULL;
}

void *threadFunc(void *data)
{
    datastruct *thread = (datastruct *)data;

    int init = thread->init;
    int end = thread->end;
    sudoku sudoku = thread->sudoku;

    for (int i = init; i < end; i++)
    {
        // Novo struct para cada parte da tarefa
        datastruct task;

        task.id = i;
        task.sudoku = sudoku;
        task.init = init;
        task.end = end;

        verifyRow(&task);
        verifyColumn(&task);
        verifySubGrade(&task);
    }

    return NULL;
}

void *execThread(char *fileName)
{
    sudoku sudoku = readFile(fileName);

    // int numThreads = sysconf(_SC_NPROCESSORS_ONLN);
    int numThreads = 12;
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));

    if (threads == NULL)
    {
        printf("File out of format");
        exit(1);
    }

    // Cria as threads
    datastruct *threadsInit = malloc(numThreads * sizeof(datastruct));

    double cellsPerThread = (double)sudoku.size / numThreads; // Calcula células por thread como um valor decimal
    int tasks = (int)ceil(cellsPerThread);                    // Arredonda para cima (ceil) o valor decimal

    for (int i = 0; i < numThreads; i++)
    {
        int taskInit = i * tasks;
        int taskEnd = (i + 1) * tasks;

        if (taskEnd > sudoku.size)
        {
            taskEnd = sudoku.size;
        }

        threadsInit[i].id = i;
        threadsInit[i].init = taskInit;
        threadsInit[i].end = taskEnd;
        threadsInit[i].sudoku = sudoku;

        // printf("Thread %d: task_start=%d, task_end=%d\n", i, taskInit, taskEnd);

        pthread_create(&threads[i], NULL, threadFunc, (void *)&threadsInit[i]);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(threadsInit);

    return NULL;
}