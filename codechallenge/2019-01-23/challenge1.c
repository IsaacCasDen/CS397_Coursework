
#include <stdio.h>

int main() {
    char name[100];
    int age;

    printf("Enter Name: ");
    scanf("%s",name);

    printf("Enter Age: ");
    scanf("%d",&age);

    printf("%s is %d years old\n", name, age);
}