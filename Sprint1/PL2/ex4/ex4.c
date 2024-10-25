#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

const char* filename = "text.txt";
FILE *file;
char *line;
size_t size;

int main() {
    int fd[2];

    if(pipe(fd) == -1) {  // cria o pipe e verifica se houve erro
        perror("Pipe failed.\n");
        return 1;
    }

    pid_t p = fork();  // cria um processo filho

    if (p < 0) {  // verifica se houve erro ao criar o processo filho
        perror("Fork failed.\n");
        return 1;
    }

    if (p > 0) {  // código executado pelo processo pai
        close(fd[0]);  // fecha a leitura do pipe
        
        file = fopen(filename, "r");  // abre o arquivo de texto em modo de leitura
        if (file == NULL) {  // verifica se houve erro ao abrir o arquivo
            exit(-1);  
        }
        
        while ((getline(&line, &size, file)) != -1) {  // lê cada linha do arquivo de texto
            write(fd[1], line, strlen(line));  // escreve a linha no pipe
        }
        fclose(file);  // fecha o arquivo de texto
        close(fd[1]);  // fecha a escrita do pipe
        
        wait(NULL);  // espera o processo filho terminar
        printf("\n\n---> The child process finished. <---\n");  
    } else {  // código executado pelo processo filho
        close(fd[1]);  // fecha a escrita do pipe
        
        char file_str[100];  // define um buffer para armazenar os dados recebidos do pipe
		while(read(fd[0], file_str, sizeof(file_str))){  // lê os dados do pipe enquanto houver dados
			printf("%s ", file_str);  // imprime os dados lidos na tela
		}
        
        close(fd[0]);  // fecha a leitura do pipe
    }
}
