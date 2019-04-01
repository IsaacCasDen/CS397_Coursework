#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
    
    if (argc>1)
        dup2(fopen(argv[1],"r")->_fileno,STDIN_FILENO);
    
    char c;
    while (fread(&c, sizeof(char),1,stdin)!=0)
        printf("%c",c);

}