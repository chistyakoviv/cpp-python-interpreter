#include <iostream>
#include <fstream>
#include "token.h"
#include "lexer.h"

int main()
{
    std::ifstream file("test.py");
    Lexer lexer(file);

    Token t1 = lexer.GetNextToken();
    Token t2 = lexer.GetNextToken();
    Token t3 = lexer.GetNextToken();

    std::cout << t1 << t2 << t3 << std::endl;
}
