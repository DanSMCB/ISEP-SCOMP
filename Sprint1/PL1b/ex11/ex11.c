#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handle_USR1(int signo){
	sigset_t cmask;
	
	sigprocmask(0, NULL, &cmask);
	
	for (int i=1; i<=64; i++) {
        if(sigismember(&cmask, i) == 1) {
            char buffer[38] = "\0";
            sprintf(buffer, "The Signal with number %d is blocked.\n", i);
            write(STDOUT_FILENO, buffer, (sizeof(buffer)+1));
        }
	}
}


int main (){
	
	struct sigaction act;
	
	memset(&act, 0, sizeof(struct sigaction));
	sigfillset(&act.sa_mask);
	act.sa_handler = handle_USR1;
	sigaction(SIGUSR1, &act, NULL);
	
	
	for( ; ; ){
		printf("I like signals \n");
		sleep(4);
	}
	
	return 0;
}
