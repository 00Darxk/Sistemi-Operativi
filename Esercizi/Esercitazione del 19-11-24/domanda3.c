#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Esercizio 3: Manipolazione della memoria:
/**
 * Scrivi un programma che alloca memoria per un array di interi, usa 
 * memset per impostare tutti gli elementi a zero, quindi usa memcpy per 
 * copiare il contenuto di un altro array nella memoria appena allocata. 
 */

void printArray(int* a, int dim){
	for(int i = 0; i < dim; i++){
		if(i == 0) printf("[%d, ", a[i]);
		else if(i == dim - 1) printf("%d]\n", a[i]);
		else printf("%d, ", a[i]);
	}
}

int* randArray(int dim){
	srand(time(NULL));
	int* array = malloc(dim * sizeof(int));
	for(int i = 0; i < dim; i++)
		array[i] = random()%dim;
	return array;
}


int main(){
	int dim = 0;
    printf("Specifica la dimensione dell'array: \n");
	scanf("%d%*c", &dim);
	int* array = malloc(dim * sizeof(int));
	array = memset(array, 0, dim * sizeof(int));
	printArray(array, dim);
	memcpy(array, randArray(dim), dim * sizeof(int));
	printArray(array, dim);
}
