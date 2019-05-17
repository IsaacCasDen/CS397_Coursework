
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

const int SIZE_CHARBUFFER = 50;

char ***sequence = (char ***) malloc(0);
int seqCount = 0;

int *seqComCount = (int *) malloc(0);
int comCount = 0;

int **pipes;

int threadCount=0;

int constructSequence(int argc, char **argv);
int addSequence(char **commands, int count);
void execute();
void executeSingle();
void executeDouble();

void printCommands(char **array, int size) {
    printf("------\n");
    for (int i=0; i<size; i++) {
        printf("[%d]:\t%s\t",i,array[i]);
    }
    printf("\n------\n\n");
}
void printCommands() {
    int lenS = seqCount;
    printf("------\n");
    for (int i=0; i<seqCount; i++) {
        int lenC = seqComCount[i];
        for (int j=0; j<lenC; j++) {
            printf("[%d][%d]:%s\n",i,j,sequence[i][j]);
        }
    }
    printf("------\n\n");
}

int main(int argc, char **argv) {

    int value = 0;
    value = constructSequence(argc,argv);

    if (value>0) {
        execute();
    };

    return value;
}

int constructSequence(int argc, char **argv) {
    
    int charRead = 0;

    char **commands = (char **) malloc(0);
    int comCount = 0;

    char *command = (char *) malloc(0);
    int charPos = 0;

    for (int i=1; i<argc; i++) {
        int argPos = 0;
        while (argv[i][argPos]!='\0') {
            charRead++;
            if (charPos%SIZE_CHARBUFFER==0) {
                command = (char *) realloc(command, sizeof(char) * (SIZE_CHARBUFFER + (charPos/SIZE_CHARBUFFER)));
            }
            switch (argv[i][argPos]) {
                case '_':
                    command[charPos]='\0';
                    commands = (char **) realloc(commands, sizeof(char *) * ++comCount);
                    commands[comCount-1]=strdup(command);
                    charPos=0;
                    argPos++;
                    continue;
                case ' ':
                    command[charPos]='\0';
                    commands = (char **) realloc(commands, sizeof(char *) * ++comCount);
                    commands[comCount-1]=strdup(command);
                    charPos=0;
                    argPos++;

                    addSequence(commands, comCount);
                    comCount=0;
                    continue;
            }
            command[charPos++]=argv[i][argPos++];
        }
        command[charPos]='\0';
        commands = (char **) realloc(commands, sizeof(char *) * ++comCount);
        commands[comCount-1]=strdup(command);
        charPos=0;
        argPos++;
        addSequence(commands, comCount);
        comCount=0;
    }

    if (charPos>0) {
        addSequence(commands, comCount);
        //printCommands();
    }
    

    return charRead;

}
int addSequence(char **commands, int count) {

    if (count<1) return 0;
    //printCommands(commands,count);

    char **copy = (char **) malloc(sizeof(char *)*count);
    for (int i=0; i<count; i++) {
        int len = strlen(commands[i]);
        copy[i] = (char *) malloc(sizeof(char) * len); 
        for (int j=0; j<len; j++) {
            copy[i][j]=commands[i][j];
        }
    }

    sequence = (char ***) realloc(sequence, sizeof(char **) * (++seqCount));
    sequence[seqCount-1]=copy;

    seqComCount = (int *) realloc(seqComCount, sizeof(int) * (++comCount));
    seqComCount[comCount-1]=count;

    return 0;
}

void print(char *text) {
    printf("%s",text);
    fflush(stdout);
}
void print(int value) {
    printf("%d",value);
    fflush(stdout);
}
void print(char c) {
    printf("%c",c);
    fflush(stdout);
}

void closePipes(int **pipes, int maxIndex) {
    for (int i=0; i<=maxIndex; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}

void execute() {

    if (seqCount==1) {
        executeSingle();
        return;
    } else if (seqCount==2) {
        executeDouble();
        return;
    } 
    
    pipes = (int **) malloc(sizeof(int *) * seqCount);
    for (int seqId=0; seqId<seqCount; seqId++) {
        if (seqId==seqCount-1) {
            threadCount++;
            switch(fork()) {
                case 0:
                    dup2(pipes[seqId-1][0],0);
                    close(pipes[seqId-1][1]);
                    closePipes(pipes,seqId-1);
                    exit(execvp(sequence[seqId][0],sequence[seqId]));
                    // exit(execlp("echo","echo","test",NULL));
                    break;
                default:
                    closePipes(pipes,seqId-1);
                    for (int i=0; i<seqCount-1; i++) {
                        free(pipes[i]);
                    }
                    for (int i=0; i<threadCount; i++) {
                        wait(NULL);
                    }
                    continue;
            }
        } else if (seqId==0) {
            int *p = (int *) malloc(sizeof(int)*2);
            pipe(p);
            pipes[seqId]=p;
            threadCount++;
            switch(fork()) {
                case 0:
                    close(pipes[seqId][0]);
                    dup2(pipes[seqId][1],1);
                    exit(execvp(sequence[seqId][0],sequence[seqId]));
                    break;
                default:
                    continue;
            }
        } else {
            int *p = (int *) malloc(sizeof(int)*2);
            pipe(p);
            pipes[seqId]=p;
            threadCount++;
            switch(fork()) {
                case 0:
                    if(seqId%2==1) {
                        dup2(pipes[seqId-1][0],0);
                        dup2(pipes[seqId][1],1);
                        closePipes(pipes,seqId-2);
                        close(pipes[seqId][0]);
                        close(pipes[seqId-1][1]);
                    } else {
                        dup2(pipes[seqId-1][0],0);
                        dup2(pipes[seqId][1],1);
                        closePipes(pipes,seqId-2);
                        close(pipes[seqId-1][1]);
                        close(pipes[seqId][0]);
                    }
                    exit(execvp(sequence[seqId][0],sequence[seqId]));
                    // exit(execlp("echo","echo","100",NULL));
                    break;
                default:
                    continue;
            }

        
        }
    }
}

void executeSingle() {
    switch(fork()) {
        case 0:
            exit(execvp(sequence[0][0],sequence[0]));
            break;
        default:
            wait(NULL);
            return;
    }
}

void executeDouble() {
    int fd[2];
    pipe(fd);
    switch(fork()) {
        case 0:
            close(fd[0]);
            dup2(fd[1],1);
            exit(execvp(sequence[0][0],sequence[0]));
            break;
        default:
            switch(fork()) {
                case 0:
                    dup2(fd[0],0);
                    close(fd[1]);
                    exit(execvp(sequence[1][0],sequence[1]));
                    break;
                default:
                    close(fd[0]);
                    close(fd[1]);
                    wait(NULL);
                    wait(NULL);
                    return;
            }
    }
}