#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc!=2) {
        fprintf(stderr,"usage: ./aout fileName\n");
        exit(1);
    }

    FILE *fptr;

    fptr = fopen(argv[1],w="w");

    fprintf(fptr,"Fred 42\n");
    fclose(fptr);

    fptr=fopen(argv[1],"r");
    char name[40];
    int age;

    fscanf(fptr, "%s",name);
    fscanf(fptr,"%d",&age);

    printf("read in: %s, %d\n", name, age);
}