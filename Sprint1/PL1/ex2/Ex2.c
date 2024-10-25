#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t p;

    p = fork(); // cria um novo processo

    if (p < 0) { // verifica se o fork falhou
        printf("Fork failed.\n");
        exit(0);
    }

    if (p == 0) { // se o valor retornado pelo fork for 0, significa que é o processo filho
        printf("I'm..\n"); 
    } else { // caso contrário, é o processo pai
        wait(NULL); // espera pelo processo filho terminar
        printf("I'll never join you!\n"); 

        p = fork(); // cria outro processo filho
        if (p == 0) { // se o valor retornado pelo fork for 0, significa que é o processo filho
            printf("the..\n"); 
        } else { // caso contrário, é o processo pai
            wait(NULL); // espera pelo processo filho terminar
            printf("I'll never join you!\n"); 

            p = fork(); // cria outro processo filho
            if (p == 0) { // se o valor retornado pelo fork for 0, significa que é o processo filho
                printf("father!\n"); 
            } else { // caso contrário, é o processo pai
                wait(NULL); // espera pelo processo filho terminar
                printf("I'll never join you!\n"); 
            }
        }
    }

    return 0; // finaliza o programa
}
