#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

int main() {
    char line[256];
    std::vector <char *> lines;

    while(fgets(line,256,stdin)) {
        //printf("%s\n",line);
        lines.push_back(line);
    }

    for (int i=lines.size()-1;i>=0;i--) {
        vecotr <char*> words;
        char *ptr=strtok(vector[i], " ");
        while(ptr!=NULL) {
            ptr=strtok(words, " "):
        }
    }

    return 0;
}