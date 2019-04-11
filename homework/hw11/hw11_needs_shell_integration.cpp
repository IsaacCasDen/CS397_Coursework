
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const int SIZE_CHARBUFFER = 50;

char ***sequence = (char ***) malloc(0);
int seqCount = 0;

int *seqComCount = (int *) malloc(0);
int comCount = 0;

int constructSequence(int argc, char **argv);
int addSequence(char **commands, int count);
int nextSegment(char *argv);

int main(int argc, char **argv) {

    int value = 0;

    value = constructSequence(argc,argv);

    return value;
}


int nextSegment(char *argv) {
    return 0;
}
int constructSequence(int argc, char **argv) {
    
    int j=0;

    int charRead = 0;

    char **commands = (char **) malloc(0);
    int comCount = 0;

    bool isString = false;
    bool isLiteral = false;

    char *command = (char *) malloc(0);
    int charPos = 0;

    for (int i=1; i<argc; i++) {
        int argPos = 0;
        while (argv[i][argPos]!='\0') {
            if (charPos%SIZE_CHARBUFFER==0) {
                command = (char *) realloc(command, sizeof(char) * (SIZE_CHARBUFFER + (charPos/SIZE_CHARBUFFER)));
            }
            switch (argv[i][argPos]) {
                case '\\':
                    isLiteral=!isLiteral;
                    break;
                case '"':
                    if (!isLiteral) {
                        isString=!isString;
                    }
                    break;
                case ' ':
                    if (!isLiteral) {
                        command[charPos='\0'];
                        commands = (char **) realloc(commands, sizeof(char *) * ++comCount);
                        commands[comCount-1]=strdup(command);
                        charPos=0;
                        continue;
                    }
                    break;
                case '|':
                    if (!isLiteral) {
                        command[charPos='\0'];
                        commands = (char **) realloc(commands, sizeof(char *) * ++comCount);
                        commands[comCount-1]=command;
                        charPos=0;

                        addSequence(commands, comCount);
                        continue;
                    }
                    break;
            }
            command[charPos++]=argv[i][argPos++];
        }

        if (!isString) {

            command[charPos='\0'];
            commands = (char **) realloc(commands, sizeof(char *) * ++comCount);
            commands[comCount-1]=strdup(command);
            charPos=0;

            for (int k=0; k<comCount; k++) {
                //printf("%s ",commands[k]);
            }
            //printf("\n");
            
        }
    }

    addSequence(commands, comCount);

    return charRead;

}
int addSequence(char **commands, int count) {
    sequence = (char ***) realloc(sequence, sizeof(char **) * ++seqCount);
    sequence[seqCount-1]=commands;

    seqComCount = (int *) realloc(seqComCount, sizeof(int) * ++comCount);
    seqComCount[comCount-1]=count;

    for (int i=0; i<count; i++) {
        printf("%s ",commands[i]);
    }
    printf("\n");

    return 0;
}