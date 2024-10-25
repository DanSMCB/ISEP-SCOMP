#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_USR1(int signo){
	write(STDOUT_FILENO, "SIGUSR1 was received and handled.\n", 34);
}


int main (){

	pid_t p;
	int s;

	p=fork();
	
	if(p==-1){
		printf("Fork failed\n");
		return-1;
	} 

	if(p>0){
		
		for(int i=1; i<4; i++){
			sleep(1);
			printf("Executing task A; %d seconds have passed.\n",i);
		}
		
		printf("Task A: done!\n");
		kill(p, SIGUSR1);
		wait(&s);
		printf("Job is complete!\n");
	
	}else{
	
		struct sigaction act;
		memset(&act, 0, sizeof(struct sigaction));
		sigemptyset(&act.sa_mask);
		act.sa_sigaction = handle_USR1;
		act.sa_flags = SA_SIGINFO;
		sigaction(SIGUSR1, &act, NULL);
		
		pause();
		
		time_t t;
        srand((unsigned) time(&t));
		
		int duration = rand() %4 + 1;
		sleep(duration);
	
		for(int i=1; i<duration; i++){
			sleep(1);
			printf("Executing task B; %d seconds have passed.\n",i);
		}
		
		printf("Task B: done!.\n");
		
		sleep(1);
		
		printf("Task C: done!.\n");
	
	}
	
	return 0;
}
