#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

struct Job {
    int id;
    int startIndex1;
    int endIndex1;
    int startIndex2;
    int endIndex2;
};

int seedParam;
int threadCount;

char **words;
int wordCount=0;

char *password;
int passwordLen;

char *foundPassword;
int foundPasswordLen;

bool isFound=false;
int findAttempts=0;

int readWords(char *wordFilepath);
int createPassword();
int bruteForce();
void printResults(int exitcode);

int main(int argc, char **argv) {

    int value =0;

    if (argc!=4) {
        printf("usage: %s <%s> <%s> <%s>\n",argv[0],"F","S","N");
        printf("F:\tFile containing list of words for password\n");
        printf("S:\tSeed for random number generator\n");
        printf("N:\tNumber of threads to use\n");
        return -1;
    }

    seedParam=atoi(argv[2]);
    threadCount=atoi(argv[3]);
    value = readWords(argv[1]);

    if (value==0) {
        value=createPassword();
        printf("Password: '%s'\n",password);
        value = bruteForce();
    }

    printResults(value);

    return value;
}

int readWords(char *wordFilepath) {
    FILE *file = fopen(wordFilepath,"r");
    if (file==NULL) return -1;

    char word[1024];
    size_t len=0;
    ssize_t read;
    // while ((read=getline(&line,&len,file))!=-1) {
    //     wordCount++;
    // }
    printf("Here\n");
    while (fscanf(file," %1023s",word)==1) {
        wordCount++;
    }
    printf("Here\n");
    printf("Word count: %d\n",wordCount);
    words = (char **) malloc(sizeof(char *) * wordCount);
    fseek(file,0,SEEK_SET);
    int i=0;
    // while ((read=getline(&line,&len,file))!=-1) {
    while (fscanf(file," %1023s",word)==1) {
        len = strlen(word);
        if (len==0) {
            i++;
            continue;
        }
        int j=0;
        char *_line = (char *) malloc(sizeof(char)*len);
        _line[len]='\0';
        for (j; j<len-1; j++) {
            _line[j]=word[j];
            if (_line[j]=='\n') {
                _line[j]='\0';
                break;
            } 
        }
        
        words[i] = (char *) malloc(sizeof(char)*j);
        for (int k=0;k<j;k++) words[i][k]=_line[k];
        free(_line);
        i++;
    }
    fclose(file);

    return 0;   
}

int createPassword() {

    if (wordCount<2) return -1;

    srand(seedParam);

    int i1;
    int i2;

    do {
        i1=rand()%wordCount;
    } while (i1>=wordCount||i1<0);

    do {
        i2=rand()%wordCount;
    } while (i2>=wordCount|i2<0||i2==i1);

    int len1=strlen(words[i1]);
    int len2=strlen(words[i2]);
    passwordLen=len1+len2;

    password = (char *) malloc(sizeof(char)*passwordLen+1);
    for (int i=0; i<len1; i++) password[i]=words[i1][i];
    for (int i=0; i<len2; i++) password[i+len1]=words[i2][i];
    password[passwordLen]='\0';
    printf("%s\n",password);

    return 0;
}

void *bruteWork(void *arg) {
    Job *job = (Job*)arg;
    printf("\tThread(%d) started\t1.start:%d\tend:%d\t2.start:%d\tend:%d\n",job->id,job->startIndex1,job->endIndex1,job->startIndex2,job->endIndex2);
    bool match=false;
    int matchChar=0;
    for (int index=job->startIndex1; index<job->endIndex1; index++) {
        if (isFound) break;
        int firstLen = strlen(words[index]);
        int i=job->startIndex2;
        int secondLen = 0;
        for (i; i<job->endIndex2; i++) {
            matchChar=0;
            match=true;
            findAttempts++;
            secondLen = strlen(words[i]);
            // Intentional performance decision to mimic real world performance impediments to brute forcing.
            for (int j=0; j<firstLen; j++) {
                if (words[index][j]!=password[j]) {
                    matchChar=0;
                    match=false;
                    break;
                } else matchChar++;
            }
            if (!match||matchChar==0) {
                continue;
            } 
            for (int j=0; j<secondLen; j++) {
                if (words[i][j]!=password[j+firstLen]) {
                    matchChar=0;
                    match=false;
                    break;
                } else matchChar++;
            }
            if (match && matchChar==passwordLen) {
                break;
            }
        }
        if (match&&matchChar==passwordLen) {
            isFound=true;
            // printf("ThreadId()\tPassword found!\n");
            foundPasswordLen = firstLen+secondLen;
            foundPassword = (char *) malloc(sizeof(char) * foundPasswordLen);
            for (int j=0; j<firstLen; j++) foundPassword[j]=words[index][j];
            for (int j=0; j<secondLen; j++) foundPassword[j+firstLen]=words[i][j];
            break;
        }
    }

    return NULL;
}
int bruteForce() {
    bool threadSat = ((double)wordCount/threadCount)<1;
    if (threadSat) {
        threadCount=wordCount-1;
        threadSat=false;
    }

    pthread_t threads[threadCount];
    Job jobs[threadCount];
    int *errCode = (int *) malloc(sizeof(int) * threadCount);
    
    for (int i=0; i<threadCount; i++) {
        jobs[i].id=i;
        if (!threadSat) {
            jobs[i].startIndex1=((double)i/threadCount)*wordCount;
            jobs[i].endIndex1=((double)i/threadCount)*wordCount + (1.0/threadCount)*wordCount;
            jobs[i].startIndex2=0;
            jobs[i].endIndex2=wordCount;
        } else {
            // Placeholder condition for dividing work where 
            // there is less than one unit of work per thread in
            // the primary methodology.
        }
        //https://stackoverflow.com/questions/19286698/how-do-i-get-the-error-code-from-pthread-join
        if ((errCode[i] = pthread_create(&threads[i],NULL,bruteWork,(void *)&jobs[i])!=0)) {
            printf("error creating thread %d, error=%d\n", i, errCode[i]);
        }
    }
    
    for (int i=0; i<threadCount; i++) {
        errCode[i] == pthread_join(threads[i],NULL);
        if (errCode[i]!=0) {
            printf("error joining thread %d, error=%d\n", i, errCode[i]);
        }
    }
    if (errno!=0) {
        perror("Error:");
    }

    // free(threads);

    return 0;
}

void printResults(int exitcode) {

    if (exitcode!=0) {
        printf("\nThe process terminated with error code %d\n", exitcode);
        return;
    }

    if (foundPasswordLen>0) {
        printf(
            "\n%d threads were able to match password\n%s with %s over %d operations\n",
            threadCount,password,foundPassword,findAttempts
            );
    } else {
        printf(
            "\n%d threads were unable to match password\n%s over %d operations\n",
            threadCount,password,findAttempts
            );
    }
}

