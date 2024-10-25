#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

volatile sig_atomic_t children_running = 5; // contador de filhos ainda em execução

// Manipulador de sinal para SIGUSR1
void handle_SIGUSR1(int signum) {
    children_running--; // decrementa o contador de filhos em execução
}

int main() {
    // Configura o manipulador de sinal para SIGUSR1
    signal(SIGUSR1, handle_SIGUSR1);

    // Loop para gerar os 5 processos filhos
    for (int i = 0; i < 5; i++) {
        pid_t pid = fork(); // cria um novo processo filho

        if (pid < 0) {
            // Se fork() retornar um valor negativo, houve um erro
            printf("Error: failed to create child process\n");
            return 1;
        } else if (pid == 0) {
            // Se fork() retornar 0, este é o processo filho
            int start = i * 200; // limite inferior do intervalo
            int end = (i + 1) * 200 - 1; // limite superior do intervalo

            // Imprime todos os números no intervalo [start, end]
            for (int j = start; j <= end; j++) {
                printf("%d\n", j);
            }

            kill(getppid(), SIGUSR1); // notifica o processo pai que terminou
            return 0; // termina o processo filho
        }
    }

    // Loop para esperar que todos os filhos terminem
    while (children_running > 0) {
        pause(); // espera até receber um sinal
    }

    // Loop para chamar a função wait() para cada filho
    for (int i = 0; i < 5; i++) {
        int status;
        wait(&status); // espera que o processo filho termine e captura o seu estado
    }

    return 0;
}
