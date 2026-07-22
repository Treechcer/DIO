#include<string.h>
#include <stdlib.h>
#include <time.h>

#include"../Headers/lexer.h"
#include"../Headers/ast.h"
#include"../Headers/token.h"
#include"../Headers/parser.h"
#include"../Headers/inputargs.h"
#include"../Headers/STD.h"

int main(int argc, char **argv){
    //BUILD INPUTS!!

    int fileArgIndex = registerArg(STRINGPUT, "file", 1, "");

    //CHECK INPUTS!
    checkInuts(argc, argv);

    srand(time(NULL)); //for pseudo random Num

    //get file
    char* filePath = getvalueByIndex(fileArgIndex);
    if (filePath != NULL && strlen(filePath) > 0){
        // FILE READ source (modified):
        // Source - https://stackoverflow.com/a/14002993
        // Posted by user529758, modified by community. See post 'Timeline' for change history
        // Retrieved 2026-07-04, License - CC BY-SA 4.0
        FILE* filePointer = fopen(getvalueByIndex(fileArgIndex), "rb");
        fseek(filePointer, 0, SEEK_END);
        long fsize = ftell(filePointer);
        fseek(filePointer, 0, SEEK_SET);

        char *string = malloc(fsize + 1 + 3);
        fread(string, fsize, 1, filePointer);
        fclose(filePointer);

        //lmao this actually fixed it (I'll keep it though)

        string[fsize - 3] = '\n';
        string[fsize - 2] = '>';
        string[fsize - 1] = '>';
        string[fsize] = '\0';
        //printf("%s\n", string);

        char* STD = getSTD();
        int STDSize = (int)strlen(STD);
        char codeWithStd[STDSize + 1 + fsize];
        strcpy(codeWithStd, STD);
        strcat(codeWithStd, string);
        codeWithStd[STDSize-1] = '\n';
        printf("%s\n", STD);
        printf("----------\n");
        printf("%s\n", string);
        printf("----------\n");
        printf("%s\n", codeWithStd);
        printf("----------\n");

        createLowLevelFunc("out");

        parse(buildAst(lex(codeWithStd, filePath)));
    }
    
    return 0;
}