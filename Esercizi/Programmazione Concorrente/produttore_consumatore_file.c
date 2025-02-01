#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>

#define MAX_DIM (10)
#define NUM_DIM (100)
#define TEST_TIME (1e7) // in usec

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int exitCondition;
int numPushes;
int numPops;

void error(char* s){
    fprintf(stderr, "%s", s);
    exit(EXIT_FAILURE);
}

void init(){
    // initialize global variables
    exitCondition = 0;
    numPops = 0;
    numPushes = 0;

    // initialize the file
    int fd = open("a.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if(fd < -1) { error("[Init]: Error in open()\n"); }
    int dim = 0;

    if(write(fd, &dim, sizeof(int)) < -1) { error("[Init]: Error in write()\n"); }
    close(fd);
    srand(time(0));
}

int getDim(){
    int fd = open("a.txt", O_RDONLY | O_CREAT, S_IRWXU);
    if(fd < -1) { error("[GetDim]: Error in open()\n"); }

    int dim = 0;
    if(read(fd, &dim, sizeof(int)) < -1) { error("[GetDim]: Error in read()\n"); }

    close(fd);
    return dim;
}

void push(int k){
    int dim = getDim() + 1;
    int fd = open("a.txt", O_WRONLY);
    if(fd < -1) { error("[Push]: Error in open()\n"); }

    // updates the dimension
    if(write(fd, &dim, sizeof(int)) < -1) { error("[Push]: Error in write()\n"); }

    // writes the new element
    lseek(fd, dim * sizeof(int), SEEK_SET);
    if(write(fd, &k, sizeof(int)) < -1) { error("[Push]: Error in write()\n"); }

    numPushes++;
    close(fd);
}

int pop(){
    int dim = getDim() - 1;
    int fd = open("a.txt", O_RDWR);
    if(fd < -1) { error("[Pop]: Error in open()\n"); }

    // updates the dimension
    if(write(fd, &dim, sizeof(int)) < -1) { error("[Pop]: Error in write()\n"); }


    // reads the last element
    int k;
    lseek(fd, (dim + 1) * sizeof(int), SEEK_SET);
    if(read(fd, &k, sizeof(int)) < -1) { error("[Pop]: Error in read()\n"); }

    // sets the last element to zero, "removing" it
    int zero = 0;
    lseek(fd, (dim + 1) * sizeof(int), SEEK_SET);
    if(write(fd, &zero, sizeof(int)) < -1) { error("[Pop]: Error in write()\n"); }

    numPops++;
    close(fd);
    return k;
}


void printStack(){
    int fd = open("a.txt", O_RDONLY | O_CREAT, S_IRWXU);
    if(fd < -1) { error("[PrintStack]: Error in open()\n"); }
    int dim = 0;
    if(read(fd, &dim, sizeof(int)) < -1) { error("[PrintStack]: Error in read()\n"); }
    printf("[PrintStack]: (%d) Item(s):", dim);
    for(int i = 0; i < dim; i++){
        int temp = 0;
        if(read(fd, &temp, sizeof(int)) < -1) { error("[PrintStack]: Error in read()\n"); }

        printf("[%d] ", temp);
        if(i == dim - 1) { printf("\n"); }
    }
    if(dim == 0) { printf("[]\n"); }
    close(fd);
}

void* producer(){
    while(!exitCondition){
        usleep((int)random()%(int)1e6);
        pthread_mutex_lock(&lock);
        int dim = getDim();
        int pushDim = random()%(MAX_DIM - dim + 1);
        printf("[Producer]: (%d) Insertion(s): ", pushDim);

        if(pushDim == 0) { printf("[]"); }
        for(int i = 0; i < pushDim; i++) {
            if(i == 0) { printf("[Push]: "); }
            int temp = random()%NUM_DIM;
            printf("[%d] ", temp);
            push(temp);
        }
        printf("\n[Producer]: ");
        printStack();
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* consumer(){
    while(!exitCondition){
        usleep((int)random()%(int)1e6);
        pthread_mutex_lock(&lock);
        int dim = getDim();
        int popDim = random()%(dim + 1);

        printf("[Consumer]: (%d) Deletion(s):  ", popDim);
        for(int i = 0; i < popDim; i++) {
            if(i == 0) { printf("[Pop]:  "); }
            printf("[%d] ", pop());
        }
        printf("\n[Consumer]: ");
        printStack();
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(){
    init();
    printf("[Main]: Starting Test (%.1f [sec])\n\n", TEST_TIME/1e6);

    pthread_t prod, cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    usleep((int)TEST_TIME);
    printf("\n\n[Main]: Terminating Test\n\n");
    exitCondition = 1;

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    printf("\n\n[Main]: Test Terminated\n");
    printf("    (%d) Insertion(s)\n    (%d) Deletion(s)\n", numPushes, numPops);

    pthread_mutex_destroy(&lock);
    return 0;
}


