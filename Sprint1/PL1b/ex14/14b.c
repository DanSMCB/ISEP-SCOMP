#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define TIMEOUT 10 // Tempo limite em segundos

void handle_timeout(int sig)
{
    // Envia um sinal SIGUSR1 para o processo pai
    kill(getppid(), SIGUSR1);
    exit(0);
}

int main()
{
    char input[100];
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0) // Processo filho
    {
        signal(SIGALRM, handle_timeout);
        alarm(TIMEOUT);

        pause(); // Espera pelo sinal do pai

        exit(0);
    }
    else if (pid > 0) // Processo pai
    {
        printf("Digite uma string (tempo limite: %d segundos):\n", TIMEOUT);
        fgets(input, sizeof(input), stdin);

        waitpid(pid, &status, WNOHANG); // Verifica se o processo filho já terminou

        if (!WIFEXITED(status)) // O processo filho ainda está em execução
        {
            printf("Es muito lento, o programa terminou!\n");
            kill(pid, SIGKILL); // Mata o processo filho
        }
        else // O processo filho já terminou
        {
            printf("O tamanho da string é: %zu\n", strlen(input));
        }
    }
    else // Erro ao criar o processo filho
    {
        printf("Erro ao criar o processo filho.\n");
        exit(1);
    }

    return 0;
}
