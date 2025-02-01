#include<stdio.h>
#include"queue.h"
#include<unistd.h>
#include<pthread.h>

#define ARRIVAL_RATE (10)       // per second
#define MAX_EXEC_TIME (1e6)     // in usec
#define MAX_QUEUE_SIZE (1e3)    // in usec
#define TEST_TIME (1e7)         // in usec

typedef struct {
    float time;
    float jobFrequency;
    int CPUs;
}Result;

float avgTTime;
int procNum;

int exitCondition;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
Queue* queue = NULL;
int size;
int pid;
int err;
int jobFrequency;

void init(){
    avgTTime = 0;
    procNum = 0;
    if(queue != NULL) { free(queue); }
    queue = (Queue*)malloc(sizeof(Queue));
    initializeQueue(queue);
    size = 0;
    pid = 0;
    err = 0;
    exitCondition = 0;
    jobFrequency = 1;
    srand(time(0));
}

void error(char* s){
    perror(s);
    exit(EXIT_FAILURE);
}


struct timeval getTime(){
    struct timeval* arrival = (struct timeval*) malloc(sizeof(struct timeval));
    err = gettimeofday(arrival, NULL);
    if(err < 0) { error("gettimeofday()"); }
    return *arrival;
}

Process* newProcess(){
    Process* process = (Process*) malloc(sizeof(Process));

    process->id = ++pid;
    process->exec_time = random()%(long)MAX_EXEC_TIME;

    process->arrival = getTime();
    printf("[Job]    [newProcess]: Created Process PID (%d), Execution Time (%.2f [sec]) and Arrival Time %s", pid, process->exec_time/1e6, asctime(localtime(&(process->arrival.tv_sec))));
    return process;
}

long turnaroundTime(Process* process){
    long arrival = (long)(process->arrival.tv_sec * 1e6) + process->arrival.tv_usec;
    long end = (long)(process->end.tv_sec * 1e6 ) + process->end.tv_usec;
    return end - arrival;
}

void updateTTime(Process* process){
    float TTime = turnaroundTime(process);
    float tempProcNum;
    float tempAvgTTime;

    pthread_mutex_lock(&lock);
    tempProcNum = ++procNum;
    avgTTime = (float)(((avgTTime * (procNum - 1)) + TTime)/procNum);
    tempAvgTTime = avgTTime;
    pthread_mutex_unlock(&lock);

    printf("[CPU]:		[updateTTime]: Process PID (%ld), Turnaround Time (%.2f [sec]), Updated Average Turnaround Time (%.2f [sec]). (%d) Process(es) Counted\n", process->id, TTime/1e6, tempAvgTTime/1e6, (int)tempProcNum);
}


void* job(){
    while(!exitCondition){
        usleep((int)1e5/jobFrequency);
        pthread_mutex_lock(&lock);
        if(size >= MAX_QUEUE_SIZE){
            pthread_mutex_unlock(&lock);
            continue;
        }
        pthread_mutex_unlock(&lock);

        Process* p = newProcess();
        int tempSize = 0;

        pthread_mutex_lock(&lock);
        tempSize = ++size;
        enqueue(queue, p);
        pthread_mutex_unlock(&lock);

        printf("[Job]: Added New Process in Queue with PID (%d), Queue Size (%d)\n", pid, tempSize);
    }
    return NULL;
}

void* cpu(){
    while(!exitCondition){
        int tempSize = 0;
        pthread_mutex_lock(&lock);
        if(size <= 0){
            pthread_mutex_unlock(&lock);
            continue;
        }
        Process* exec = dequeue(queue);
        tempSize = --size;
        pthread_mutex_unlock(&lock);

        // assigns the process start time
        exec->start = getTime();
        printf("[CPU]: Removed Process from Queue with PID (%ld), Start Execution Time %s", exec->id, asctime(localtime(&(exec->start.tv_sec))));
        // waits for the process execution
        usleep((long)exec->exec_time);

        // assigns the process end time
        exec->end = getTime();
        printf("[CPU]:   Finished Execution of Process PID (%ld),   End Execution Time %s", exec->id, asctime(localtime(&(exec->end.tv_sec))));

        // updates the turnaround time (mutex locking inside the function)
        updateTTime(exec);

        free(exec);
    }
    return NULL;
}

void swap(Result* array, int i, int j){
    if(i == j) return;
    Result temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int min(Result* array, int dim){
    int iMin = 0;
    for(int i = 1; i < dim; i++) { if(array[iMin].time > array[i].time) iMin = i; }
    return iMin;
}

void order(Result* array, int dim){
    for(int i = 0; i < dim - 1; i++){ swap(array, min(array + i, dim - i) + i, i); }
}

void printOrdered(Result* result, int dim){
    if(dim < 0) return;
    order(result, dim);
    for(int k = 0; k < dim; k++) { printf("  [%d](%.2f [sec]): CPUs (%d), Job Frequency (%.1f/[sec])\n", k + 1, result[k].time, result[k].CPUs, result[k].jobFrequency); }
}


int main(){
    int jobProd = 0;
    int numCons = 0;
    Result* result = (Result*) malloc(sizeof(Result)*9);
    for(int k = 0; k < 9; k++){
        init();
        if(k%3 == 0) {
            jobProd = 0;
            numCons++;
            if(k == 6) { numCons++; }
        }
        jobProd++;
        jobFrequency = jobProd;

        printf("\n\n[Main]: Starting (Next) Test. CPUs (%d), Job Frequency (%.1f/[sec]). Test Time (%.1f [sec])\n\n", numCons, (float) ARRIVAL_RATE * jobProd, TEST_TIME/1e6);


        pthread_t prod;
        pthread_t* cons = (pthread_t*) malloc(sizeof(pthread_t) * numCons);

        pthread_create(&prod, NULL, job, NULL);
        for(int j = 0; j < numCons; j++) { pthread_create(&(cons[j]), NULL, cpu, NULL); }

        usleep((int)TEST_TIME);
        printf("\n\n[Main]: Terminating Current Test\n\n");

        exitCondition = 1;

        pthread_join(prod, NULL);
        for(int j = 0; j < numCons; j++) { pthread_join(cons[j], NULL); }
        printf("\n\n[Main]: Test Terminated\n");

        result[k].time = avgTTime/1e6;
        result[k].jobFrequency = ARRIVAL_RATE * jobProd;
        result[k].CPUs = numCons;

        printf("[Main]: Average Turnaround Time (%.2f [sec]). CPUs (%d), Job Frequency (%.1f/[sec])\n\n", avgTTime/1e6, numCons, (float) ARRIVAL_RATE * jobProd);
        free(cons);

        if(k == 8) { break; }

        printf("[Main]: Average Turnaround Time for Conducted Test(s):\n");
        printOrdered(result, k + 1);

        usleep((int)1e6);
    }
    pthread_mutex_destroy(&lock);

    printf("[Main]: Average Turnaround Time for Each Test (%.1f [sec]): \n", TEST_TIME/1e6);
    printOrdered(result, 9);

    return 0;
}



