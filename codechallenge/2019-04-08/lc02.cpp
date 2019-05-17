
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Person {
    int age;
    char c;    
};

void *run(void *v) {
    Person *person = (Person*)v;
    printf("%d %d %c\n",pthread_self(),person->age,person->c);
    return NULL;
}

int main() {
    pthread_t threads[100];
    Person persons[100];

    for (int i=0; i<100; i++) {
        persons[i].age=i;
        persons[i].c = '!';
        pthread_create(&threads[i],NULL,run,(void *) &persons[i]);
    }

    for (int i=0; i<100; i++) {
        pthread_join(threads[i],NULL);
    }
}