
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>

int executeProgram(int iter);

struct Job {
    int id;
    int iterCount;
};
struct Result {
    unsigned long long int currentIteration;
    unsigned long long int maxIteration;
    unsigned long long int hits;
    unsigned long long int attempts;
};

int errorCode;
int jobCount=-1;

pthread_mutex_t lockJobsRunning = PTHREAD_MUTEX_INITIALIZER;
int jobsRunning=0;
Result *results;

int main(int argc, char **argv) {

    if (argc != 3) {
        errorCode = printf("Usage: %s <i> <n>\n", argv[0]);
        if (errorCode!=0) {
            perror("Error:");
            return errorCode;
        }
        errorCode = printf("i\tNumber of iterations thate each thread will run the Monte Carlo simulation\nN\tNumber of threads\n");
        if (errorCode!=0) {
            perror("Error:");
            return errorCode;
        }
        return -1;
    }
    int iter = atoi(argv[1]);
    jobCount = atoi(argv[2]);

    if (iter==0||jobCount==0) {
        return -1;
    }

    jobsRunning=jobCount;
    results = (Result *) malloc(sizeof(Result) * jobCount);
    if (results==NULL) {
        perror("Error:");
        return -1;
    }
    for (int i=0; i<jobCount; i++) {
        results[i].attempts=0;
        results[i].hits=0;
    }

    return executeProgram(iter);

} 


void *displayThread(void *arg) {
    unsigned long long int hits, attempts;
    float complete;
    if (printf("Attempting to calculate Pi\n")<0) {
        perror("Error:");
        exit(errno);
    }
    sleep(1);
    while (jobCount==-1||jobsRunning>0) {
        hits=0; attempts=0, complete=0;
        for (int i=0; i<jobCount; i++) {
            hits+=results[i].hits;
            attempts+=results[i].attempts;
            
            complete=(complete==0)?(float)results[i].currentIteration/results[i].maxIteration:
                (complete+((float)results[i].currentIteration/results[i].maxIteration))/2;
        }

        if(printf("hits:\t%llu;\tattempts:\t%llu\t\tTentative Result:\t%1.10f\tPercent Complete: %f\n",hits,attempts,((float)hits/attempts)*4,complete)<0) {
            perror("Error:");
            exit(errno);
        }
        sleep(1);
    }
    hits=0; attempts=0;
    for (int i=0; i<jobCount; i++) {
        hits+=results[i].hits;
        attempts+=results[i].attempts;
    }

    if (printf("hits:\t%llu;\tattempts:\t%llu\t\tFinal Result:\t%1.10f\n",hits,attempts,((float)hits/attempts)*4)<0) {
        perror("Error:");
        exit(errno);
    }

    return NULL;
}
void *montecarloThread(void *argv) {
    Job *job = (Job *)argv;
    srand48(time(NULL)+pow(job->id,100));

    if (errno!=0) {
        perror("Error:");
        exit(errno);
    }

    results[job->id].maxIteration=job->iterCount;

    for (int i=0; i<job->iterCount; i++) {
        results[job->id].currentIteration=i;
        float x=pow(drand48(),2), y=pow(drand48(),2);
        if (x+y<=1) 
            results[job->id].hits++;
        results[job->id].attempts++;
    }

    while (pthread_mutex_lock(&lockJobsRunning)!=0) { sleep(1); }
    jobsRunning--;
    while (pthread_mutex_unlock(&lockJobsRunning)!=0) { sleep(1); }

    return NULL;
}
int executeProgram(int iter) {
    pthread_t threads[jobCount+1];
    for (int i=0; i<jobCount; i++) {
        Job *job = (Job *) malloc(sizeof(Job));
        if (job==NULL) {
            perror("Error:");
            exit(errno);
        }
        job->id=i;
        job->iterCount = iter;
        if (pthread_create(&threads[i], NULL, montecarloThread, (void *)job)!=0) {
            perror("Error:");
            exit(errno);
        }   
    }

    if (pthread_create(&threads[jobCount], NULL, displayThread, NULL)!=0) {
        perror("Error:");
        exit(errno);
    }

    for (int i=0; i<=jobCount; i++) {
        if (pthread_join(threads[i], NULL)!=0) {
            perror("Error:");
            exit(errno);
        }
    }

    if (printf("Program Complete\n")<0) {
        perror("Error:");
        exit(errno);
    }

    return 0;
}