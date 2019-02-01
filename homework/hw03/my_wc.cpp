
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

int * readFile(char *filename);
void printResult(char * filename, int * values);
bool validateWord(char c, bool *hasPrev, bool *prevIsChar, bool *prevIsSpace, bool *isNewLine);

int main(int argc, char **argv) {

    if (argc==1) {

    } else {
        for (int i=1; i<argc; i++) {
            int * val = readFile(argv[i]);
            if (val!=NULL) {
                printResult(argv[i],val);
            } else {
                printf("Error reading file %s\n",argv[i]);
            }
            //free(val);
        }
    }
    return 0;
}

int * readFile(char *filename) {
    bool success=false;;
    FILE *file;
    int size = 4;
    int *value = (int *)malloc(sizeof(int)*size);
    char c;
    bool hasPrev=false;
    bool prevIsChar=false;
    bool prevIsSpace=false;
    bool isNewLine;

    for (int i=0; i<size; i++)
        value[i]=0;

    file = fopen(filename, "r");
    if (file) {
        success=true;;
        while (fscanf(file,"%c",&c)!=EOF) {
            isNewLine=false;
            //printf("%c",c);
            
            if (validateWord(c,&hasPrev,&prevIsChar,&prevIsSpace,&isNewLine)) {
                value[0]+=1;
            }
            if (prevIsChar) {
                    value[1]++;
            }
            if (isNewLine) {
                value[3]++;
            }
            value[2]+=sizeof(char);
        }
        fclose(file);
    } else {
       // errno=1;
    }

    //free(file);

    if (success)
        return value;
    else {
        return NULL;
    }
}

void printResult(char * filename, int * values) {
    printf("The file %s contains %d words", filename,values[0]);
    printf(", %d characters",values[1]);
    printf(", %d bytes",values[2]);
    printf(", %d line%c\n", values[3],(values[3]>1)?'s':' ');
}

bool validateWord(char c, bool *hasPrev, bool *prevIsChar, bool *prevIsSpace, bool *isNewLine) {
    bool value = false;
    
    if (c==' ') {
        if (!*hasPrev) {
            *isNewLine=true;
            *hasPrev=true;
        }
        *prevIsSpace=true;
        *prevIsChar=false;
    } else if (c=='\n') {
        *hasPrev=false;
        *prevIsSpace=false;
        *prevIsChar=false;
    } else if (c>='!'&&c<='~') {
        if (!*hasPrev) {
            *isNewLine=true;
            value=true;
        } else if (*prevIsSpace) {
            value=true;
        }
        *hasPrev=true;
        *prevIsSpace=false;
        *prevIsChar=true;
    } else {
        *hasPrev=true;
        *prevIsSpace=false;
        *prevIsChar=true;
    }
    

    return value;
}
