
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

const char *FILE_TAR = "-a";
const char *FILE_UNTAR = "-x";

struct filedata {

    size_t fileNameSize;
    size_t fileStatSize;
    size_t byteLength;
    
    char *fileName;
    struct stat *fileStat;
    char *bytes;
    
};

int tarFiles(char *filepath, int argc, char **argv);

int main(int argc, char **argv) {

    if (argc>2) {
        if (strcmp(argv[1],FILE_TAR)==0 && argc>3) {
            tarFiles(argv[2],argc-3,argv+(3*sizeof(char)));
        } else if (strcmp(argv[1],FILE_UNTAR)==0) {
            //untarFiles(argv[2]);
        } else {
            return -1;
        }
    } else {
        return 0;
    }

    
}

int tarFiles(char *filepath, int argc, char **argv) {

    char *filestream;
    int fileCount=0;
    
    for (int i=0; i<argc; i++) {
        struct stat *statdata = (struct stat *) malloc(sizeof(struct stat));
        stat(argv[i],statdata);

        if (S_ISREG(statdata->st_mode))
            fileCount++;

        free(statdata);
    }

    FILE *outFile = fopen(filepath,"wb");
    fwrite(&fileCount,sizeof(int),1,outFile);
    fileCount=0;

    for (int i=0; i<argc; i++) {
        struct stat *statdata = (struct stat *) malloc(sizeof(struct stat));
        stat(argv[i],statdata);

        if (!S_ISREG(statdata->st_mode))
            continue;

        FILE *inFile = fopen(argv[i],"rb");
        struct filedata *filedata = (struct filedata *) malloc(sizeof(struct filedata));

        filedata->fileNameSize=sizeof(*argv[i]);
        filedata->fileName=argv[i];

        filedata->fileStatSize=sizeof(*statdata);
        filedata->fileStat=statdata;

        filedata->byteLength=statdata->st_size;
        filedata->bytes=(char *) malloc(filedata->byteLength*sizeof(char));
        fread(filedata->bytes,statdata->st_size,1,inFile);
        fwrite(filedata,sizeof(struct filedata),1,outFile);
    }

    return 0;
}

/*
int untarFiles(char *filepath) {

}

int addFile(char *filename) {

}
int extractFile() {

}
*/