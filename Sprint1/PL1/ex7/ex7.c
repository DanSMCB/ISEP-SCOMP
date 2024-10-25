#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#define ARRAY_SIZE 1000
	
int main (){

	int numbers[ARRAY_SIZE]; /* array to lookup */
	int n; /* the number to find */
	time_t t; /* needed to init. the random number generator (RNG)*/
	int i, total=0;
	int s;

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	/* initialize array with random numbers (rand(): stdlib.h) */
	for (i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand () % 10000;
	}
	
	/* initialize n */
	n = rand () % 10000;
	
	pid_t p = fork();
	
	if(p==0){
		for(i=ARRAY_SIZE/2; i<ARRAY_SIZE; i++){
			if(numbers[i]==n) total++; 
		}
		printf("Total of n found in the child process: %d\n",total);
		exit(total);
	}else{
		for(i=0; i<ARRAY_SIZE/2; i++){
			if(numbers[i]==n) total++; 
		}
		printf("Total of n found in the parent process: %d\n",total);
		waitpid(p, &s, 0);
		total=total+WEXITSTATUS(s);
		printf("Total of n found in the entire array: %d\n",total);
	}
	return EXIT_SUCCESS;
}
