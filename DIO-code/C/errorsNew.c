#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* scanForFileName(const char* filePath){
    #ifdef WIN32
        const char slash = '\\';
    #else
        const char slash = '/';
    #endif

    char* fileName = malloc(sizeof(char));
    int counter = 0;

    for (int i = strlen(filePath)-3; i > 0; i--){ //-3 because '/0c.'
        if (filePath[i] == slash){
            break;
        }
        fileName[counter++] = filePath[i];
        fileName = realloc(fileName, counter*sizeof(char));
    }
    fileName[counter] = '\0';

    char* unRev = malloc(sizeof(char));
    counter = 0;

    for (int i = strlen(fileName)-1; i >= 0; i--){
        unRev[counter++] = fileName[i];
        unRev = realloc(unRev, counter*sizeof(char));
    }
    unRev[counter] = '\0';

    return unRev;
}

void raiseError(const char* file){
    char* fileOriginName = scanForFileName(file);
}