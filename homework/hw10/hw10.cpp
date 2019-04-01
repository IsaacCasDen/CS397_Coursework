
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

//using namespace std;

char *FILENAME_INPUT = strdup("in=");
const int FILENAME_INPUT_LEN = 3;
char *FILENAME_OUTPUT = strdup("out=");
const int FILENAME_OUTPUT_LEN = 4;
char *FILENAME_ERROR = strdup("err=");
const int FILENAME_ERROR_LEN = 4;
char *OUTPUT_APPEND = strdup("-append");
const int OUTPUT_APPEND_LEN = 7;

bool appendOutput = false;
char *inputFilename;
bool inputFileSet=false;
char *outputFilename;
bool outputFileSet=false;
char *errorFilename;
bool errorFileSet=false;

void parseArguments(int arg, char **argv);
bool checkArgument(char *arg, char *ARG_CONST, int ARG_LENGTH);
int parseArgument(char *arg, char seperator, char *output);

int main(int argc, char **argv) {

    parseArguments(argc,argv);
    if (!errorFileSet) dup2(STDOUT_FILENO, STDERR_FILENO);
        
    char text[1024];
    while (fgets(text,1024,stdin)!=NULL) {
        fprintf(stdout,"stdout: ");
        fprintf(stdout,text);
        fprintf(stderr,"stderr: ");
        fprintf(stderr,text);
    }
}

void parseArguments(int argc, char **argv) {
    for (int i=1; i<argc; i++) 
        if (strcmp(argv[i],OUTPUT_APPEND)==0) {
            appendOutput=true;
            break;
        }
    for (int i=1; i<argc; i++) {
        if (checkArgument(argv[i],FILENAME_INPUT,FILENAME_INPUT_LEN)) {
            inputFilename = (char *) malloc(sizeof(char) * strlen(argv[i]));
            parseArgument(argv[i],'=',inputFilename);
            dup2(fopen(inputFilename,"r")->_fileno,STDIN_FILENO);
            inputFileSet=true;
        } else if (checkArgument(argv[i],FILENAME_OUTPUT,FILENAME_OUTPUT_LEN)) {
            outputFilename = (char *) malloc(sizeof(char) * strlen(argv[i]));
            parseArgument(argv[i],'=',outputFilename);
            //printf("%s->%s\n",argv[i],outputFilename);
            if (appendOutput)
                dup2(fopen(outputFilename,"a+")->_fileno,STDOUT_FILENO);
            else 
                dup2(fopen(outputFilename,"w")->_fileno,STDOUT_FILENO);
            outputFileSet=true;
        } else if (checkArgument(argv[i],FILENAME_ERROR,FILENAME_ERROR_LEN)) {
            errorFilename = (char *) malloc(sizeof(char) * strlen(argv[i]));
            parseArgument(argv[i],'=',errorFilename);
            //printf("%s->%s\n",argv[i],errorFilename);
            if (appendOutput)
                dup2(fopen(errorFilename,"a+")->_fileno,STDERR_FILENO);
            else
                dup2(fopen(errorFilename,"w")->_fileno,STDERR_FILENO);
            errorFileSet=true;
        }
    }
}

bool checkArgument(char *arg, char *ARG_CONST, int ARG_LENGTH) {
    for (int i=0; i<ARG_LENGTH; i++) {
        if (arg[i]!=ARG_CONST[i]) 
            return false;
    }
    return true;
}
int parseArgument(char *arg, char seperator, char *output) {
    int startPos, length = strlen(arg);

    for (startPos=0; startPos<length; startPos++) if (arg[startPos]==seperator) break;
    startPos++;

    if (length>startPos) {
        int i;
        for (i = startPos; i<length; i++)
            output[i-startPos]=arg[i];
        output[i]='\0';
    }

    return (length-startPos);
}