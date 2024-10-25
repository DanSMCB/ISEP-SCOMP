#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Função para criar n processos filhos e retornar um valor para cada filho
// 1 para o primeiro filho criado, 2 para o segundo filho criado, etc.
// e 0 para o processo pai.
int spawn_childs(int n) {
    int i;
    pid_t p;

    for (i = 0; i < n; i++) {
        p = fork();
        if (p < 0) { // erro ao criar processo filho
            perror("Fork failed.\n");
            exit(1);
        }
        
        if (p == 0) { // processo filho
            return i + 1; // retorna valor para o filho
        }
    }
    return 0; // processo pai
}

int main() {
    pid_t p;
    int i, n, status;

    n = spawn_childs(6); // cria 6 processos filhos

    if (n > 0) { // processo filho
        exit(n * 2); // sai com valor igual ao índice multiplicado por 2
    }

    for (i = 0; i < 6; i++) { // processo pai
        wait(&status); // aguarda finalização de todos os filhos
        if (WIFEXITED(status)) { // se o filho saiu normalmente
            printf("Valor de saída: %d\n", WEXITSTATUS(status)); // imprime valor de saída do filho
        }
    }
    return 0;
}
