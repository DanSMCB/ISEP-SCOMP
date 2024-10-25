#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

int num_finished = 0;
int num_success = 0;

// função para tratar o sinal SIGUSR1 e SIGUSR2
void handle_signal(int signum) {
    if (signum == SIGUSR1) {
        printf("Processo filho %d: simulate1() teve sucesso\n", getpid());
    } else if (signum == SIGUSR2) {
        printf("Processo filho %d: simulate1() não teve sucesso\n", getpid());
    }
}

// função para simular a execução da simulate1()
int simulate1() {
    srand(time(NULL) + getpid()); // semente aleatória
    int result = rand() % 20;
    if (result == 0) {
        return 1;
    } else {
        return 0;
    }
}

// função para simular a execução da simulate2()
void simulate2() {
    srand(time(NULL) + getpid()); // semente aleatória
    int result = rand() % 20;
    printf("Processo filho %d: simulate2() teve resultado %d\n", getpid(), result);
}

int main() {
    int num_processes = 50;

    // criando processos filhos
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // processo filho
            signal(SIGUSR1, handle_signal);
            signal(SIGUSR2, handle_signal);
            int success = simulate1();
            if (success) {
                kill(getppid(), SIGUSR1); // notifica o processo pai
            } else {
                kill(getppid(), SIGUSR2); // notifica o processo pai
            }
            pause(); // aguarda sinal do processo pai para iniciar a simulate2()
            simulate2();
            exit(0);
        }
    }

    // processo pai
    signal(SIGUSR1, SIG_IGN); // ignora sinal SIGUSR1
    signal(SIGUSR2, SIG_IGN); // ignora sinal SIGUSR2

    // aguardando notificações dos processos filhos
    while (num_finished < num_processes) {
        int status;
        pid_t pid = wait(&status);
        if (pid > 0) {
            num_finished++;
            if (WIFSIGNALED(status)) {
                int signum = WTERMSIG(status);
                if (signum == SIGUSR1) {
                    num_success++;
                }
            }
        }
        if (num_finished == 25 && num_success == 0) {
            printf("Algoritmo ineficiente!\n");
            for (int i = 0; i < num_processes; i++) {
                kill(pid, SIGTERM); // termina todos os processos filhos
            }
            exit(0);
        } else if (num_success > 0 && num_finished == num_processes) {
            for (int i = 0; i < num_processes; i++) {
                kill(pid, SIGUSR1); // envia sinal para iniciar a simulate2()
            }
        }
    }

    return 0;
}
