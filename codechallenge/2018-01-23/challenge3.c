#include <stdio.h>
#include <std

int main(int argc, char *argv[]) {

    FILE *fptr;

    fptr = fopen(argv[1],w="w");

    fprintf(fptr,"Fred 42\n");
}