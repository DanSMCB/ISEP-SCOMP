#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t flag = 0;

void handle_alarm(int signum) {
    flag = 1;
}

int main() {
    char input_string[100];
    printf("Insira uma string (10 segundos limite): ");
    fflush(stdout);

    signal(SIGALRM, handle_alarm);
    alarm(10);

    fgets(input_string, 100, stdin);

    if (flag) {
        printf("Es muito lento, o programa terminou!\n");
        exit(1);
    } else {
        int string_size = strlen(input_string) - 1; // subtrai 1 para não contar o caractere nulo
        printf("O tamanho da string é: %d\n", string_size);
        exit(0);
    }
}
