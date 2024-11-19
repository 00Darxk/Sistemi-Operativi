#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct vettore{
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
	if(vect->l <= i){
		printf("\nIndex Overflow\n");
		return;
	}else if(i < 0){
		printf("\nIndex Underflow\n");
		return;
	}
	for (int j = i; j < vect->l-1; j++)
		vect->v[j] = vect->v[j + 1];
}

// stampa tutti gli elementi del vettore
void printVect(Vect* vect){
	for(int i = 0; i < vect->l; i++){
		if(i == 0)
			printf("[%d, ", vect->v[i]);
		else if(i == vect->l - 1)
			printf("%d]\n", vect->v[i]);
		else
			printf("%d, ", vect->v[i]);
		}
}

int main(){
	srand(time(NULL));
	int dim = 0;
	printf("Specificare la dimensione del vettore: \n");
	scanf("%d%*c", &dim);
	Vect* v = newVect(dim);
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


