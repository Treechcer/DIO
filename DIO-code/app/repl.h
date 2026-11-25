#ifndef REPL__H
#define REPL__H

#include"../interpreter/token.h"
#include"../libraries/dynamic_array.h"
#include"../libraries/helper_functions.h"
#include"../libraries/errors.h"
#include"../interpreter/lexer.h"

void repl(char* filePath);

#endif