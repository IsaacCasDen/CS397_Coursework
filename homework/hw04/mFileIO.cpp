#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

const int MAX_READ = 1;


struct mFile
{
    int fileDescriptor;
};

mFile *mfopen(const char *filename, const char *mode);
int mfread(void *ptr, size_t size, size_t nitems, mFile *fptr);
int mfwrite(void *ptr, size_t size, size_t nitems, mFile *fptr);
int mfclose(mFile *fptr);

int main(int argc, char **argv) {

    char *inputFilename = strdup("input.txt");
    char *outputFilename = strdup("output.txt");

    mFile *inFile;
    mFile *outFile;
    
    char *data= (char *) malloc(MAX_READ*sizeof(char));

    if (argc>1) {
        inputFilename = argv[1];
        if (argc>2) {
            outputFilename = argv[2];
        }
    } 

    inFile = mfopen(inputFilename,"r");
    
    if (inFile->fileDescriptor==-1) {
        printf("Usage: mFileIO [input filename] [output filename]");
    } else {
        outFile = mfopen(outputFilename,"w");

        if (outFile->fileDescriptor!=-1) {
            while (mfread(data,sizeof(char),MAX_READ,inFile)) {
                mfwrite(data,sizeof(char),MAX_READ,outFile);
            }

            mfclose(outFile);
        }

        mfclose(inFile);

    }
    
    free(data);
    free(inFile);
    free(outFile);
    free(inputFilename);
    free(outputFilename);

}

mFile *mfopen(const char *filename, const char *mode) {
    mFile *value = (mFile *)malloc(sizeof(mFile));

    mode_t m = 
        (strcmp(mode,"r")==0)?O_RDONLY:
        (strcmp(mode,"w")==0)?O_WRONLY | O_CREAT | O_TRUNC:
        (strcmp(mode,"a")==0)?O_WRONLY | O_CREAT | O_APPEND:
        (strcmp(mode,"r+")==0)?O_RDWR:
        (strcmp(mode,"w+")==0)?O_RDWR | O_CREAT | O_TRUNC:
        (strcmp(mode,"a+")==0)?O_RDWR | O_CREAT | O_APPEND:0;

    value->fileDescriptor = open(filename, m);

    return value;        
}

int mfread(void *ptr, size_t size, size_t nitems, mFile *fptr) {
    
    void *buffer = malloc(size*nitems);
    size_t result = read(fptr->fileDescriptor,ptr,size*nitems);
    //ptr=buffer;
    return result;

}

int mfwrite(void *ptr, size_t size, size_t nitems, mFile *fptr) {

    size_t result = write(fptr->fileDescriptor,ptr,size*nitems);

    return result;
}

int mfclose(mFile *fptr) {

    if (close(fptr->fileDescriptor)==-1)
        perror("Error closing file");

    return 0;
}