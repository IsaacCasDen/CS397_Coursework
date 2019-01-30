
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char input[100];
    while (scanf("%s",input) != EOF) {
        for (int i=0;i<strlen(input);i++) {
            printf("%c (%d),",input[i],input[i]);
        }
    }
    printf("%c",'\n');
