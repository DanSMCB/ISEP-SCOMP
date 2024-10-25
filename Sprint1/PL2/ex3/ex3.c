#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFFER_SIZE 80

int main(void){
	char read_msg1[BUFFER_SIZE];
	char read_msg2[BUFFER_SIZE];
	char write_msg1[]="Hello World!\n";
	char write_msg2[]="Goodbye!\n";
	int fd[2];
	pid_t p;
	int s;
	
	/* cria o pipe */
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	p = fork();
	
	if(p>0){
		close(fd[0]);
	
		write(fd[1],write_msg1,strlen(write_msg1)+1);
		write(fd[1],write_msg2,strlen(write_msg2)+1);
	
		close(fd[1]);
	
		wait(&s);
	
		printf("Child pid: %d\nExit status: %d\n", p,WEXITSTATUS(s));
	
	}else{
	
		close(fd[1]);
	
		while(read(fd[0], read_msg1, strlen(write_msg1)+1)==0){
		}
		printf("%s", read_msg1);
	
		while(read(fd[0], read_msg2, strlen(write_msg2)+1)==0){
		}
		printf("%s", read_msg2);
	
		close(fd[0]);
	
		exit(strlen(read_msg2)+strlen(read_msg1));
	}
	
	return EXIT_SUCCESS;
}
