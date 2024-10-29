#include <stdio.h>
#include <unistd.h>

/* 
    Write a program that calls fork(). Before calling fork(), have the main process access a variable, and set its value to something. What
    value is the variabile in the child process? What happens to the variabile when both the child and parent change the value of x?
*/

int main() {
	int x = 0;
	x = 100;
	int rc = fork();
	x++;
	printf("Viene incrementato di uno: %d\n", x);
}
