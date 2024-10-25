#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
	int vec1[1000], vec2[1000], fd[5][2], result[1000];
	pid_t p;
	int s, soma;
	
	for (int i = 0; i < 1000; ++i)	{
		vec1[i] = rand() % 10;
		vec2[i] = rand() % 10;
	}
	
	for(int i=0; i<5; i++){
		if(pipe(fd[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
		
		p = fork();
		
		if(p>0){
		
			wait(&s);
		
			close(fd[i][1]);
	
			for(int j=i*200; j<(i+1)*200; j++){
				read(fd[i][0], &soma, sizeof(int));
				result[j]=soma;
				printf("Total: %d\n", result[j]);
			}
			printf("\n");
	
			close(fd[i][0]);
	
		}else{
			close(fd[i][0]);
	
			for(int j=i*200; j<(i+1)*200; j++){
				int res=vec1[j]+vec2[j];
				write(fd[i][1], &res, sizeof(int));
			}
	
			close(fd[i][1]);
	
			exit(1);
		}
		
	}
	
	
	return 0;
}
