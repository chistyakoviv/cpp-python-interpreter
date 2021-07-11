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
    Parser parser(lexer);

    std::unique_ptr<AST::Node> tree = parser.Expr();

    if (ObjectHolder result = tree->Evaluate())
    {
        result->Print(std::cout);
        std::cout << std::endl;
    }
    else
    {
        std::cout << "None";
    }

}
