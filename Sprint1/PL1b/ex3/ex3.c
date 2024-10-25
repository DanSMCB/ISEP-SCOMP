#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int main (){
	
	pid_t pid;
	int sig;
	
	printf("Enter pid: \n");
	scanf("%d", &pid);
   
	printf("Enter sig: \n");
	scanf("%d", &sig);
	
	kill(pid, sig);
}
