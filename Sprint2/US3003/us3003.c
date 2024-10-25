#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

#define ARRAY_LENGHT 10
#define MAX_STRING_LENGHT 10

typedef struct {
    int board[ARRAY_LENGHT][ARRAY_LENGHT];
} shared_data_type;

void printBoard(shared_data_type *shared_data);

int main() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    int row, column;
    char update[MAX_STRING_LENGHT];

    fd = shm_open("/shared_board", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
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
	
	sem_t *sem = sem_open("/sem_shared_board", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
		
	pid_t p = fork();
	if(p>0){
		while(1){
		sem_wait(sem);
		kill(p, SIGINT);
		
		bool valid=false;
		while(!valid){
			printf("Type the row of the board you wish to edit:\n");
			scanf("%d", &row);
			printf("Type the column of the board you wish to edit:\n");
			scanf("%d", &column);
		
			if((row<=0 || row>ARRAY_LENGHT) || (column<=0 || column>ARRAY_LENGHT)){
				valid=false;
				printf("Values out of board range. Type again.\n");
			}else{
				valid=true;
			}
		}
		
		printf("Type the integer to update the position [%d][%d] on the board:\n", row, column);
		scanf("%d", &shared_data->board[row-1][column-1]);
		
		printf("\n----------------Updated Board------------------------\n");
		printBoard(shared_data);
		
		printf("\nDo you wish to do more updates on the board? (Type \"0\" to exit or any other key to update again)\n");
		scanf("%s",update);
		
		if(strcmp(update,"0")==0){
			
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
		
	}if(p==0){
		while(1){
			printBoard(shared_data);
			sleep(3);
		}
		
	}else{
		printf("Fork failed.\n");
        return 1;
	}
	
}

void printBoard(shared_data_type *shared_data){
	for(int i=0; i<ARRAY_LENGHT; i++){
			for(int j=0; j<ARRAY_LENGHT; j++){
				printf(" [%d]", shared_data->board[i][j]);
			}
			printf("\n");
		}
}
