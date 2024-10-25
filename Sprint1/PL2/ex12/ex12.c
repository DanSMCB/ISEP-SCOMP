#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd[6][2]; // 5 pipes pai-filho e +1 entre filhos
    pid_t p;
    int barcode, index;

    struct product {
        char name[20];
        float price;
        int barcode;
    };

    // banco de dados de produtos
    struct product database_prod[5];
    strcpy(database_prod[0].name, "iPhone");
    database_prod[0].price = 1279.00;
    database_prod[0].barcode = 1;
    strcpy(database_prod[1].name, "Samsung");
    database_prod[1].price = 875.00;
    database_prod[1].barcode = 2;
    strcpy(database_prod[2].name, "Xiaomi");
    database_prod[2].price = 250.00;
    database_prod[2].barcode = 3;
    strcpy(database_prod[3].name, "Huawei");
    database_prod[3].price = 279.00;
    database_prod[3].barcode = 4;
    strcpy(database_prod[4].name, "Nokia");
    database_prod[4].price = 289.00;
    database_prod[4].barcode = 5;

    // criação dos pipes
    for (int i = 0; i < 6; i++){
		if (pipe(fd[i]) == -1){
			perror("Pipe failed.\n");
			return 1;
		}
	}
    
    // criação dos processos filhos (leitores de código de barras)
    for (int i = 0; i < 5; i++) {
        p = fork();

        if (p < 0) {
            perror("Fork failed.\n");
            return 1;
        }

        if (p == 0) { // código do processo filho
            close(fd[0][0]); // fecha o lado de leitura do pipe pai-filho
            close(fd[i+1][1]); // fecha o lado de escrita do pipe entre filhos

            printf("Reader %d\n", i+1);
            printf("-> Input barcode (1-5):\n\n");
            scanf("%d", &barcode); // lê o código de barras

            write(fd[0][1], &barcode, sizeof(barcode)); // escreve o código de barras no pipe pai-filho
            write(fd[0][1], &i, sizeof(i)); // escreve o índice do processo filho no pipe pai-filho

            struct product product1;
            read(fd[i+1][0], &product1, sizeof(struct product)); // lê as informações do produto do pipe entre filhos
            printf("Reader %d\n", i+1);
            printf("Product: %s | Price: %f\n", product1.name, product1.price); // exibe as informações do produto na tela

            close(fd[0][1]); // fecha o lado de escrita do pipe pai-filho
            close(fd[i+1][0]); // fecha o lado de leitura do pipe entre filhos
            exit(0);
        }
    }

    //codigo do processo pai
for (int i = 0; i < 5; i++) {
close(fd[0][1]); //fecha a escrita do pipe principal para o processo pai 
close(fd[i+1][0]); //fecha a leitura do pipe entre o pai e o processo atual

    read(fd[0][0], &barcode, sizeof(barcode)); //le o codigo de barras a partir do pipe principal
    read(fd[0][0], &index, sizeof(index)); //le o indice do processo atual a partir do pipe principal

    int found = 0; //flag para indicar se o codigo de barras foi encontrado na base de dados

    for (int j = 0; j < 5; j++) {
        if (barcode == database_prod[j].barcode) { //se o codigo de barras for encontrado
            write(fd[index+1][1], &database_prod[j], sizeof(struct product)); //escreve as informacoes do produto no pipe entre o processo atual e o processo pai
            found = 1;
        }
    }

    if (found == 0) { //se o codigo de barras nao for encontrado
        printf("O produto com esse codigo de barras nao existe.\n");
    }
}
return 0; 
}
