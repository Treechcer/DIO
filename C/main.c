#include<string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h> 
#include <libgen.h>
#include <direct.h>

#include"../Headers/lexer.h"
#include"../Headers/ast.h"
#include"../Headers/token.h"
#include"../Headers/parser.h"
#include"../Headers/inputargs.h"
#include"../Headers/STD.h"
#include"../Headers/helper_functions.h"

Node* createInput(char* name, variableTypes type){
    Node* input = createNode();
    input->type = VARIABLENODE;
    input->data.variableNode = malloc(sizeof(variableNode));
    input->data.variableNode->initialise = 1;
    input->data.variableNode->name = name;
    input->data.variableNode->type = type;
    input->data.variableNode->value = NULL;

    return input;
}

void initLowLevelFuncs(){
    dynamicNode inputs = {0,0,0};
    DYN_PUSH(createInput("a", UNKNOWNVARTYPE), inputs);
    createLowLevelFunc("out", inputs);
    
    inputs = (dynamicNode){0,0,0};
    DYN_PUSH(createInput("a", STRINGVAR), inputs);
    createLowLevelFunc("exec", inputs);
}

int main(int argc, char **argv){
    //BUILD INPUTS!!

    int fileArgIndex = registerArg(STRINGPUT, "file", 1, "");

    //CHECK INPUTS!
    checkInuts(argc, argv);

    srand(time(NULL)); //for pseudo random Num

    //get file
    char* filePath = getvalueByIndex(fileArgIndex);

    if (filePath != NULL && strlen(filePath) > 0){
        char* fName = getvalueByIndex(fileArgIndex);
        char *dir_copy = strdup(fName);
        chdir(dirname(dir_copy));

        char *base_copy = strdup(fName);
        char *realName = basename(base_copy);

        fileReadReturn mainFile = readFile(realName);

        char code[mainFile.size + 3];
        strcpy(code, mainFile.content);
        strcat(code, "\n>>");

        initLowLevelFuncs();
        dynamicToken toks = {0,0,0};
        parse(buildAst(lex(code, filePath, lex(getSTD(), "STDlib.dio", toks))));
    }
    
    return 0;
}