#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int fd=open("test_lseek.txt",O_CREAT|O_WRONLY,S_IWUSR|S_IRUSR);
    write(fd, "hello!", 6);
    lseek(fd,10000,SEEK_CUR);
    write(fd,"goodbye!",8);
    close(fd);
}