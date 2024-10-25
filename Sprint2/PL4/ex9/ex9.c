#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <n> <time>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    unsigned int time = atoi(argv[2]);

    sem_t *p1 = sem_open("/sem_ex9", O_CREAT|O_EXCL, 0644, 0);

    struct timeval start, end;
    gettimeofday(&start, NULL); // Start time measurement

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            sem_wait(p1);
            usleep(time);
            sem_post(p1);
            exit(0);
        } else if (pid < 0) {
            printf("Fork failed.\n");
            return 1;
        }
    }

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    gettimeofday(&end, NULL); // End time measurement

    // Calculate execution time
    unsigned long long start_time = start.tv_sec * 1000000 + start.tv_usec;
    unsigned long long end_time = end.tv_sec * 1000000 + end.tv_usec;
    unsigned long long execution_time = end_time - start_time;

    printf("Execution time: %llu microseconds\n", execution_time);

	sem_close(p1);
    sem_unlink("/sem_ex09");

    return 0;
}
