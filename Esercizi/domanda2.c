#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
	Write a program that opens a file (with the open() system call)
	and then calls fork() to create a new process. Can both the child
	and parent access the file descriptor returned by open()? What
	happens when they are writing to the file concurrently, i.e., at the
	same time?
*/

int main(){
	int fd = open("domanda2.txt", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
	int rc = fork();
	if(rc == 0){ // child
		char* txt = "Ciao, sono il figlio\n";
		write(fd, txt, strlen(txt));

	} else { // parent
		char* txt = "Ciao, sono il padre\n";
		write(fd, txt, strlen(txt));
	}
	close(fd);
}
