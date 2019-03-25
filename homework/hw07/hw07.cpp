
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <utime.h>
#include <stdlib.h>
#include <ctime>

bool isValidDateTime(struct tm datetime);
bool isValidDate(int month, int day, int year);
bool isValidTime(int hour, int minute, int second);
int changeFileDateTime(char *filename, struct utimbuf *datetime);
bool isLeapYear(int year);

const int MIN_YEAR = 1583;

int main(int argc, char **argv) {
    

    if (argc!=8) {
        printf("usage: %s filename month day year hour minute second\nexample: %s filename 01 23 2019 45 12 30\n",argv[0],argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1],"r");
    if (file!=NULL) {
        fclose(file);
        
        struct tm datetime;
        datetime.tm_mon=atoi(argv[2])-1;
        datetime.tm_mday=atoi(argv[3]);
        datetime.tm_year=atoi(argv[4])-1900;
        datetime.tm_min=atoi(argv[6]);
        datetime.tm_hour=atoi(argv[5]);
        datetime.tm_sec=atoi(argv[7]);
        datetime.tm_isdst=-1;

        if (!isValidDateTime(datetime))
            return 1;

        time_t interval = mktime(&datetime);
        //printf("%s\n", ctime(&interval));
        struct utimbuf newTime;
        
        newTime.actime=interval;
        newTime.modtime=interval;
        changeFileDateTime(argv[1],&newTime);
        
        
    } else {
        perror("Error accessing file: ");
    }

    
}

//https://www.geeksforgeeks.org/program-check-date-valid-not/
bool isLeapYear(int year) {
    return (((year%4==0) && (year%100!=0)) || (year%400==0));
}

bool isValidDateTime(struct tm datetime2) {
    return (
        isValidDate(datetime2.tm_mon,datetime2.tm_mday, datetime2.tm_year+1900) &&
        isValidTime(datetime2.tm_hour,datetime2.tm_min,datetime2.tm_sec));
}
bool isValidDate(int month, int day, int year) {
    if (month<0||month>11) 
        return false;
    if (year<MIN_YEAR)
        return false;
    if (day<1)
        return false;
    else if (month==3||month==5||month==8||month==10)
        return day<=30;
    else if (month==2)
        if (isLeapYear(year)) 
            return day<30;
        else
            return day<29;
    else
        return day<32;
}
bool isValidTime(int hour, int minute, int second) {
    return
        hour>=0 && hour<24 &&
        minute>=0 && minute<60 &&
        second>=0 && second<61;
}
int changeFileDateTime(char *filename, struct utimbuf *datetime2) {
    
    //printf("%s\n",ctime(&(datetime2->actime)));
    utime(filename, datetime2);

    return 0;    
}