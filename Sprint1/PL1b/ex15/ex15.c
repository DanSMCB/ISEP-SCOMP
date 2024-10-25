#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct{
	char cmd[32]; // Command name .
	int time_cap; // Time limit to complete (in seconds ).
}command_t ;


void handle_ALARM(int signo){
	write(STDOUT_FILENO, "The command did not complete in its allowed time!\n", 53);
}


int main (){

	char *cmd[3]={"Command A", "Command B", "Command C"};
	int time_cap[3]={5,12,3};

	command_t commands[3];
	for (int i=0; i<3; i++)
    {
		strcpy(commands[i].cmd, cmd[i]);
		commands[i].time_cap=time_cap[i];
    }
    
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_sigaction = handle_ALARM;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGALRM, &act, NULL);

	
	for(int i=0; i<3; i++){
		printf("Executing %s\n",commands[i].cmd);
		alarm(commands[i].time_cap);
		if(sleep(10)==0){
			printf("Execution ended!\n");
		}
		alarm(0);
	}

	return 0;
}
