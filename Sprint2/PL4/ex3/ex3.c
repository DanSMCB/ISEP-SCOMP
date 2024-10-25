#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <time.h>
#include <string.h>

typedef struct {
	char strings[50][80];
    int index;
} shared_data_type;

int main() {
	int espera;
	int prob;
	srand(time(NULL));
	struct timespec ts;
    
    
    sem_unlink("/sem_ex03");
    sem_t *sem = sem_open("/sem_ex03", O_CREAT|O_EXCL, 0644, 1); //Semáforo exclusão mútua

    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    shm_unlink("/ex03"); //Removes the memory area from the file system

    fd = shm_open("/ex03", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //Creates and opens a shared memory area
    ftruncate(fd, data_size); //Defines the size
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Maps the shared area in the process address space

    if (sem == SEM_FAILED) {
        perror("Error: sem_open()");
        exit(EXIT_FAILURE);
    }

    
    int i ;
    for ( i = 0; i < 50; i++) {
		ts.tv_sec += rand()%12;
		sem_timedwait(sem, &ts);
		
		espera=rand()%5;
		sleep(espera);
		prob=rand()%10;
        sprintf(shared_data->strings[i], "I'm the Father - with PID %d\n", getppid());
        if(prob<3){
			printf("Last written string deleted: %s\n", shared_data->strings[i]);
			strcpy(shared_data->strings[i], "");
            i--;
        }
        
        sem_post(sem);
    }
   

    munmap(shared_data, data_size);
    close(fd);
}
