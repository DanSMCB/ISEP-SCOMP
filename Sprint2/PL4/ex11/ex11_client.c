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
    fd = shm_open("/ex11", O_RDWR, 0644);
    shared_data = (shared_data_type *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *sem = sem_open("/sem_ex11", 0);

    srand(time(NULL));

    sem_wait(sem);
    int ticketNumber = shared_data->ticketNumber;
    sem_post(sem);

    printf("Client received ticket number %d\n", ticketNumber);

    int serviceTime = rand() % 10 + 1;
    sleep(serviceTime);

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
