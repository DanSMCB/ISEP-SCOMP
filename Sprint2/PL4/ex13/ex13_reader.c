#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include "ex13.h"

int main() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    fd = shm_open("/ex13", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }
    ftruncate(fd, data_size);
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    sem_t* sem_readers = sem_open("/sem_readers", O_CREAT, 0644, 1);
    if (sem_readers == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    
    
    
    while(1){
		sem_wait(sem_readers);
		shared_data->numReaders++;
		
		sleep(1);
		printf("String in shared memory: %s (Readers: %d)\n", shared_data->string, shared_data->numReaders);
		
		shared_data->numReaders--;
		sem_post(sem_readers);
	}
    
    if (munmap(shared_data, data_size) == -1){
		perror("Error in munmap\n");
		exit(1);
	}

	if(close(fd) == -1) {
		perror("Error in munmap\n");
		exit(1);
	}
    
    sem_close(sem_readers);
    
    return 0;
}
