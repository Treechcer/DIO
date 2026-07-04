#include"..\Headers\lexer.h"
#include"..\Headers\ast.h"
#include"..\Headers\token.h"
#include"..\Headers\parser.h"

int main(int argc, char **argv){
    parse(buildAst(lex("1+1\n 1 - 7\n 7+2", "shell")));
    return 0;
}