#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MAX_THREADS (1 << 24)

void printer(int *arg) {
    printf("in %d\n", *arg);
    free(arg);
}

pthread_t threads[MAX_THREADS] = {0};

int main()
{
    int i;
    int created_threads = 0;

    for (i = 0; i < MAX_THREADS; i++) {
        int *tmp = malloc(sizeof(*tmp));
        if (tmp != NULL) {
            *tmp = i;
        } else {
            fprintf(stderr, "Not enough memory to spawn thread %d\n", i);
        }
        if (pthread_create(threads + i, NULL, (void *(*)(void *))printer, tmp)) {
            fprintf(stderr, "Thread %d did not spawn: %s\n", i, strerror(errno));
        } else {
            created_threads++;
        }
    }
    for (i = 0; i < created_threads; i++) {
        printf("Joining %d\n", i);
        pthread_join(threads[i], NULL);
    }
    printf("Spawned %d threads.\n", created_threads);

    return 0;
}
