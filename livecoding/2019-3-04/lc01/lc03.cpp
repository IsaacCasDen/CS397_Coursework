
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid=fork();
    switch (pid)
    {
        case 0:
            printf("orphan\n");
            while(1);
            break;
    
        default:
            printf("child has pid: %d\n",pid);
            printf("parent exiting...\n");
            break;
    }
}