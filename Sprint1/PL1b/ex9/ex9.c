#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_INT(int signo){
	char buffer[40] = "\0";
	sprintf(buffer, "\nI won’t let the process end with CTRL-C!\n");
	
	write(STDOUT_FILENO, buffer, sizeof(buffer)+1);
}

void handle_QUIT(int signo){
	char buffer[40] = "\0";
	sprintf(buffer, "\nI won’t let the process end with CTRL-\\!\n");
	
	write(STDOUT_FILENO, buffer, sizeof(buffer)+1);
}


int main (){
	
	struct sigaction act;
	
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_handler = handle_INT;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &act, NULL);
	
	struct sigaction act2;
	
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act2.sa_mask);
	act2.sa_handler = handle_QUIT;
	act2.sa_flags = SA_SIGINFO;
	sigaction(SIGQUIT, &act2, NULL);
	
	for( ; ; ){
		printf("I like signals \n");
		sleep(1);
	}
}
