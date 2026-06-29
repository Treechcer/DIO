#include"..\Headers\lexer.h"
#include"..\Headers\ast.h"

int main(int argc, char **argv){
    buildAst(lex("5 + 7 - 82 * 9 / 5.24", "shell"));
    return 0;
}