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

#define N 1000000

typedef struct{
	int a;
	int b;
} shared_data_type;

int main(int argc, char *argv[]) {
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/ex5", O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	
	if(fd==-1){
		perror("Error in shm_open");
		exit(1);
	}
	
	if(ftruncate(fd, data_size)==-1){
		perror("Error in ftruncate");
		exit(1);
	}
		
	shared_data = (shared_data_type*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	shared_data->a=100;
	shared_data->b=100;
	
	pid_t p=fork();
	
	if(p==-1){
		perror("Error in fork\n");
		exit(1);
	}
	if(p==0){
		for(int i=0; i<N; i++){
			//pause();
			shared_data->a++;
			shared_data->b--;
			kill(SIGUSR1, getppid());
		}
	
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
	if(p>0){
		for(int i=0; i<N; i++){
			shared_data->b++;
			shared_data->a--;
			kill(SIGUSR1, p);
			//pause();
		}
		
		printf("\"a\" value: %d\n\"b\" value: %d\n", shared_data->a, shared_data->b);
	
		if (munmap(shared_data, data_size) == -1){
			perror("Error in munmap\n");
			exit(1);
		}

		if(close(fd) == -1) {
			perror("Error in munmap\n");
			exit(1);
		}
	
		if (shm_unlink("/ex5") == -1) {
			perror("Error in shm_unlink\n");
			exit(1);
		}
   
		return 0;
	}
	
	
}
