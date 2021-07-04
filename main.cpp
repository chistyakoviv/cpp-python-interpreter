#include <iostream>
#include <fstream>
#include "token.h"
#include "lexer.h"
#include "parser.h"

int main()
{
    std::ifstream file("test.py");
    Lexer lexer(file);
    Parser parser(lexer);

    std::cout << parser.Expr() << std::endl;
}
