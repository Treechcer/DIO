#include"..\Headers\lexer.h"
#include"..\Headers\ast.h"
#include"..\Headers\token.h"
#include"..\Headers\parser.h"

int main(int argc, char **argv){
    parse(buildAst(lex("false + false", "shell")));
    return 0;
}