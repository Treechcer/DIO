#include<string.h>

#include"..\Headers\lexer.h"
#include"..\Headers\ast.h"
#include"..\Headers\token.h"
#include"..\Headers\parser.h"
#include"..\Headers\inputargs.h"

int main(int argc, char **argv){
    //BUILD INPUTS!!

    int fileArgIndex = registerArg(STRINGPUT, "file", 1, "");

    //CHECK INPUTS!
    checkInuts(argc, argv);

    //get file
    char* filePath = getvalueByIndex(fileArgIndex);
    if (filePath != NULL && strlen(filePath) > 0){
        //FILE READ source (modified):
        // Source - https://stackoverflow.com/a/14002993
        // Posted by user529758, modified by community. See post 'Timeline' for change history
        // Retrieved 2026-07-04, License - CC BY-SA 4.0
        FILE* filePointer = fopen(getvalueByIndex(fileArgIndex), "r");
        fseek(filePointer, 0, SEEK_END);
        long fsize = ftell(filePointer);
        fseek(filePointer, 0, SEEK_SET);

        char *string = malloc(fsize + 1);
        fread(string, fsize, 1, filePointer);
        fclose(filePointer);

        string[fsize-1] = '\0';
        //printf("%s\n", string);

        parse(buildAst(lex(string, filePath)));
    }
    
    return 0;
}