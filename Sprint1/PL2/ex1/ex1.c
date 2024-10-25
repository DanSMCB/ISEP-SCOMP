#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFFER_SIZE 80

int main(void){
	char read_msg[BUFFER_SIZE];
	char write_msg[BUFFER_SIZE];
	int pid= getpid();
	snprintf(write_msg,BUFFER_SIZE,"Pid=%d\n",pid);
	int fd[2];
	pid_t p;
	
	/* cria o pipe */
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	p = fork();
	
	if(p > 0){
		close(fd[0]);
	
		printf("(Parent process) %s", write_msg);
		write(fd[1],write_msg,strlen(write_msg)+1);
	
		close(fd[1]);
	}else{
	
		close(fd[1]);
	
		read(fd[0], read_msg, BUFFER_SIZE);
		printf("(Child process) %s", read_msg);
	
		close(fd[0]);
	}
	
	return EXIT_SUCCESS;
}
