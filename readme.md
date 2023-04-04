# Multithreading in Sudoku Validator

The Sudoku Validator utilizes multithreading to efficiently verify if the input Sudoku puzzle is valid or not. In this program, multiple threads are used to perform the validation of rows, columns, and subgrids in parallel, improving the overall performance of the program.

## Requirements



To compile and use this tool, you will need the following:

C compiler (GCC or Clang)
pthread library
A remote service to test against (e.g., SSH server)

## Compilation



To compile the tool, run the following command:


```
$ make
```

This will create an executable file called bruteforce.

## Usage


The tool takes the following arguments:


```
$ ./sudoku <filename>
```

The Sudoku puzzle file should be formatted as follows:

- The first line contains the dimensions of the grid (NxN) as `N x N` (e.g., `9 x 9`).
- The second line contains the dimensions of the subgrids (AxB) as `A x B` (e.g., `3 x 3`).
- The remaining lines contain the Sudoku puzzle, with each row separated by a newline and each number separated by a space.

Example of a valid Sudoku puzzle file:

```
9 x 9
3 x 3
5 3 4 6 7 8 9 1 2
6 7 2 1 9 5 3 4 8
1 9 8 3 4 2 5 6 7
8 5 9 7 6 1 4 2 3
4 2 6 8 5 3 7 9 1
7 1 3 9 2 4 8 5 6
9 6 1 5 3 7 2 8 4
2 8 7 4 1 9 6 3 5
3 4 5 2 8 6 1 7 9
```

After running the tool, it will create an output file called sudoku_aham.out containing either `SUCCESS` or `FAIL`, depending on whether the Sudoku puzzle is valid or not.

## How Threads are Created and Managed

The program creates a number of threads based on the number of available processors in the system:

```
int numThreads = 12;
pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
```

Each thread is assigned a specific range of tasks to execute:

```
double cellsPerThread = (double)sudoku.size / numThreads;
int tasks = (int)ceil(cellsPerThread);
```

A datastruct is created for each thread, containing the Sudoku puzzle and the range of tasks to be executed by that thread. The pthread_create() function is then used to create and run the threads:

```
pthread_create(&threads[i], NULL, threadFunc, (void *)&threadsInit[i]);
```

### Thread Functions

Each thread executes the `threadFunc()` function, which in turn calls three different functions to verify rows, columns, and subgrids:

```
verifyRow(&task);
verifyColumn(&task);
verifySubGrade(&task);
```

These functions use the datastruct passed to them to determine which parts of the Sudoku puzzle to validate. If a validation fails, the global variable globalResult is incremented using the atomic __sync_fetch_and_add() function, ensuring that there are no race conditions between threads updating the global result:

`
__sync_fetch_and_add(&globalResult, 1);
`

### Synchronization and Cleanup

After all threads have been created, the program uses pthread_join() to wait for each thread to complete its execution:

```
for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
}
```

Finally, the memory allocated for the threads and their data structures is released:

```
free(threads);
free(threadsInit);
```

In conclusion, the Sudoku Validator program effectively uses multithreading to efficiently validate the input Sudoku puzzle. This design allows the program to take advantage of multiple processors, resulting in faster validation times.


## License

This project is licensed under the MIT License. For more information, see [MIT License](https://opensource.org/licenses/MIT).
