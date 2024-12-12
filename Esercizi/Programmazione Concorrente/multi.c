#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>

#define MATRIX_SIZE 2000
#define NUM_THREADS 4
    int a[MATRIX_SIZE][MATRIX_SIZE];
    int b[MATRIX_SIZE][MATRIX_SIZE];
    int result[MATRIX_SIZE][MATRIX_SIZE];

typedef struct {
    int (*a)[MATRIX_SIZE];
    int (*b)[MATRIX_SIZE];
    int (*result)[MATRIX_SIZE];
    int start_row;
    int end_row;
}ThreadData;

void fill_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]){
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            matrix[i][j] = rand() % 100;
}

void* multiply_matrices_multi_threaded(void *arg){
    ThreadData *data = (ThreadData*) arg;
    for (int i = data->start_row; i < data->end_row; i++)
        for (int j = data->start_row; j < data->end_row; j++){
            data->result[i][j] = 0;
            for(int k = data->start_row; k < data->end_row; k++)
                data->result[i][j] += data->a[i][k] * data->b[k][j]
        }
    return NULL;
}

int main(){
    a = fill_matrix();
    b = fill_matrix();

    pthread_t threads[NUM_THREADS];
    ThreadData *thread_data[NUM_THREADS];
    int rows_per_thread = MATRIX_SIZE/NUM_THREADS;

    struct time_val start, end;

    gettimeofday(&start, NULL);

    for(int i = 0; i < NUM_THREADS; i++){
        thread_data[i]->a = a;
        thread_data[i]->b = b;
        thread_data[i]->result = result;
        thread_data[i]->start_row = i * rows_per_thread;
        thread_data[i]->end_row = (i + 1) * rows_per_thread;
        if (i == NUM_THREADS - 1)
            thread_data[i]->start_row += MATRIX_SIZE % NUM_THREADS;
        
        pthread_create(&threads[i], NULL, multiply_matrices_multi_threaded, thread_data[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
        
    gettimeofday(&end, NULL);

    double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    printf("Time taken: (multi-threaded) %f seconds\n", time_taken);
    
    return 0;
}