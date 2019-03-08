
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/wait.h>

const int OPERAND_MAX = 9;
const int OPERAND_MIN = 0;
const int EXPRESSION_SIZE = 3;

int init();
int mainLoop();
int childWorker();
int getExpression(char *expression);
int writeExpression(char *value);
int readExpression(char *expression);
int evalExpression(char *expression);

char *operators;
int operatorCount;
FILE *file;
int count=0;

int main(int argc, char **argv) {

    if (argc!=3) {
        printf("usage: %s filename n\n",argv[0]);
        return 0x01;
    }

    srand(time(NULL));
    file = fopen(argv[1],"w+");
    if (file==NULL) {
        perror("Error opening file for write:");
        return 0x02;
    }

    count=atoi(argv[2]);

    init();

    int pid = fork();

    switch (pid) {
        case 0:
            return childWorker();
            break;
        default:
            return mainLoop();
            break;
    }
    
    return 0;
}

int init() {
    operatorCount=5;
    operators = (char*) malloc(sizeof(char)*operatorCount);
    int index=0;
    operators[index++]='+';
    operators[index++]='-';
    operators[index++]='*';
    operators[index++]='/';
    operators[index++]='%';

    return 0;
}

int mainLoop() {
    wait(NULL);
    
    fseek(file,SEEK_SET,0);
    char *expression = (char*) malloc(sizeof(char)*EXPRESSION_SIZE);

    while (readExpression(expression)!=0) {
        evalExpression(expression);
    }

    free(operators);
    fclose(file);
    return 0;
}

int childWorker() {

    for (int i=0; i<count; i++) {
        char *expression = (char*) malloc(sizeof(char)*EXPRESSION_SIZE);
        int result = getExpression(expression);
        
        if (result==0) {
            writeExpression(expression);
        }
    }
        
    exit(0);
}

int getExpression(char *expression) {
    int x = (rand()%(OPERAND_MAX-OPERAND_MIN+1))+OPERAND_MIN;    
    char op = operators[(rand()%(operatorCount))];
    int y = (rand()%(OPERAND_MAX-OPERAND_MIN+1))+OPERAND_MIN;;

    if (op=='/'||op=='%') {
        while (y==0)
            y = (rand()%(OPERAND_MAX-OPERAND_MIN+1))+OPERAND_MIN;
    }
    
    sprintf(&expression[0],"%d",x);
    sprintf(&expression[2],"%d",y);
    expression[1]=op;
    
    return 0;
}

int writeExpression(char *value) {
    
    fputs(value,file);
    fputs("\n",file);
    
    return 0;
}

int readExpression(char *expression) {
    return fread(expression,sizeof(char),EXPRESSION_SIZE,file);
}

int evalExpression(char *expression) {

    int x = atoi(&expression[0]);
    int y = atoi(&expression[2]);
    double z;

    switch (expression[1]) {
        case '+':
            z = x+y;
            break;
        case '-':
            z = x-y;
            break;
        case '*':
            z = x*y;
            break;
        case '/':
            z = (double)x/y;
            break;
        case '%':
            z = x%y;
            break;
        default:
            return 1;
    }

    printf("%d %c %d = %f\n",x,expression[1],y,z);

    return 0;
}

