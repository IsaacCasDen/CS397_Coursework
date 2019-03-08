
#include <stdio.h>
#include <unistd.h>

struct Node {
    int x;
    char c;
};

int main() {
    void *startOfHeap = sbrk(256);

    Node *node1 = (Node *) ((unsigned long)startOfHeap);
    node1->x=42;
    node1->c ='F';
    printf("%d %c\n", node1->x, node1->c);
    
    Node *node2=(Node *) ((unsigned long)startOfHeap+sizeof(Node));
    node2->x=43;
    node2->c='G';
    printf("%d %c\n", node2->x, node2->c);
}

/*

void *mall(int size);

struct pair {
    int key;
    char val;
};

size_t addrSize=0;
void *addrPtr;

int main() {
    
    struct pair *p;



}

void *mall(size_t size) {
    if (addrSize<size) {
        size_t alloc=size;
        if (alloc<256) {
            alloc=256;
        }
        
        addrSize = alloc;
        addrPtr = sbrk(alloc);
    }

    addrSize-=size;
    addrPtr+=size+sizeof(__u_long);

    return addrPtr;

}

*/