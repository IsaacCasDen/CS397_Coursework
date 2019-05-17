

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

struct Thread {
    int thread;
    int value;
};

void *work(void *value) {
    Thread *val = (Thread *)value;

    printf("Thread: %d, Value: %d\n",val->thread,val->value);
    return NULL;
}

int start(int maxCount, int threadCount);

int main(int argc, char **argv) {
    
    if (argc!=3) return -1;

    return start(atoi(argv[1]),atoi(argv[2]));

}

int start(int maxCount, int threadCount) {
    printf("Max Count: %d, Max Threads: %d\n",maxCount,threadCount);

    pthread_t *threads = (pthread_t *) malloc((sizeof(pthread_t)*threadCount)) ;
    Thread *values = (Thread *) malloc(sizeof(Thread)*maxCount);

    int threadIndex=0;
    int value=0;
    for (int i=0; i<threadCount; i++) {
        for (int j=0; j<maxCount/threadCount; j++) {
            values[value].thread=i;
            values[value].value=value;
            pthread_create(&threads[i], NULL, work, (void *)&values[value]);
            value++;
        }
    }

    for (int i=0; i<threadCount; i++) {
        pthread_join(threads[i],NULL);
    }

    free(values);
    free(threads);

    return 0;

}

