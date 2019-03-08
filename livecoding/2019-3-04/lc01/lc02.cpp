
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("hey!\n");
    fork();
    printf("hey!\n");
    fork();
    printf("hey!\n");
    fork();
    printf("hey!\n");
}