#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// Define a função que será chamada quando o sinal SIGUSR1 for recebido
void sigusr1_handler(int sig)
{
    // Obtém o PID do processo que enviou o sinal
    pid_t sender_pid = getpid();
    // Cria uma mensagem com o PID do processo que enviou o sinal
    char msg[100];
    sprintf(msg, "I captured a SIGUSR1 sent by the process with PID %d\n", sender_pid);
    // Escreve a mensagem no terminal
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main()
{
    // Imprime o PID do processo atual
    printf("My PID is %d\n", getpid());
    // Registra a função sigusr1_handler para ser chamada quando o sinal SIGUSR1 for recebido
    signal(SIGUSR1, sigusr1_handler);
    // Loop infinito para manter o processo em execução e aguardar sinais
    while(1) {
        sleep(1); // wait for signals
    }
    return 0;
}
