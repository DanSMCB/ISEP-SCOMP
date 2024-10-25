#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t USR1_counter = 0;

void handle_USR1(int signum) {
    // bloqueia todos os sinais
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    // incrementa o contador e mostra a mensagem
    USR1_counter++;
    printf("SIGUSR1 signal captured: USR1 counter = %d\n", USR1_counter);

    // desbloqueia todos os sinais
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Fork falhou
        printf("Error: failed to create child process\n");
        return 1;
    } else if (pid == 0) {
        // processo filho
        for (int i = 0; i < 12; i++) {
            kill(getppid(), SIGUSR1);
            struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 10000000}; // 10 ms
            nanosleep(&sleep_time, NULL);
        }
        kill(getppid(), SIGINT);
        return 0;
    } else {
        // processo pai
        signal(SIGUSR1, handle_USR1);

         // loop infinito
        while (1) {
            printf("I'm working!\n");
            sleep(1);
        }
    }

    return 0;
}
