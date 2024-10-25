#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define ARRAY_SIZE 1000

int main(void){
	int fd[5][2];
	pid_t p[4];
	int i, s;
	
	for(i=0; i<5; i++){
		if(pipe(fd[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
		
	for(i=0; i<3; i++){
		p[i] = fork();
		
		if(p[i]==-1){
			perror("Fork failed");
			return 1;
		}
		
		if(p[i]==0) break;
	}
	
	//M1
	if(p[0]>0){
		close(fd[0][0]);
			
			
		int notification=1;					//notificação a avisar o próximo processo que já foram cortadas 5 peças
		for(int i=0; i<ARRAY_SIZE; i+=5){
			printf("5 pieces cut. Notifying M2...\n");
			write(fd[0][1], &notification, sizeof(int));
		}
		notification=2;						//notificação a avisar o próximo processo que já foram cortadas todas as peças
		write(fd[0][1], &notification, sizeof(int)); 
		printf("All pieces were cut.\n");
			
		close(fd[0][1]);

	}
	
	//M2
	if(p[0]==0){
		close(fd[0][1]);
		close(fd[1][0]);
			
		int noti_received;
		while(read(fd[0][0], &noti_received, sizeof(int))==0){
		}
		
		int notification=1;					//notificação a avisar o próximo processo que já foram dobradas 5 peças
		while(noti_received!=2){		
			while(read(fd[0][0], &noti_received, sizeof(int))==0){		//esperar pela notificação do processo anterior
			}
		
			printf("5 pieces folded. Notifying M3...\n");
			write(fd[1][1], &notification, sizeof(int));
		}
		notification=2;						//notificação a avisar o próximo processo que já foram dobradas todas as peças
		write(fd[1][1], &notification, sizeof(int)); 
		printf("All pieces were folded.\n");
		
			
		close(fd[0][0]);
		close(fd[1][1]);
		
		exit(0);
	}
	
	//M3
	if(p[1]==0){
		close(fd[1][1]);
		close(fd[2][0]);
			
		int noti_received;
		while(read(fd[1][0], &noti_received, sizeof(int))==0){
		}
		
		int notification=1, wait=0;
		while(noti_received!=2){
			while(wait<2){					//como são soldadas 10 peças de cada vez, é preciso esperar que estejam 10 peças disponíveis (2 notificações do processo anterior)
				while(read(fd[1][0], &noti_received, sizeof(int))==0){		//esperar pela notificação do processo anterior a avisar que estão mais 5 peças disponíveis
				}
				wait++;
			}
			printf("10 pieces welded. Notifying M4...\n");
			write(fd[2][1], &notification, sizeof(int));
			wait=0;
		}
		notification=2;						//notificação a avisar o próximo processo que já foram soldadas todas as peças
		write(fd[2][1], &notification, sizeof(int)); 
		printf("All pieces were welded.\n");
		
			
		close(fd[1][0]);
		close(fd[2][1]);
		
		exit(0);
	}
	
	//M4
	if(p[2]==0){
		close(fd[2][1]);
			
		int noti_received;
		while(read(fd[2][0], &noti_received, sizeof(int))==0){
		}
		
		int wait=0;
		while(noti_received!=2){
			while(wait<10){							//como são embaladas 100 peças de cada vez, é preciso esperar que estejam 100 peças disponíveis (10 notificações do processo anterior)
				while(read(fd[2][0], &noti_received, sizeof(int))==0){		//esperar pela notificação do processo anterior a avisar que estão mais 10 peças disponíveis
				}
				wait++;
			}
			
			printf("100 pieces packed and transfered to storage A1.\n");
			wait=0;
		}
		
		printf("All pieces were packed and added to the inventory.\n");
		
			
		close(fd[2][0]);
		
		exit(0);
	}
	
	return 0;
}
