
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *run(void *v) {
    sleep(1);
    printf("%d hello world!!\n",pthread_self);

    return NULL;
}

int main(int argc, char **argv) {

    int count = 10000;

    if (argc>1) {
        count = atoi(argv[1]);
    }
    
    pthread_t threads[count];
    for (int i=0; i<count; i++) {
        pthread_create(&threads[i], NULL, run, NULL);
    }

    for (int i=0; i<count; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}