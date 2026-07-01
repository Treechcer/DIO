#include"..\Headers\lexer.h"
#include"..\Headers\ast.h"
#include"..\Headers\token.h"

int main(int argc, char **argv){
    buildAst(lex("if(5 + 7 - 82 * 9 / 5.24 )", "shell"));
    return 0;
}