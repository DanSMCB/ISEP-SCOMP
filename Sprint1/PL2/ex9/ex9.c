#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#define ARRAY_SIZE 50000

typedef struct{
	int customer_code;
	int product_code;
	int quantity;
} sales;


int main(void){
	sales sales[ARRAY_SIZE];
	int products[ARRAY_SIZE];
	int fd[2];
	pid_t p;
	int i, s, k=0, l=0;
	
	for (int i = 0; i < ARRAY_SIZE; i++){
		sales[i].customer_code = rand() % 10;
		sales[i].product_code = i;
		sales[i].quantity = rand() % 25;
	}
	
	for(i=0; i<10; i++){
		if(pipe(fd) == -1){
			perror("Pipe failed");
			return 1;
		}
		
		p = fork();
		
		if(p==-1){
			perror("Fork failed");
			return 1;
		}
		if(p==0){
			close(fd[0]);
	
			for(int j=i*5000; j<(i+1)*5000; j++){
				if(sales[j].quantity>20){
					write(fd[1], &(sales[j].product_code), sizeof(int));
				}
			}
	
			close(fd[1]);
	
			exit(1);
		}
	}
		
	if(p>0){
		
		close(fd[1]);
		
		for(int j=0;j<10;j++){
			wait(&s);
		}
		
		int product;
		while(read(fd[0], &product, sizeof(int))>0){
			products[k]=product;
			k++;
		}
		
		close(fd[0]);
		
		printf("Products that sold more than 20 units:\n");
		for(int j=0; j<k; j++){
			printf("%d\n",j);
			printf("Product %d\n",products[j]);
		}
	
	}
	
	return 0;
}
