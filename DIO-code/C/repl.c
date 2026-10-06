#include <string.h>

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
    while (1){
        char buffer[4096];
        dynamicToken STD = (dynamicToken){0,0,0};
        lex(getSTD(), "STDlib.dio", STD);
        printf("DIO REPL>> ");

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            int canSkip = 0;
            for (int i = 0; i < strlen(buffer); i++){
                if (buffer[i] == ' ' || buffer[i] == ';' || buffer[i] == '\0' || buffer[i] == '\n'){
                    canSkip = 1;
                }
                else{
                    canSkip = 0;
                    break;
                }
            }

            if (canSkip == 0){
                parse(buildAst(lex(buffer, "REPL.dio", STD)));
            }
        }
        else {
            break;
        }
    }
}
