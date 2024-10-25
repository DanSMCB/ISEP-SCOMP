#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>

// define uma struct para armazenar a mensagem "Win" e o número da rodada
struct st {
    char sms[10];
    int num_round;
};

int main () {
    struct st st1;
    int fd[2];

    // cria o pipe
    if (pipe(fd) == -1) {
        perror("Pipe failed.\n");
        return 1;
    }

    pid_t p;

    // cria 10 processos filhos
    for (int i = 0; i < 10; i++) {
        p = fork();

        // trata erro no fork
        if (p < 0) {
            perror("Fork failed.\n");
            return 1;
        }

        // código para o processo filho
        if (p == 0) {
            close(fd[1]); // fecha a escrita do pipe
            if (read(fd[0], &st1, sizeof(st1)) != 0) {
                // se o filho conseguir ler dados do pipe com sucesso, imprime a mensagem de vitória e o número da rodada
                printf("Winner! Round number %d\n", st1.num_round);
                // encerra a execução com um valor de saída igual ao número da rodada vencedora
                exit(st1.num_round);
            }
            close(fd[0]); // fecha a leitura do pipe
        }
    }

    // código para o processo pai
    for (int i = 0; i < 10; i++) {
        sleep(2); // espera 2 segundos

        // preenche a estrutura com a mensagem de vitória e o número da rodada
        st1.num_round = i+1;
        strcpy(st1.sms, "Win");

        close(fd[0]); // fecha a leitura do pipe
        write(fd[1], &st1, sizeof(st1)); // escreve os dados da estrutura no pipe
    }

    int status = 0;

    // espera que todos os processos filhos terminem e imprime o PID e a rodada vencedora de cada um
    for (int i = 0; i < 10; i++) {
        pid_t pid = wait(&status);
        sleep(1); // espera 1 segundo para que o printf seja impresso apenas depois da última rodada
        printf("Round %d with Child's PID %d!\n", WEXITSTATUS(status), pid);
    }
}
