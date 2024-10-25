#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#define ARRAY_SIZE 1000
	
int main (){

	int numbers[ARRAY_SIZE]; /* array to lookup */
	time_t t; /* needed to init. the random number generator (RNG)*/
	pid_t p_array[5];
	int i;
	int status[5];
	int s;
	int max=0;
	int max_total=0;
	int result[ARRAY_SIZE];

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	/* initialize array with random numbers (rand(): stdlib.h) */
	for (i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand () % 255;
	}
	
	for (i = 0; i < 5; i++){
		p_array[i]=fork();
	
		if(p_array[i]<0){
			perror("erro na criação do processo");
			exit(-1);
		} 
		
		if(p_array[i]==0) break;
	}
	
	if(p_array[i]==0){
		for(int j=i*200; j<i*200+200; j++){
			if(numbers[j]>max) max=numbers[j]; 
		}
		exit(max);
	}else{
		for(i=0; i<5; i++){
			wait(&status[i]);
			printf("%d\n",WEXITSTATUS(status[i]));
			if(WEXITSTATUS(status[i])>max_total) max_total=WEXITSTATUS(status[i]);
		}
		printf("%d\n",max_total);
		
		pid_t p = fork();
		
		if(p==0){
			for(i=0; i<ARRAY_SIZE/2; i++){
				result[i]==(numbers[i]/max_total)*100;
				printf("%d\n",result[i]);
			}
		}else{
			waitpid(p,&s,0);
			for(i=ARRAY_SIZE/2; i<ARRAY_SIZE; i++){
				result[i]==( numbers[i]/max_total)*100;
				printf("%d\n",result[i]);
			}
		}
		
	}
}
