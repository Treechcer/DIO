#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../Headers/token.h"
#include "../Headers/helper_functions.h"
#include "../Headers/errorsNew.h"

int getVarIndexByName(char* name);

long int convertToLongInt(char* value){
    return atol(value);
}

int convertToInt(char* value){
    return atol(value);
}

float convertToFloat(char* value){
    return atof(value);
}

double convertToDouble(char* value){
    char* endptr;
    return strtod(value, &endptr);
}

bool isDigit(char c){
    if (c >= '0' && c <= '9'){
        return true;
    }
    return false;
}

bool isAlpha(char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool isAlphaString(char* c){
    for (int i = 0; i < strlen(c); i++){
        char ch = *c;
        if (!isAlpha(ch)){
            return 0;
        }
        c++;
    }

    return 1;
}

Token createToken(char* value, TokenType identifier, Position pos){
    return (Token) {.value = value, .identifier = identifier, .pos = pos};
}

Position createPosition(int* start, int* end, int* line, char* file){
    int* positionStart = malloc(sizeof(*positionStart));
    int* positionEnd = malloc(sizeof(*positionEnd));
    int* positionLine = malloc(sizeof(*positionLine));
    *positionStart = *start;
    *positionEnd = *end;
    *positionLine = *line;

    return (Position) {.start = positionStart, .end = positionEnd, .line = positionLine, .file = file};
}

size_t getStringSize(const char* string){
    if (string == NULL) return 0;
    const char *ch = string;
    size_t len = 0;
    
    while (*ch != '\0'){
        ch++;
        len++;
    }

    return len;
}

fileReadReturn readFile(char* fName){
    // FILE READ source (modified):
    // Source - https://stackoverflow.com/a/14002993
    // Posted by user529758, modified by community. See post 'Timeline' for change history
    // Retrieved 2026-07-04, License - CC BY-SA 4.0
    FILE* filePointer = fopen(fName, "rb");
    if (filePointer == NULL){
        printf("File '%s' not found.", fName);
        exit(1);
    }

    fseek(filePointer, 0, SEEK_END);
    long fsize = ftell(filePointer);
    fseek(filePointer, 0, SEEK_SET);

    char *mainFile = malloc(fsize + 1);
    fread(mainFile, fsize, 1, filePointer);
    fclose(filePointer);

    mainFile[fsize] = '\0';

    fileReadReturn ret = (fileReadReturn){.size = fsize, .content = mainFile, .exists = 1};
    return ret;
}

int getIndexFromInput(char* name){
    if (name == NULL){
        return -1;
    }
    int indexArr = getVarIndexByName(name);
    indexArr = (indexArr == -1) ? atoi(name) : indexArr;

    return indexArr;
}

Position* copyPos(Position* input){
    Position* ret = malloc(sizeof(Position));
    ret->end = input->end;
    ret->start = input->start;
    ret->line = input->line;
    ret->file = input->file;
    return ret;
}

int checkIfValidFileEnd(char* fileName){
    char* fName = malloc(sizeof(char));
    int startIndex = 0;
    int size = 0;

    for (int i = strlen(fileName)-1; i >= 0; i--){
        if (fileName[i] == '.'){
            startIndex = i;
            break;
        }
        size += 1;
        fName = realloc(fName, size);
        fName[size-1] = fileName[i];
    }
    
    fName[size] = '\0';
    fName = strrev(fName);

    char* names[] = arrayOfFileExtensions();
    int count = sizeof(names) / sizeof(names[0]);
    
    int canContinue = 0;

    for (int i = 0; i < count; i++){
        if (strcmp(names[i], fName) == 0){
            canContinue = 1;
            break;
        }
    }

    return canContinue;

    //for (int i = startIndex; i <= strlen(fileName)-1; i++){
    //    printf("%c", fileName[i]);
    //}
}