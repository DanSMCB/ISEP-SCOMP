#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 

#define STR_SIZE 50
#define NR_DISC 10
#define NR_PROC 2

typedef struct{
	int numero;
	char nome[STR_SIZE];
	int disciplinas[NR_DISC];
}aluno;

int main(int argc, char *argv[]) {
	int fd, data_size = sizeof(aluno);
	aluno *shared_data;
	int s;
	fd = shm_open("/ex7", O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	
	if(fd==-1){
		perror("Error in shm_open");
		exit(1);
	}
	
	if(ftruncate(fd, data_size)==-1){
		perror("Error in ftruncate");
		exit(1);
	}
	
	shared_data = (aluno*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	pid_t p = fork();
	if(p==-1){
		perror("Error in fork\n");
		exit(1);
	}
	
	if(p>0){
		pid_t p2=fork();
		
		if(p2==-1){
			perror("Error in fork\n");
			exit(1);
		}
		
		if(p2==0){
			while(shared_data->numero=0){}
			int sum=0;
			float average;
			
			for(int i=0; i<NR_DISC; i++){
				sum+=shared_data->disciplinas[i];
			}
			
			average=sum/NR_DISC;
			printf("Student's average grade: %2f\n", average);
			
			if (munmap(shared_data, data_size) == -1){
				perror("Error in munmap\n");
				exit(1);
			}

			if(close(fd) == -1) {
				perror("Error in munmap\n");
				exit(1);
			}
		
			exit(0);
		}
		
		printf("Write the student's number:\n");
		scanf("%d",&shared_data->numero);
		printf("Write the student's name:\n");
		scanf("%s",&shared_data->nome);
		printf("Write the student's grades:\n");
		for(int i=0; i<NR_DISC; i++){
			scanf("%d",&shared_data->disciplinas[i]);	
		}
		
		for(int i=0; i<NR_PROC; i++){
			wait(&s);
		}
		
		if (munmap(shared_data, data_size) == -1){
			perror("Error in munmap\n");
			exit(1);
		}
	
		if (shm_unlink("/ex7") == -1) {
			perror("Error in shm_unlink\n");
			exit(1);
		}
		
		return 0;
	}else{
		while(shared_data->numero=0){}
		
		int highest=0, lowest=20;
		for(int i=0; i<NR_DISC; i++){
			if(shared_data->disciplinas[i]>highest){
				highest=shared_data->disciplinas[i];
			}
			if(shared_data->disciplinas[i]<lowest){
				lowest=shared_data->disciplinas[i];
			}
		}
		
		printf("Student's highest grade: %d\nStudent's lowest grade: %d\n",highest, lowest);
		
		if (munmap(shared_data, data_size) == -1){
			perror("Error in munmap\n");
			exit(1);
		}

		if(close(fd) == -1) {
			perror("Error in munmap\n");
			exit(1);
		}
		
		exit(0);
	}
	
}
