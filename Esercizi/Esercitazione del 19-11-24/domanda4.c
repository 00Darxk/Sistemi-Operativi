#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

void printArray(int* a, int dim){
	for (int i = 0; i < dim; i++){
		if(i == 0) printf("[%d, ", a[i]);
		else if(i == dim - 1) printf("%d]\n", a[i]);
		else printf("%d, ", a[i]);
	}
}

// TODO finire esercizio

int main(){
	int dim = 15;
	int* array = (int*) mmap(NULL, dim * sizeof(int) );
	

	munmap(array, dim * sizeof(int));
}
