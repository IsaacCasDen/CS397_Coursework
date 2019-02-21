
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int write_ver1();
int read_ver1();

int write();
int writeStruct(FILE *file, struct filetest *inStruct);
int read();
int readStruct(FILE *file, struct filetest *outStruct);

const char *FILE_NAME = "test.bin";

struct filetest {

    int arraySize;
    int *array;

};

int main(int argc, char **argv) {
    
    write();
    read();

}

int write_ver1() {
    printf("Write Start\n");
    FILE *file = fopen(FILE_NAME,"wb");

    int j = 10;
    int *k = (int *) malloc(j*sizeof(int));
    for (int i = 0; i<j; i++) {
        k[i]=i;
    }

    fwrite(&j,sizeof(int),1,file);
    fwrite(k,sizeof(int),j,file);
    fclose(file);
    printf("Write End\n");
    return 0;
}

int read_ver1() {
    printf("Read Start\n");
    FILE *file = fopen(FILE_NAME,"rb");

    int j;
    int *k;

    fread(&j,sizeof(int),1,file);
    printf("%d\n",j);
    k = (int *) malloc(j*sizeof(int));
    fread(k,sizeof(int),j,file);
    fclose(file);

    printf("Read End\n");

    for (int i=0; i<j; i++) {
        printf("%d\n",k[i]);
    }

    return 0;
}

int write_ver2() {
    printf("Write Start\n");
    
    struct filetest *s = (struct filetest *) malloc(1 * sizeof(struct filetest));
    s->arraySize=20;
    s->array=(int *) malloc(s->arraySize*sizeof(int));

    for (int i = 0; i<s->arraySize; i++) {
        s->array[i]=i;
    }

    FILE *file = fopen(FILE_NAME,"wb");
    writeStruct(file, s);
    fclose(file);
    printf("Write End\n");

    return 0;
}

int read_ver2() {
    
    struct filetest *s = (struct filetest *) malloc(1 * sizeof(struct filetest));
    
    FILE *file = fopen(FILE_NAME,"rb");
    printf("Read Start\n");
    readStruct(file,s);
    fclose(file);
    printf("Read End\n");

    
    printf("%d\n",s->arraySize);
    for (int i=0; i<s->arraySize; i++) {
        printf("%d\n",s->array[i]);
    }
    

    return 0;
}

int write_ver3() {
    printf("Write Start\n");
    
    int testCount = 10;

    struct filetest **s = (struct filetest **) malloc(testCount * sizeof(struct filetest));
    
    for (int i=0; i<testCount; i++) {
        s[i]=(struct filetest *)malloc(sizeof(struct filetest));
        s[i]->arraySize=(i+1)*2;
        s[i]->array = (int *) malloc(s[i]->arraySize*sizeof(int));
        for (int j = 0; j<s[i]->arraySize; j++) {
            s[i]->array[j]=j+i;
        }
    }

    FILE *file = fopen(FILE_NAME,"wb");
    fwrite(&testCount,sizeof(int),1,file);
    for (int i=0; i<testCount; i++) {
        writeStruct(file, s[i]);
    }
    fclose(file);
    printf("Write End\n");

    return 0;
}

int read_ver3() {
    
    int size;
    
    
    FILE *file = fopen(FILE_NAME,"rb");
    printf("Read Start\n");
    fread(&size,sizeof(int),1,file);
    printf("%d structs\n",size);
    struct filetest **s = (struct filetest **) malloc(size * sizeof(struct filetest));
    for (int i=0; i<size; i++) {
        s[i]=(struct filetest *) malloc(sizeof(struct filetest));
        readStruct(file,s[i]);
    }
    fclose(file);
    printf("Read End\n\n");
    
    for (int i=0; i<size; i++) {
        printf("Struct %d:\nArray Size: %d\n",i,s[i]->arraySize);
        
        for (int j=0; j<s[i]->arraySize; j++) {
            if (j>0)
                printf(", ");
                
            printf("%d",s[i]->array[j]);
            
        }        
        printf("\n\n");
    }
    

    return 0;
}

int write() {
    printf("Write Start\n");
    
    int testCount = 10;

    struct filetest **s = (struct filetest **) malloc(testCount * sizeof(struct filetest));
    
    for (int i=0; i<testCount; i++) {
        s[i]=(struct filetest *)malloc(sizeof(struct filetest));
        s[i]->arraySize=(i+1)*2;
        s[i]->array = (int *) malloc(s[i]->arraySize*sizeof(int));
        for (int j = 0; j<s[i]->arraySize; j++) {
            s[i]->array[j]=j+i;
        }
    }

    FILE *file = fopen(FILE_NAME,"wb");
    fwrite(&testCount,sizeof(int),1,file);
    for (int i=0; i<testCount; i++) {
        writeStruct(file, s[i]);
    }
    fclose(file);
    printf("Write End\n");

    return 0;
}

int read() {
    
    int size;
    
    
    FILE *file = fopen(FILE_NAME,"rb");
    printf("Read Start\n");
    fread(&size,sizeof(int),1,file);
    printf("%d structs\n",size);
    struct filetest **s = (struct filetest **) malloc(size * sizeof(struct filetest));
    for (int i=0; i<size; i++) {
        s[i]=(struct filetest *) malloc(sizeof(struct filetest));
        readStruct(file,s[i]);
    }
    fclose(file);
    printf("Read End\n\n");
    
    for (int i=0; i<size; i++) {
        printf("Struct %d:\nArray Size: %d\n",i,s[i]->arraySize);
        
        for (int j=0; j<s[i]->arraySize; j++) {
            if (j>0)
                printf(", ");
                
            printf("%d",s[i]->array[j]);
            
        }        
        printf("\n\n");
    }
    

    return 0;
}

int writeStruct_old(FILE *file, struct filetest *inStruct) {

    fwrite(&inStruct->arraySize,sizeof(int),1,file);
    fwrite(inStruct->array,sizeof(int),inStruct->arraySize,file);
    
    return 0;
}

int readStruct_old(FILE *file, struct filetest *outStruct) {

    fread(&outStruct->arraySize,sizeof(int),1,file);

    outStruct->array=(int *) malloc(outStruct->arraySize*sizeof(int));
    fread(outStruct->array,sizeof(int),outStruct->arraySize,file);

    return 0;
}

int writeStruct(FILE *file, struct filetest *inStruct) {

    //fwrite(&inStruct->arraySize,sizeof(int),1,file);
    //fwrite(inStruct->array,sizeof(int),inStruct->arraySize,file);

    fwrite(inStruct,sizeof(struct filetest),1,file);
    
    return 0;
}

int readStruct(FILE *file, struct filetest *outStruct) {

    //fread(&outStruct->arraySize,sizeof(int),1,file);

    //outStruct->array=(int *) malloc(outStruct->arraySize*sizeof(int));
    //fread(outStruct->array,sizeof(int),outStruct->arraySize,file);

    fread(outStruct,sizeof(struct filetest),1,file);

    return 0;
}