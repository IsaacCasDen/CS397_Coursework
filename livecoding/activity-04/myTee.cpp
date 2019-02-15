#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int *fds = (int *) malloc((argc-1)*sizeof(int));

    for (int i=0; i<argc-1; i++) {
        fds[i]=open(argv[i+1],
        O_WRONLY | O_CREAT | O_TRUNC,
        S_IRUSR | S_IWUSR);
    }

    char c;
    while(read(0,&c,1)!=0) {
        write(1,&c,1);
        for (int i=0;i<argc-1;i++) {
            write(fds[i],&c,1);
        }
    }

    for (int i=0; i<argc-1;i++) {
        close(fds[i]);
    }

    free(fds);
}