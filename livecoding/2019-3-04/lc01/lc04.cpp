
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid=fork();
    switch (pid)
    {
        case 0:
            exit(0);
            break;
    
        default:
            while(1);
            break;
    }
}