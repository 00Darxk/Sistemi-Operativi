#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Esercizio 1: Allocazione di memoria dinamica
/**
 * Scrivi un programma che crea un array dinamico di numeri interi di dimensione specificata dall'utente, 
 * riempie l'array con numeri casuali e stampa l'array. Assicurati di liberare la memoria allocata alla 
 * fine del programma.
 */

int printArray(int *array, int x){
	for (int i = 0; i < x; i++){
		if(i == 0) printf("[%d, ", array[i]);
		else if (i == x - 1) printf("%d]\n", array[i]);
		else printf("%d, ", array[i]);
	}
}

int main(){
	srand(time(NULL));
	int x = 0;
	printf("Specificare la dimensione: \n");
	scanf("%d%*c", &x);

	int* array = malloc(x*sizeof(int));
	for (int i = 0; i < x; i++)
		array[i] = rand()%x;
	
    printArray(array, x);
	free(array);
}
