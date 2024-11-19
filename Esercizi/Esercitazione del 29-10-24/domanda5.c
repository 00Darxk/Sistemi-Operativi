#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/*
    Now write a program that uses wait() to wait for the child process
    to finish in the parent. What does wait() return? What happens if
    you use wait() in the child?
*/

int main(){
	int rc = fork();
	if(rc == 0){ // child
		printf("Hello\n");
	} else {
		wait(NULL);
		printf("Goodbye\n");
	}
}
