
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void fx(int sig) {
    printf("Hahahaha\n");
}

int main(int argc, char **argv) {
    signal(SIGINT,fx);
    while(true) pause();
}