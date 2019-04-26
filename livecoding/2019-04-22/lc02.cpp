
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

pid_t pid;

void fx(int sig) {
    kill(pid,SIGINT);
}

int main(int argc, char **argv) {
    pid=atoi(argv[1]);
    signal(SIGTSTP,fx);
    

    while(true) pause();
}