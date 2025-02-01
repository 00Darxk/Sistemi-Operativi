#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/time.h>

#define NUM (10)
#define MAX_THINK (1e6)
#define MAX_EAT (1e6)
#define MAX_WAIT (NUM * (1e6))
#define TEST_TIME (1e7)

pthread_mutex_t* lock;

int* forkUsed;
int exitCondition;

void init(){
	forkUsed = calloc(NUM, sizeof(int));
	lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t) * NUM);
	for(int i = 0; i < NUM; i++) { pthread_mutex_init(lock + i, NULL); }
	exitCondition = 0;
	srand(time(0));
}


void think(useconds_t usec, int id){
	printf("[Philosopher %d]: Started Thinking. Time (%.2f [sec])\n", id, usec/1e6);
	usleep(usec);
	printf("[Philosopher %d]: Stopped Thinking\n", id);
}

void eat(useconds_t usec, int id){
	printf("[Philosopher %d]: Started Eating. Time (%.2f [sec])\n", id, usec/1e6);
	usleep(usec);
	printf("[Philosopher %d]: Stopped Eating\n", id);
}

int isDead(struct timeval* time){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * 1e6 + currentTime.tv_usec - time->tv_sec * 1e6 - time->tv_usec > (int)MAX_WAIT;
}

void resetsTime(struct timeval* time){
	gettimeofday(time, NULL);
}

void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

// every ith-philosopher can access only fork[i - 1] and fork[i%5]
// there are only 5 forks with id 0~4, each corresponding to the jth lock
void* philosopher(void* arg){
	// the id is set before entering the loop
	int id = *((int*) arg);
	// to determine the philosopher next action (1) is eat, (0) is think
	int toEat = 1;
        // int toEat = random()%2; // alternate version
	// sets the waitTime
	struct timeval timeSinceLastAte;
	resetsTime(&timeSinceLastAte);
	while(!exitCondition){
		// check if died
		if(isDead(&timeSinceLastAte)){
			fprintf(stderr, "[Philosopher %d]: Starved to Death...\n", id);
			exit(EXIT_FAILURE);
		}

		if(toEat){
			// first check the lowest ranking fork (the one on their left)
			// circularity -> deadlock
			int leftFork = id - 1;
			int rightFork = id%5;
			if(leftFork > rightFork){ swap(&leftFork, &rightFork); }

			pthread_mutex_lock(lock + leftFork);
			pthread_mutex_lock(lock + rightFork);
			if(forkUsed[leftFork] || forkUsed[rightFork]){
				// the forks are not free, better wait
				pthread_mutex_unlock(lock + rightFork);
				pthread_mutex_unlock(lock + leftFork);
				continue;
			}

			// both of them are free! picks them up
			forkUsed[leftFork] = 1;
			forkUsed[rightFork] = 1;
			pthread_mutex_unlock(lock + rightFork);
			pthread_mutex_unlock(lock + leftFork);

			eat(random()%(int)MAX_EAT, id);

			// ate, put them down
			pthread_mutex_lock(lock + leftFork);
			pthread_mutex_lock(lock + rightFork);
			forkUsed[leftFork] = 0;
			forkUsed[rightFork] = 0;
			pthread_mutex_unlock(lock + leftFork);
			pthread_mutex_unlock(lock + rightFork);

			// if successfully eaten, sets action to (0) to think, and resets the wait time
			toEat = 0;
			resetsTime(&timeSinceLastAte);
		}else{
			think(random()%(int)MAX_THINK, id);
			toEat = 1;
		}
	}
	return NULL;
}

int main(){
	init();

	pthread_t* phis = (pthread_t*) malloc(sizeof(pthread_t) * NUM);
	int ids[NUM];

	// every thread is given an id 1~5
	for(int i = 0; i < NUM; i++) { ids[i] = i + 1; pthread_create(&(phis[i]), NULL, philosopher, &ids[i]); }

	usleep((int)TEST_TIME);
	printf("\n\n[Main]: Terminating the Test\n\n");
	exitCondition = 1;

	for(int i = 0; i < NUM; i++) { pthread_join(phis[i], NULL); }
	printf("\n\n[Main]: Test Terminated\n\n");

	for(int i = 0; i < NUM; i++) { pthread_mutex_destroy(lock + i); }

	return 0;
}
