#include <stdio.h>
#include <signal.h>
#include <unistd.h>

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
    // instala o signal handler para SIGUSR1 
    signal(SIGUSR1, handle_USR1);

    // loop infinito
    while (1) {
        printf("I'm working!\n");
        sleep(1);
    }

    return 0;
}
