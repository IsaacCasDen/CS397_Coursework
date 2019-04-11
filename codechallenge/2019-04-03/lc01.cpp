
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


int main(int argc, char **argv) {

    int fd1[2];
    pipe(fd1);

    switch (fork()) {
        case 0:
            close(fd1[0]);
            printf("%d\n", fd1[1]);
            dup2(fd1[1],1);
            execlp("seq","seq","1000",NULL);
            break;
        default:
            int fd2[2];
            pipe(fd2);
            switch(fork()) {
                case 0:
                    dup2(fd1[0],0);
                    dup2(fd2[1],1);
                    close(fd1[1]);
                    close(fd2[0]);
                    execlp("grep","grep","4",NULL);
                    break;
                default:
                    switch (fork())
                    {
                        case 0:
                            dup2(fd2[0],0);
                            close(fd1[0]);
                            close(fd1[1]);
                            close(fd2[1]);
                            execlp("wc","wc","-l",NULL);
                            
                            break;
                    
                        default:
                            close(fd1[0]);
                            close(fd1[1]);
                            close(fd2[0]);
                            close(fd2[1]);
                            wait(NULL);
                            wait(NULL);
                            wait(NULL);
                            
                    }
            }
    }

}