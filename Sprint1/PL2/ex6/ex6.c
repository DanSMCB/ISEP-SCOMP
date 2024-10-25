#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

int main() {
    int vec1[1000], vec2[1000]; 
    time_t t;
    srand((unsigned) time(&t)); // Inicializa o gerador de números aleatórios com o tempo atual

    for (int i = 0; i < 1000; i++) { // Enche os vetores com números aleatórios
        vec1[i] = abs(rand()) % 100;
        vec2[i] = abs(rand()) % 100;
    }

    int fd[2];

    if (pipe(fd) == -1) { // Criação do pipe
        perror("Pipe failed.\n");
        return 1;
    }

    pid_t p;
    int status, tmp;

    for (int i = 0; i < 5; i++) { // Criação dos processos filhos
        p = fork();
        
        if (p < 0) {
            perror("Fork failed.\n");
            return 1;
        }

        if (p == 0) { // Código executado pelos processos filhos
            close(fd[0]); // Fecha a leitura do pipe
            int start = i * 200; // Calcula o início e o fim do intervalo do vetor que este processo deve somar
            int finish = start + 200;
            for (int j = start; j < finish; j++) { // Soma os elementos do intervalo determinado
                tmp += vec1[j] + vec2[j];
            }

            write(fd[1], &tmp, sizeof(tmp)); // Escreve o resultado no pipe
            close(fd[1]); // Fecha a escrita do pipe
            exit(0); // Encerra o processo filho
        }
    }

    close(fd[1]); // Fecha a escrita do pipe no processo pai

    int p_sum, sum = 0;

    for (int i = 0; i < 5; i++) { // Lê os resultados do pipe e calcula a soma final
        wait(&status);
        p_sum = 0;
        read(fd[0], &p_sum, sizeof(p_sum)); // Lê o resultado do pipe
        printf("Partial sum - parent process: %d\n", p_sum); // Imprime o resultado parcial
        sum += p_sum; // Acumula o resultado parcial na soma total
    }

    close(fd[0]); // Fecha a leitura do pipe no processo pai

    for (int i = 0; i < 200; i++) { // Imprime os primeiros 200 elementos do vetor 1
        printf("Vec1: %d\n", vec1[i]);
    }

    for (int i = 0; i < 200; i++) { // Imprime os primeiros 200 elementos do vetor 2
        printf("Vec2: %d\n", vec2[i]);
    }

    printf("Total sum: %d\n", sum); // Imprime o resultado final da soma

    return 0;
}
