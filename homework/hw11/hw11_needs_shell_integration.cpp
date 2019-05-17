
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int BUFFER_SIZE = 50;
char **fullCommand = (char **)malloc(sizeof(char)*BUFFER_SIZE);

int readArgs(int argc, char **argv);

int main(int argc, char **argv) {

    int value = 0;

    value = readArgs(argc,argv);

    return value;
}

int readArgs(int argc, char **argv) {

    int charRead = -1;
    int comPos = 0;
    int comLength=BUFFER_SIZE;
    for (int command=1; command<argc; command++) {
        int pos=0;
        while (argv[command][pos]!='\0') {
            if ()
            printf("%d:%c",pos,argv[command][pos]);
            pos++;
            printf("\n");
            sleep(1);
        }
    }
    

    return charRead;

}