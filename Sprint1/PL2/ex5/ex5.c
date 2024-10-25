#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_SIZE 256

void upper(char *str);

int main(void){
	int fd1[2], fd2[2];
	pid_t p;
	
	/* cria o pipe */
	if(pipe(fd1) == -1){
		perror("Pipe 1 failed");
		return 1;
	}
	
	if(pipe(fd2) == -1){
		perror("Pipe 2 failed");
		return 1;
	}
	
	p = fork();
	
	if(p>0){
		close(fd1[0]);
		close(fd2[1]);
		
		char msg[MAX_SIZE];
		fgets(msg, MAX_SIZE, stdin); 
	
		write(fd1[1], &msg, MAX_SIZE);
	
		close(fd1[1]);
		
		read(fd2[0], &msg, MAX_SIZE);
		
		close(fd2[0]);
		
		printf("Converted string: %s\n", msg);
	
	}else{
	
		close(fd1[1]);
		close(fd2[0]);
	
		char msg[MAX_SIZE];
		printf("Introduza a frase:\n");
		read(fd1[0], &msg, MAX_SIZE);
		
		close(fd1[0]);
		
		upper(msg);
		write(fd2[1], &msg, MAX_SIZE);
		
		close(fd2[1]);
	}
	
	return 0;
}

void upper(char *str) {
	int i;
	for(i = 0; i < strlen(str); i++) {
		if(str[i] > 96 && str[i] < 123)
			str[i] -= 32;
	}
}
