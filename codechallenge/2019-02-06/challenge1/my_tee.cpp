
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

std::vector<char *> text;

const char *STRING_USAGE = "usage my_tee file [ file ... ]";

int main(int argc, char **argv) {

    if (argc==1) {
        write(1,STRING_USAGE,sizeof(char)*strlen(STRING_USAGE));
        write(1,"\n",1);
    } else {
        while ()
    }
}

int open(const char *pathname, int flags, mode_t mode) {
    
}

ssize_t read(int fd, void *buf, size_t count) {

}

ssize_t write(int fd, const void *buf, size_t count) {

}

int close(int fd) {

}
