#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define MATRIX_SIZE 2000
    int a[MATRIX_SIZE][MATRIX_SIZE];
    int b[MATRIX_SIZE][MATRIX_SIZE];
    int result[MATRIX_SIZE][MATRIX_SIZE];

void fill_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]){
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            matrix[i][j] = rand() % 100;
}

void multiply_matrices_single_threaded( int a[MATRIX_SIZE][MATRIX_SIZE], 
                                        int b[MATRIX_SIZE][MATRIX_SIZE]){
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++){
            result[i][j] = 0;
            for(int k = 0; k < MATRIX_SIZE; k++)
                result[i][j] += a[i][k] * b[k][j]
        }
}

int main(){
    a = fill_matrix();
    b = fill_matrix();

    struct time_val start, end;

    gettimeofday(&start, NULL);
    multiply_matrices_single_threaded(a, b);
    gettimeofday(&end, NULL);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    printf("Time taken: (single-threaded) %f seconds\n", time_taken);
    
    return 0;
}