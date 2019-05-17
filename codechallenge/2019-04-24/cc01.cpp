
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

bool run = true;

void fx(int sig) {
    run=false;
}

int main(int argc, char **argv) {
    
    if (argc<2) return -1;

    signal(SIGINT,fx);

    srand(0);
    int count = atoi(argv[1]);
    int *arr = (int *)malloc(sizeof(int)*count);
    for (int i = 0; i<count; i++) {
        arr[i]=rand();
        printf("%d\n",arr[i]);
    }

    while (run) {
        int pos = rand()%count;
        printf("%d\n",arr[pos]);
    }

    printf("Exiting\n");
    free(arr);

    return 0;

}