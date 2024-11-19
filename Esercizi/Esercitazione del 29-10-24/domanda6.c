#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/*
	Write a slight modification of the previous program, this time using waitpid() instead of wait(). When would waitpid() be
    useful?
*/

int main(){
	int rc = fork();
	if(rc == 0){ // child
		printf("Hello\n");

	} else {
		waitpid(rc, NULL, 0);
		printf("Goodbye\n");
	}
}
