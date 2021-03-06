#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

const char *FILE_TAR = "-a";
const char *FILE_UNTAR = "-x";

struct filedata {

    int fileNameSize;
    char *fileName;
    
    int fileStatSize;
    struct stat fileStat;
    
    int byteLength;
    char *bytes;
};
    

int tarFiles(char *filepath, int argc, char **argv);
int untarFiles(char *filepath);

int writeStruct(FILE *file, struct filedata *inStruct);
int readStruct(FILE *file, struct filedata *outStruct);

int main(int argc, char **argv) {
    
    if (argc>2) {
        if (strcmp(argv[1],FILE_TAR)==0 && argc>3) {
            tarFiles(argv[2],argc-3,argv+(3*sizeof(char)));
        } else if (strcmp(argv[1],FILE_UNTAR)==0) {
            untarFiles(argv[2]);
        } else {
            return -1;
        }
    } else {
        return 0;
    }

    
}

int tarFiles(char *filepath, int argc, char **argv) {
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

        filedata->fileNameSize=strlen(argv[i]);
        filedata->fileName=strdup(argv[i]);

        //printf("Filename: %s\n",filedata->fileName);
        printf("Filename Size: %d\n",filedata->fileNameSize);

        filedata->fileStatSize=sizeof(*statdata);
        filedata->fileStat=statdata;

        filedata->byteLength=statdata->st_size;
        //filedata->bytes=(char *) malloc(filedata->byteLength*sizeof(char));
        //fread(filedata->bytes,filedata->byteLength,1,inFile);
        writeStruct(outFile,filedata);
        fclose(inFile);
    }
    
    fclose(outFile);

    return 0;
}

int untarFiles(char *filepath) {
    int fileCount=0;

    FILE *inFile = fopen(filepath, "rb");
    fread(&fileCount,sizeof(int),1,inFile);

    struct filedata **files = (struct filedata **) malloc(fileCount*sizeof(struct filedata));
    for (int i=0; i<fileCount; i++) {
        files[i]=(struct filedata *) malloc(sizeof(struct filedata));
        readStruct(inFile,files[i]);

        printf("File found: \n");
        printf("Size: %d\n",files[i]->byteLength);
        printf("Name Length: %d\n",files[i]->fileNameSize);
        printf("Name: %s\n",files[i]->fileName);
        printf("Date: %s",files[i]->fileStat->st_ctim);
        
    }
    

/*
    for (int i=0; i<fileCount; i++) {
        printf("Reading file: %d\n",i);
        struct filedata *filedata = (struct filedata *) malloc(sizeof(struct filedata));
        readStruct(inFile,filedata);
        
        
        //printf("Byte Length: %d\n",filedata->byteLength);
        //printf("Bytes: %c\n", filedata->bytes);
        //printf("Stat Size: %d\n", filedata->fileStatSize);
    }
*/
    fclose(inFile);

    return 0;
}

int writeStruct(FILE *file, struct filedata *inStruct) {

    fwrite(inStruct,sizeof(struct filedata),1,file);
    
    return 0;
}

int readStruct(FILE *file, struct filedata *outStruct) {

    fread(outStruct,sizeof(struct filedata),1,file);

    return 0;
}