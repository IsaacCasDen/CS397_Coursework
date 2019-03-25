
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sys/wait.h>

const char* CONST_EXIT = strdup("exit");
const int CONST_EXIT_LEN = 4;
const char *CONST_EXIT_ERR = strdup("130");
const int CONST_EXIT_ERR_LEN = 3;

int BUFFER_SIZE = 1025;
bool verbose = false;

int startShell();
int clearBuffer(char *buffer, int size);
int splitCmdArgs(char *buffer, char *command, char *args);

int main(int argc, char **argv) {
    
    if (argc>1) {
        verbose = argv[1]!="0";
    } 
    
    int shell = startShell();

    return shell;
}

int startShell() {

    bool shellActive = true;
    int returnValue = 0;

    do {
        char buffer[BUFFER_SIZE];
        char command[BUFFER_SIZE];
        char args[BUFFER_SIZE];

        clearBuffer(buffer,BUFFER_SIZE);
        clearBuffer(command,BUFFER_SIZE);
        clearBuffer(args,BUFFER_SIZE);

        printf("MY_UNIX> ");
        
        if (fgets(buffer,BUFFER_SIZE,stdin)==NULL) {
            char *com = (char *) malloc(sizeof(char)*10);
            strcat(com,CONST_EXIT);
            strcat(com," ");
            strcat(com,CONST_EXIT_ERR);

            int i=0;
            for (i; i<CONST_EXIT_LEN + CONST_EXIT_ERR_LEN +1 && i<BUFFER_SIZE; i++) {
                buffer[i]=com[i];
            }
            buffer[i]='\0';
            printf("%s\n",args);
            free(com);
        }
        //scanf(" %1024[^\n]",buffer);
        if (verbose) {
            printf("Buffer: %s\n",buffer);
            sleep(1);
        }
        
        int split = splitCmdArgs(buffer,command,args);
            
        if (split==0) {

            if (strcmp(command,CONST_EXIT)==0) {
                if (strcmp(args,"")!=0) {
                    returnValue = atoi(args);
                }
                shellActive=false;
            } else {
                switch (fork()) {
                    case 0:
                        if (strcmp(args,"")==0) {
                            if (execlp(command,command,NULL)==-1) {
                                printf("ERROR: could not exec %s\n",command);
                                perror("ERROR");
                            }
                        } else {
                            if (execlp(command,command,args,NULL)==-1) {
                                printf("ERROR: could not exec %s\n",command);
                                perror("ERROR");
                            }
                        }
                        break;
                    default:
                        wait(NULL);
                }
            }
        }
    } while (shellActive);

    return returnValue;
}

int clearBuffer(char *buffer, int size) {
    for (int i=0; i<size; i++) buffer[i]='\0';
    return 0;
}

int splitCmdArgs(char *buffer, char *command, char *args) {

    int commandPos=0;
    while (buffer[commandPos]!=' '&&buffer[commandPos]!='\0'&&buffer[commandPos]!='\n'&&commandPos<BUFFER_SIZE) {
        command[commandPos] = buffer[commandPos];
        commandPos++;
    }
    command[commandPos++]='\0';
    if (verbose) {
            printf("Command: %d to %d; %s",0,commandPos-2,command);
            printf("\n");
            sleep(1);
        }
    
    int argPos = commandPos;
    while (buffer[argPos]!='\n'&&buffer[argPos]!='\0'&&argPos<BUFFER_SIZE) {
        args[argPos-commandPos]=buffer[argPos];
        argPos++;
    }
    args[(argPos-commandPos)]='\0';
    if (argPos>commandPos && verbose) {
            printf("Args: %d to %d; %s",commandPos,commandPos + (argPos-commandPos)-1,args);
            printf("\n");
            sleep(1);
        }
    

    return 0;
}
