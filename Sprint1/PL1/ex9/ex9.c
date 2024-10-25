#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#define ARRAY_SIZE 10
	
int main (){

	pid_t p_array[ARRAY_SIZE];
	int status[ARRAY_SIZE];
	int i;
	int* s;


	for (i = 0; i < ARRAY_SIZE; i++){
		p_array[i]=fork();
	
		if(p_array[i]<0){
			perror("erro na criação do processo");
			exit(-1);
		} 
		
		if(p_array[i]==0) break;
	}
	
	if(p_array[i]==0){
		for(int j=i*100+1; j<i*100+100; j++){
				printf("%d\n",j);
			}
	}else{
		for(i=0; i<10; i++) wait(&status[i]);
	}
}
