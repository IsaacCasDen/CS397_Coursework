
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid=fork();
    switch (pid)
    {
        case 0:
            printf("child!\n");
            exit(0);
            break;
    
        default:
            printf("parent: child has PID %d\n", pid);
            printf("parent: waiting...");
            wait(NULL);
            break;
    }

    return 0;
}