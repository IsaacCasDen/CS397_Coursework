
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    //my_cp src dest

    if (argc!=3) {
        write(2,"Usage: my_cp src dest\n",15);
        exit(1);
    }

    int src, dest;

    src = open(argv[1],O_RDONLY); // should error check
    if (src==-1){
        perror("my_cp: ");
        exit(1);
    }

    dest=open(argv[2],
        O_WRONLY|O_CREAT|O_TRUNC,
        S_IWUSR|S_IRUSR);

    if (dest==-1){
        perror("my_cp: ");
        exit(1);
    }

    char c;
    while (read(src,&c,1)) {
        write(dest,&c,1);
    }

    close(src);
    close(dest);
}