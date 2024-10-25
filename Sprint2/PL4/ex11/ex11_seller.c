#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include "ex11.h"

int main() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    fd = shm_open("/ex11", O_CREAT | O_RDWR, 0644);
    ftruncate(fd, data_size);
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    shared_data->ticketNumber = 1;

    sem_t *sem = sem_open("/sem_ex11", O_CREAT | O_EXCL, 0644, 1);

    srand(time(NULL));

    while (1) {
        sem_wait(sem);

        printf("Seller is selling ticket number %d\n", shared_data->ticketNumber);
        shared_data->ticketNumber++;

        sem_post(sem);
        sleep(1);
    }

    if (munmap(shared_data, data_size) == -1){
		perror("Error in munmap\n");
		exit(1);
	}

	if(close(fd) == -1) {
		perror("Error in munmap\n");
		exit(1);
	}
	
    shm_unlink("/ex11");
    sem_unlink("/sem_ex11");

    return 0;
}
