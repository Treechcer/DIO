#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Headers/Token.h"
#include "../Headers/helper_functions.h"

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

char* getLine(Position pos){
    fileReadReturn fileData = readFile(pos.file);
    int line = 1;
    for (size_t i = 0; i < fileData.size; i++){
        if (line == *pos.line){
            char* str = malloc(1);
            str[0] = '\0';
            for (int j = i; j < fileData.size; j++){
                //printf("%c\n", fileData.content[j]);
                if (fileData.content[j] == '\n'){
                    //printf("||%s\n", str);
                    return str;
                }
                char ch = fileData.content[j];
                int len = strlen(str);
                char* strTemp = realloc(str, (len+2)*sizeof(char));
                str = strTemp;
                str[len] = ch;
                str[len+1] = '\0';
            }
        }

        if (fileData.content[i] == '\n'){
            line++;
        }
    }
}

void raiseError(const char* file, Position pos){
    char* processName = scanForFileName(file);
    char* codeLine = getLine(pos);

    printf("%s\n", codeLine);
}