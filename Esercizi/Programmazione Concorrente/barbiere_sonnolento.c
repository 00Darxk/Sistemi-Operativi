#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include"queue.h"

#define MAX_WAIT_SIZE (5)
#define MAX_HAIRCUT_TIME (1e6)	        // in usec
#define MAX_ARRIVAL_TIME (2e6)	        // in usec
#define MAX_SLEEP (MAX_ARRIVAL_TIME/2)	// in usec
#define TEST_TIME (3e7)		            // in usec

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int cid;
Queue* queue;
int waitSize;
int exitCondition;

void init(){
	if(queue != NULL) { free(queue); }
	queue = (Queue*) malloc(sizeof(Queue));
	initializeQueue(queue);
	waitSize = 0;
	cid = 0;
	exitCondition = 0;
}

void error(char* s){
    perror(s);
    exit(EXIT_FAILURE);
}


struct timeval getTime(){
    struct timeval* arrival = (struct timeval*) malloc(sizeof(struct timeval));
    if(gettimeofday(arrival, NULL) < 0) { error("gettimeofday()"); }
    return *arrival;
}


// each process correspond to a new client in the queue,
Process* newCustomer(){
    Process* c = (Process*) malloc(sizeof(Process));

    c->id = ++cid;
    c->exec_time = random()%(long)MAX_HAIRCUT_TIME;

    c->arrival = getTime();
    return c;
}


void* barber(){
	Process* currentCustomer = NULL;
	while(!exitCondition){
		pthread_mutex_lock(&lock);
		if(waitSize == 0){
			pthread_mutex_unlock(&lock);
			printf("[Barber]: No Customer(s)... Sleeping!\n");
			usleep(random()%(int)MAX_SLEEP);
			continue;
		}
		currentCustomer = dequeue(queue);
		waitSize--;
		pthread_mutex_unlock(&lock);
		printf("[Barbar]: Customer(s)... Waking Up\n");
		currentCustomer->start = getTime();
		printf("[Barber]:   Serving Customer (%ld). Starting Haircut Time: %s", currentCustomer->id, asctime(localtime(&(currentCustomer->start.tv_sec))));

		usleep((int)currentCustomer->exec_time);

		currentCustomer->end = getTime();
		printf("[Barber]:    Served Customer (%ld). Finished Haircut Time: %s", currentCustomer->id, asctime(localtime(&(currentCustomer->start.tv_sec))));

		free(currentCustomer);
	}
	return NULL;
}

void* customer(){
	Process* customer = NULL;
	while(!exitCondition){
		usleep(random()%(int)MAX_ARRIVAL_TIME);
		pthread_mutex_lock(&lock);
		if(waitSize >= MAX_WAIT_SIZE){
			pthread_mutex_unlock(&lock);
			printf("[Customer]: Queue Full, Can't Enter\n");
			continue;
		}
		pthread_mutex_unlock(&lock);
		customer = newCustomer();

		printf("[Customer]: Entered Customer (%ld), Wakey Wakey! Haircut Time (%.2f [sec]) and Arrival Time %s", customer->id, customer->exec_time/1e6, asctime(localtime(&(customer->arrival.tv_sec))));

		pthread_mutex_lock(&lock);
		enqueue(queue, customer);
		waitSize++;
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}


int main(){
	init();
	pthread_t barb, cust;

	printf("\n\n[Main]: Opening Barbershop for The Day\n\n");

	pthread_create(&barb, NULL, barber, NULL);
	pthread_create(&cust, NULL, customer, NULL);


	usleep((int)TEST_TIME);

	printf("\n\n[Main]: Closing Barbershop\n\n");
	exitCondition = 1;

	pthread_join(barb, NULL);
	pthread_join(cust, NULL);

	printf("\n\n[Main]: Closed. Time to Sleep for The Night\n");

	pthread_mutex_destroy(&lock);
}
