
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>

int printFile(char *filename);

int main(int argc, char **argv) {
    char c;
    int result=0;

    if (argc==1) {
        while (scanf("%c",&c)!=EOF) {
            printf("%c",c);
        }
    } else if (argc>1) {
        for (int i=1; i<argc; i++) {
            int val = printFile(argv[i]);
            if (val!=0) {
                result=val;
            }
        }
    }
}

int printFile(char *fileName) {
    FILE *file;
    char c;

    file = fopen(fileName, "r");
    if (file) {
        while (fscanf(file,"%c",&c)!=EOF) {
            printf("%c",c);
        }
        printf("\n");
        return 0;
    } else {
        return 1;
    }
}