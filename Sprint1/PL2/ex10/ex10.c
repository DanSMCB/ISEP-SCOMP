#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

const int still = 1, finish = 0;

int main () {
    time_t t;
    srand((unsigned) time(&t));

    int fd1[2]; 
    int fd2[2]; 

    if (pipe(fd1) == -1) { //cria o primeiro pipe
        perror("Pipe 1 failed.\n");
        return 1;
    }

    if (pipe(fd2) == -1) { //cria o segundo pipe
        perror("Pipe 2 failed.\n");
        return 1;
    }

    pid_t p;
    int credit_c = 0, bet_c = 0;

    p = fork(); //cria um novo processo

    if (p < 0) {
        perror("Fork failed.\n");
        return 1;
    }

    if (p > 0) { //processo pai
        //srand(time(NULL));

        close(fd2[1]);
        close(fd1[0]);
        credit_c = 20;
        while (credit_c > 0) {
            int r = rand() % 5 + 1; //gera um número aleatório entre 1 e 5
            printf("Random number: %d\n", r);

            write(fd1[1], &still, sizeof(still)); //escreve no primeiro pipe

            read(fd2[0], &bet_c, sizeof(bet_c)); //lê a aposta do filho

            if (bet_c == r) {
                printf("Child bet %d was right!\n", bet_c); //se a aposta do filho estiver correta
                credit_c += 10; //aumenta o crédito do filho em 10 euros
            } else {
                printf("Child bet %d was not right...\n", bet_c); //se a aposta do filho estiver errada
                credit_c -= 5; //diminui o crédito do filho em 5 euros
            }
            printf("Remaining Credit: %d\n\n\n", credit_c);

            write(fd1[1], &credit_c, sizeof(credit_c)); //escreve o novo crédito no primeiro pipe
        }
        write(fd1[1], &finish, sizeof(finish)); //escreve 0 no primeiro pipe para notificar o filho que o crédito acabou
        close(fd2[0]);
        close(fd1[1]);
    }

    int notification = 1;

    if (p == 0) { //processo filho
        srand(time(NULL) * getpid()); //gera uma semente aleatória baseada no PID do processo

        close(fd2[0]);
        close(fd1[1]);

        while (notification) {
            read(fd1[0], &notification, sizeof(notification)); //lê a notificação do primeiro pipe
            int r = rand() % 5 + 1; //gera um número aleatório entre 1 e 5
            write(fd2[1], &r, sizeof(r)); //escreve a aposta no segundo pipe
        }
        close(fd2[1]);
        close(fd1[0]);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_SUCCESS);
}
