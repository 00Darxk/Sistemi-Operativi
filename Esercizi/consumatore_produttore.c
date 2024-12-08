#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define SIZE 10

typedef struct Stack{
    int* a;    // array
    int size;  // dimensione dell'array
    int used;  // posizione occupate nell'array
}stack;

stack pila;

void init(){
    pila.a = (int*) calloc(SIZE, sizeof(int));
    pila.size = SIZE;
    pila.used = 0;
}

void push(int v){
    if (pila.used < pila.size){
        pila.a[pila.used] = v;
        pila.used++;
    }
}

int pop(){
    int val = -1;
    if (pila.used > 0){
        val = pila.a[pila.used-1];
        pila.used--;
    }
    return val;
}

void* produttore(void* arg){
    while(1){
        usleep(random()%(int)1e6);
        pthread_mutex_lock(&pila);
        if(pila.used <= pila.size){
                int aggiunte = random()%(pila.size - pila.used);
                for( int i = 0; i < aggiunte; i++ ){
                    int add = random();
                    push(random());
                    printf("[Produttore]: Inserito %d\n", add);
            }
        }
        pthread_mutex_unlock(&pila);
    }

}

void* consumatore(void* arg){
    while(1){
        usleep(random()%(int)1e6);
        pthread_mutex_lock(&pila);
        if(pila.used < pila.size){
                int rimozioni = random()%pila.used;
                for( int i = 0; i < rimozioni; i++ ){
                    printf("[Consumatore]: Rimosso %d\n", pop());
            }
        }
        pthread_mutex_unlock(&pila);
    }
}


int main(){
    init();
    pthread_t cons, prod;

    pthread_create(&cons, NULL, consumatore, NULL);
    pthread_create(&prod, NULL, produttore, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
}