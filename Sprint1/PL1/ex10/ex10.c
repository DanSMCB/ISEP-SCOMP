#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

#define ARRAY_SIZE 2000

int main () {
  int numbers[ARRAY_SIZE];
  int i, j, status, n;
  pid_t p;
  time_t t;

  /* inicia RNG (srand():stdlib.h; time(): time.h) */
  srand((unsigned) time(&t));

  /* inicia array de numeros random (rand(): stdlib.h) */
  for(i = 0; i < ARRAY_SIZE; i++){
    numbers[i] = rand() % 1000;
  }
  /* inicia n */
  n = rand() % 1000;
  printf("Generated number: %d\n", n);

  for (i = 0; i < 10; i++) {
    p = fork();
    
    if (p < 0) {
      perror("Fork failed.\n");
      exit(-1);
    }

    if (p == 0) {
      p = getpid();
      for (j = 0; j < 200; j++) {
	if (numbers[j + i*200] == n) {
	  exit(j);
	}
      }
      exit(255);
    }
  }
  
  for (i = 0; i < 10; i++) {
    wait(&status);
    if (WEXITSTATUS(status) != 255 && WIFEXITED(status)) {
      int pos = WEXITSTATUS(status);
      printf("The child number is %d and it found the number %d at the position %d.\n", i, n, pos);
    }
  }
  return 0;
}