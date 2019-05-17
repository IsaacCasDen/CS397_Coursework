2
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

int threadCount=0;

int constructSequence(int argc, char **argv);
int addSequence(char **commands, int count);
int execute();
void execute(char **commands, bool isReader, int* _pipe);
void execute(char **commands, int* _pipe);

int io[2];

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

    if (value>0) return execute();

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
int execute() {
    io[0]=STDOUT_FILENO;
    io[1]=STDOUT_FILENO;

    int _pipe[2];
    

    printf("------------\n\n");
    //printCommands();
    for (int i=0; i<seqCount; i++) {
        if (i==seqCount-1) {
            printf("\n---3.1(%d)---\n",i);
            execute(sequence[i],false,_pipe);
            printf("---3.2---\n");
        } else if (i==0) {
            printf("\n---1.1(%d)---\n",i);
            execute(sequence[i],true,_pipe);
            printf("---1.2---\n");
        } else {
            printf("\n---2.1(%d)---\n",i);
            printf("---2.2---\n");
        }
    }

    printf("4.\n");

    close(io[0]);
    close(io[1]);

    for (int i=0; i<threadCount; i++) {
        wait(NULL);
    }

    printf("5.\n");
    return 0;
}

void execute(char **commands, bool isReader, int* _pipe) {

    int fd[2];
    pipe(fd);
    
    if (!isReader) {
        switch(fork()) {
            case 0:
                threadCount++;
                //close(fd[1]);
                //dup2(io[1],io[0]);
                exit(execvp(commands[0],commands));
                break;
            default:
                wait(NULL);
                close(fd[0]);
                close(fd[1]);
        }
    } else {
        switch(fork()) {
            case 0:
                threadCount++;
                // close(fd[1]);
                // dup2(fd[0],0);
                exit(execvp(commands[0],commands));
                break;
            default:
                wait(NULL);
                close(fd[0]);
                close(fd[1]);
        }
    }
}
void execute(char **commands, int* _pipe) {
    int fd[2];
    pipe(fd);

    switch(fork()) {
        case 0:
            threadCount++;
            
            exit(execvp(commands[0],commands));
            break;
        default:
            wait(NULL);
            close(fd[0]);
            close(fd[1]);
    }

    
}