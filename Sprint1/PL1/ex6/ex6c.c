#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
 
int main () {
    
    pid_t p;
    int status;

    for (int i = 0; i < 2; i++) { // executa o loop duas vezes
        p = fork(); // cria um novo processo

        if (p < 0) { // verifica se o fork falhou
            perror("Fork failed.\n");
            exit(-1);
        }

        if (p > 0 && p % 2 != 0) { // se o processo atual for o pai e o PID for ímpar
            wait(&status); // espera pelo processo filho terminar
        }
    }

    printf("This is the end.\n"); 
}
