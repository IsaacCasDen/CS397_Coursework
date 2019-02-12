#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_BUFFER=1024;

int main(int argc, char **argv) {

    if (argc>1) {
        for (int i=0; i<argc; i++) {

        }
    } else {
        char s[MAX_BUFFER];
        char c;

        while (fgets(s,MAX_BUFFER,stdin)) {
            //printf("Break 1\n");
            int pos=0;
            int lastPos=pos;
            for (pos;pos<MAX_BUFFER;pos++) {
                if (s[pos]=='\0') {
                    //printf("Break 1 - %d",pos);
                    lastPos=pos;
                    break;
                }
            }
            while (pos>=0) {
                if (s[pos]==' '||pos==0) {
                    //printf("Break 2 - %d",pos);
                    for (int i=pos;i<lastPos;i++) {
                        printf("%c",s[i]);
                        s[i]='\0';
                    }
                    
                    lastPos=pos;
                    
                } else if (pos==0) {
                    for (int i=pos;i<lastPos;i++) {
                        printf("%c",s[i]);
                        s[i]='\0';
                        printf(" ");
                    }
                }
                pos--;
            }
        }
        //printf("Break 3\n");
    }
    printf("\n");
    
    return 0;
}