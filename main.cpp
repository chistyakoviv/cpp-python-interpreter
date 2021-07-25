#include <iostream>
#include <fstream>
#include <memory>
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "object_holder.h"
#include "ast.h"

int main()
{
    std::ifstream file("test.py");
    Lexer lexer(file);

    // while (true)
    // {
    //     static const auto Eof = Token{Tokens::Eof{}};
    //     auto token = lexer.GetNextToken();

    //     if (token == Eof)
    //         break;

    //     std::cout << token;
    // }

    Parser parser(lexer);

    Runtime::Closure closure;
    std::unique_ptr<AST::Node> tree = parser.ParseProgram();

    ObjectHolder result = tree->Evaluate(closure);
    std::cout << closure;
}
