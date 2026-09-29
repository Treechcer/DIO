#include"../Headers/token.h"
#include"../Headers/dynamic_array.h"
#include"../Headers/helper_functions.h"
#include"../Headers/errorsNew.h"
#include"../Headers/lexer.h"
#include"../Headers/parser.h"
#include"../Headers/STD.h"

void repl(char* filePath){
    //TODO: fix???
    //This BAREALY works, can ruin one command, like "for (int a = 0; a < 10; a = a + 1); out(a); end;", adn doesn't throw out errors 

    initLowLevelFuncs();
    dynamicToken toks = {0,0,0};
    toks = lex(getSTD(), "STDlib.dio", toks);
    while (1){
        char buffer[4096];

        printf("DIO REPL>> ");

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            toks = lex(buffer, "REPL.dio", toks);
            parse(buildAst(toks));
        }
        else {
            break;
        }
    }
}
