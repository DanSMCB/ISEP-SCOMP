#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	
	int status;
	
	pid_t p = fork();

	if(p!=0){
		waitpid(p, &status, 0);
		printf("%d\n",WEXITSTATUS(status));
		pid_t p1 = fork();
		if(p!=0){
			waitpid(p1, &status, 0);
			printf("%d\n",WEXITSTATUS(status));
		}
		else{
			sleep(2);
			exit(2);
		}
	}
	else{
		sleep(1);
		exit(1);
	}
}
