
#include <unistd.h>
#include <stdio.h>

const int BLOCK_SIZE = 1024;

struct Node {
    struct Node *prev=NULL;
    struct Node *next=NULL;
    size_t size=0;
};

const int NODE_SIZE = sizeof(struct Node);

void *mmalloc(size_t size);
void mfree(void *ptr);
unsigned long heapSize();
unsigned long findOffset(void * ptr, int size);
struct Node * split(Node * node, size_t size);
struct Node * consume(Node * node, size_t size);
void printFreeList();
unsigned long getPosition(void * ptr);
unsigned long getRelPosition(void * ptr);

unsigned long progStart = getPosition(sbrk(0));
Node *heapStart = NULL;

int main() {
    printf("main: starting main()\n");
    printFreeList();

    printf("main: testing imperfect match...\n");
    void *a1 = mmalloc(10);
    printFreeList();
    

    printf("main: testing perfect match...\n");
    void *a2 = mmalloc(BLOCK_SIZE-NODE_SIZE);
    printFreeList();

    printf("main: free perfect match...\n");
    mfree(a2);
    printFreeList();

    printf("main: testing imperfect match...\n");
    void *a3 = mmalloc(50);
    printFreeList();
}

unsigned long getPosition(void * ptr) {
    return (unsigned long)ptr;
}
unsigned long getRelPosition(void * ptr) {
    return (getPosition(ptr)-progStart);
}
unsigned long heapSize() {
    return getPosition(sbrk(0))-progStart;
}
unsigned long findOffset(void * ptr, int size) {
    return getPosition(ptr)+size;
}
struct Node * consume(Node * node, size_t size) {
    if (node->size==size) {
        printf("node at %lu has enough space %zu\n", getRelPosition(node), node->size);
        printf("releasing %lu (size=%zu) from free list\n",getRelPosition(node),node->size);
        if (node==heapStart) {
            if (node->next!=NULL) {
                heapStart=heapStart->next;
            } else if (node->prev!=NULL) {
                heapStart=heapStart->prev;
            } else {
                heapStart=NULL;
            }
        }
        if (node->prev!=NULL&&node->next!=NULL) {
            node->prev->next=node->next;
            node->next->prev=node->prev;
        } else if (node->prev!=NULL) {
            node->prev->next=NULL;
        } else if (node->next!=NULL) {
            node->next->prev=NULL;
        }
    } else {
        _exit(1);
    }
    return node;
}

struct Node * split(Node * node, size_t size) {
    if (node->size<size) {
        _exit(1);
    } else if (node->size>size) {
        printf("mmalloc: node at %lu has enough space (%zu)\n",getRelPosition(node),node->size);
        Node *freeNode = (struct Node *)findOffset(node,size+NODE_SIZE);
        freeNode->size=node->size-size-NODE_SIZE;
        printf("splitting %lu (%zu) into %lu (%zu) and %lu (%zu)\n", 
            getRelPosition(node), node->size,
            getRelPosition(node), size,
            getRelPosition(freeNode),freeNode->size);
        
        node->size=size;
        
        if (heapStart==NULL||heapStart==node) {
            heapStart=freeNode;
        }
        if (node->prev!=NULL&&node->next!=NULL) {
            freeNode->prev=node->prev;
            freeNode->next=node->next;

            freeNode->prev->next=freeNode;
            freeNode->next->prev=freeNode;
        } else if (node->prev!=NULL) {
            freeNode->prev=node->prev;
            freeNode->prev->next=freeNode;
        } else if (node->next!=NULL) {
            freeNode->next=node->next;
            freeNode->next->prev=freeNode;
        }
        return node;
    } else {
        return consume(node,size);
    }
}

void *mmalloc(size_t size) {
    printf("mmalloc: requesting %d bytes\n",size);
    struct Node *node;
    void * mem;
    if (size<=0||size>BLOCK_SIZE-NODE_SIZE) {
        _exit(1);
    } else if (heapStart==NULL) {
        printf("mmalloc: calling sbrk..\n");
        mem = sbrk(BLOCK_SIZE);
        node = (struct Node *)mem;
        node->size=BLOCK_SIZE-NODE_SIZE;
        if (node->size==size) {
            node=consume(node,size);
        } else {
            node = split(node,size);
        }
        
    } else {
        node = heapStart;
        if (heapStart->size==size) {
            node=consume(node,size);
        //} else if (heapStart->size>size+NODE_SIZE) {
              
        } else {
            bool createNode = false;
            node = heapStart;
            if (node->size<size) {
                createNode=true;
                while (node->next!=NULL) {
                    if (node->next->size>=size) {
                        node=node->next;
                        createNode=false;
                        break;
                    } else {
                        node=node->next;
                    }
                }
            }
            
            Node * currNode;
            if (createNode) {
                printf("mmalloc calling sbrk..\n");
                currNode = (struct Node *)sbrk(BLOCK_SIZE);
                currNode->size=BLOCK_SIZE-NODE_SIZE;
                currNode->prev=node;
                currNode->next=node->next;
                node->next=currNode;
            } else {
                currNode=node;
            }

            if (currNode->size>size) {
                //printf("\n\n\n");
                //printf("mmalloc: splitting %d (%d)",getRelPosition(node), node->size);
                node=split(node,size);
            } else {
                node=consume(currNode,size);
            }
        }
    }
    printf("mmaloc: returning %lu\n",getRelPosition(node));
    return (void *)(getPosition(node)+NODE_SIZE);
}
void mfree(void *ptr) {
    Node * node = (struct Node *)findOffset(ptr,-NODE_SIZE);
    //printf("Pointer %d, Node %d, size: %d\n", getRelPosition(ptr),getRelPosition(node),node->size);
    if (heapStart==NULL) {
        if (node->prev!=NULL&&node->next!=NULL) {
            node->prev->next=node->next;
            node->next->prev=node->prev;
        } else if (node->prev!=NULL) {
            node->prev->next=NULL;
        } else if (node->next!=NULL) {
            node->next->prev=NULL;
        } 
        heapStart=node;
    } else {
        Node * currNode = heapStart;
        while (currNode->next!=NULL) currNode=currNode->next;
        currNode->next=node;
        node->prev=currNode;
    }
    printf("mfree: node %lu (size %d)\n",getRelPosition(node),node->size);
}

void printFreeList() {
    printf("=== FREE LIST ===\n");
    if (heapStart!=NULL) {
        unsigned long prev=0;
        unsigned long next=0;
        Node *node = heapStart;
        if (node->prev==NULL) prev=-1; else prev = getRelPosition(node->prev);
        if (node->next==NULL) next=-1; else next = getRelPosition(node->next);

        printf("%lu: (%lu,%lu,%zu)\n",getRelPosition(node),prev,next,node->size);
        while ((node=node->next)!=NULL) {
            if (node->prev==NULL) prev=-1; else prev = getRelPosition(node->prev);
            if (node->next==NULL) next=-1; else next = getRelPosition(node->next);
            printf("%lu: (%lu,%lu,%zu)\n",getRelPosition(node),prev,next,node->size);
        }
    }
    printf("=== END       ===\n");
}