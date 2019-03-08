
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

    if (argc<2) return 1;

    int count = atoi(argv[1]);
    int max = 1000;
    int min = 0;
    int pid=fork();

    switch (pid) {
        case 0:
            for (int i=0; i<count; i++) {
                int randVal = (rand()%(max-min+1))+min;
                printf("%d\n", randVal);
            }
            exit(0);
            break;

        default:
            printf("Parent waiting...\n");
            wait(NULL);
            printf("Child complete...\n");
            break;
    }

    return 0;

}