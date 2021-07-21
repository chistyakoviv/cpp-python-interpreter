#include <memory>
#include "lexer.h"
#include "token.h"
#include "ast.h"

class Parser
{
public:
    Parser(Lexer& lexer)
        : m_Lexer(lexer), m_CurrentToken(m_Lexer.GetNextToken())
    {
    }

    std::unique_ptr<AST::Node> ParseProgram();

private:

    std::unique_ptr<AST::Node> Expr();
    std::unique_ptr<AST::Node> Term();
    std::unique_ptr<AST::Node> Factor();
    std::unique_ptr<AST::Node> Statement();
    std::unique_ptr<AST::Node> StatementList();
    std::unique_ptr<AST::Node> AssignmentStatement();
    std::string Variable();

    template<typename T>
    void Consume()
    {
        if (m_CurrentToken.Is<T>())
            m_CurrentToken = m_Lexer.GetNextToken();
        else
            throw std::runtime_error("Unxpected Token at line");
    }

private:
    Lexer& m_Lexer;
    Token m_CurrentToken;
};
