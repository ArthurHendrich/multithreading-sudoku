CC=gcc
CFLAGS=-Wall -Wextra -pedantic

sudoku: sudoku.c aux.c aux.h
	$(CC) $(CFLAGS) -o sudoku sudoku.c aux.c -pthread -lm -std=c11