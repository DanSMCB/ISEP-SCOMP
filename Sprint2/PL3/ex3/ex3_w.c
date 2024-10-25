#include "ex3.h"
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
#include <time.h>

int main(int argc, char *argv[]) {
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/ex3", O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	time_t t;
	srand ((unsigned) time (&t));
	
	if(fd==-1){
		perror("Error in shm_open");
		exit(1);
	}
	
	if(ftruncate(fd, data_size)==-1){
		perror("Error in ftruncate");
		exit(1);
	}
	shared_data = (shared_data_type*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	for(int i=0; i<ARRAY_SIZE; i++){
		shared_data->num[i] = rand()%100;	
	}
	
	if (munmap(shared_data, data_size) == -1){
		perror("Error in munmap\n");
		exit(1);
    }

	if(close(fd) == -1) {
		perror("Error in munmap\n");
		exit(1);
	}
   
    return 0;
}
