#include"..\Headers\lexer.h"
#include"..\Headers\ast.h"
#include"..\Headers\token.h"
#include"..\Headers\parser.h"

int main(int argc, char **argv){
    parse(buildAst(lex("1 < 2; 1 > 2; 1 >= 1", "shell")));
    return 0;
}