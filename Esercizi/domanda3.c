#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*
	Write another program using fork(). The child process should
	print “hello”; the parent process should print “goodbye”. 
*/
int main(){
	int rc = fork();
	if(rc == 0){ // child
		printf("Goodbye\n");
	} else { // parent
		pritnf("Hello\n");
	}
}
