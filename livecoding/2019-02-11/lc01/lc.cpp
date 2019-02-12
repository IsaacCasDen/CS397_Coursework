#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    struct stat sb;

    for (int i=1; i<argc; i++) {
        stat(argv[i], &sb);
        printf("%s %c%c%c%c%c%c%c%c%c%c %lld %lu %lu",argv[i],
            S_ISREG(sb.st_mode)?'-':'?',
            (sb.st_mode&S_IRUSR)?'r':'-',
            (sb.st_mode&S_IWUSR)?'w':'-',
            (sb.st_mode&S_IXUSR)?'x':'-',
            (sb.st_mode&S_IRGRP)?'r':'-',
            (sb.st_mode&S_IWGRP)?'w':'-',
            (sb.st_mode&S_IXGRP)?'x':'-',
            (sb.st_mode&S_IROTH)?'r':'-',
            (sb.st_mode&S_IWOTH)?'w':'-',
            (sb.st_mode&S_IXOTH)?'x':'-',
            sb.st_size,
            sb.st_mtim,
            sb.st_atim
            );

        printf("\n");
    }
}