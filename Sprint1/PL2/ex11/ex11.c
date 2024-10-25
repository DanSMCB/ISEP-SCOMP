#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void){
	int fd[6][2];
	pid_t p;
	int i, s;
	time_t t;
    srand((unsigned) time(&t));
	
	for(i=0; i<6; i++){
		if(pipe(fd[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
		
	for(i=0; i<5; i++){
		p = fork();
		
		if(p==-1){
			perror("Fork failed");
			return 1;
		}
		if(p==0){
			close(fd[i][1]);
			close(fd[i+1][0]);
	
			int num = rand () % 500 + 1;
			printf("Process pid: %d\nNumber: %d\n\n",getpid(),num);
			int aux;
			while(read(fd[i][0], &aux, sizeof(int))==0){
			}
			
			if(num>aux){
				write(fd[i+1][1], &num, sizeof(int));
			}else write(fd[i+1][1], &aux, sizeof(int));
			
	
			close(fd[i][0]);
			close(fd[i+1][1]);
	
			exit(0);
		}
	}
	
	if(p>0){
		close(fd[0][0]);
		close(fd[i+1][1]);
	
		int num = rand() % 500;
		printf("Process pid: %d\nNumber: %d\n\n",getpid(),num);
		write(fd[0][1], &num, sizeof(int));
		
		close(fd[0][1]);
		
		while(wait(&s)>0){
		}
		
		int greatest;
		read(fd[i+1][0], &greatest, sizeof(int));
		
		close(fd[i+1][0]);
		
		printf("Greatest number: %d\n", greatest);
	
	}
	
	return 0;
}
