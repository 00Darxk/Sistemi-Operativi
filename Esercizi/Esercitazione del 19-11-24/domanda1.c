#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
	srand(time(NULL));
	int x = 0;
	printf("Specificare la dimensione: \n");
	scanf("%d%*c", &x);

	int* array = malloc(x*sizeof(int));
	for (int i = 0; i < x; i++){
		array[i] = rand()%x;
	}
	for (int i = 0; i < x; i++){
		if(i == 0)
			printf("[%d, ", array[i]);
		else if (i == x - 1)
			printf("%d]\n", array[i]);
		else
			printf("%d, ", array[i]);
	}
	free(array);
}
