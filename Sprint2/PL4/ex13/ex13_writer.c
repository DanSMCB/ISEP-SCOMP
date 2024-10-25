#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include "ex13.h"
#include <time.h>
#include <string.h>

int main() {
	sem_unlink("/sem_ex13");
	time_t t;
	time(&t);
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
	
	sem_t *sem = sem_open("/sem_ex13", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    sem_wait(sem);
    shared_data->numWriters++;
    sem_post(sem);
    
    while(1){
		sem_wait(sem);
		printf("Type the string (\"0\" to exit):\n");
		scanf("%s",shared_data->string);
		printf("Writer Process: PID %d, Time %ld (Writers: %d, Readers: %d)\n",
           getpid(), ctime(&t), shared_data->numWriters, shared_data->numReaders);
		if(strcmp(shared_data->string,"0")==0){
			
			shared_data->numWriters--;
			sem_post(sem);
			
			if (munmap(shared_data, data_size) == -1){
				perror("Error in munmap\n");
				exit(1);
			}

			if(close(fd) == -1) {
				perror("Error in munmap\n");
				exit(1);
			}
			sem_close(sem);
			return 0;
		}
		sem_post(sem);
	}
}

