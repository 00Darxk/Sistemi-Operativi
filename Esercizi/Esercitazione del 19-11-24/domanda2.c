#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DEFAULT_DIM 4

// Esercizio 2: Gestione di vettori dinamici:
/**
 * Implementa una struttura dati per un vettore dinamico di interi. Includi funzioni per aggiungere un 
 * elemento, rimuovere un elemento e stampare tutti gli elementi del vettore. Assicurati che il vettore 
 * aumenti la sua capacità quando necessario.
 */

typedef struct Vect{
	int* v;
	int l;
	int lmax;
}Vect;


// crea una struttura di tipo Vect
Vect* newVect(int lmax){
	int* newArray = malloc(lmax * sizeof(int));
	Vect* newVect = malloc(sizeof(Vect));
	newVect->v = newArray;
	newVect->l = 0;
	newVect->lmax = lmax;

	return newVect;
}


// aggiunge un elemento in coda al vettore vect
void addVect(Vect* vect, int x){
	if(vect->l >= vect->lmax){
		vect->v = realloc(vect->v, vect->lmax * 2);
		vect->lmax *= 2;
	}
	vect->v[vect->l] = x;
	vect->l += 1;
}

// rimuovere un elemento dato il suo indice
void removeVect(Vect* vect, int i){
	if(vect->l <= i) printf("\nIndex Overflow\n");
	else if(i < 0)   printf("\nIndex Underflow\n");
	else{ 
        for (int j = i; j < vect->l-1; j++)
            vect->v[j] = vect->v[j + 1];
        vect->l--;
    }
}

// stampa tutti gli elementi del vettore
void printVect(Vect* vect){
    printArray(vect->v, vect->l);
}

int printArray(int *array, int x){
	for (int i = 0; i < x; i++){
		if(i == 0) printf("[%d, ", array[i]);
		else if (i == x - 1) printf("%d]\n", array[i]);
		else printf("%d, ", array[i]);
	}
}

int main(){
	srand(time(NULL));
	Vect* v = newVect(DEFAULT_DIM);
	int dim = 0;
        printf("Specifica la dimensione dell'array: \n");
	scanf("%d%*c", &dim);
	for (int i = 0; i < dim; i++)
		addVect(v, rand()%dim);
	printVect(v);
	int del = -1;
	printf("Specificare l'indice dell'elemento da rimuovere: \n");
	scanf("%d%*c", &del);
	removeVect(v, del);
	printVect(v);
	free(v);
}


