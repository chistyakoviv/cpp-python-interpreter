#include <memory>
#include "lexer.h"
#include "token.h"
#include "ast.h"

class Parser
{
public:
    Parser(Lexer& lexer)
        : m_Lexer(lexer), m_CurrentToken(m_Lexer.GetCurrentToken())
    {
    }

    std::unique_ptr<AST::Node> Expr();
    std::unique_ptr<AST::Node> Term();
    std::unique_ptr<AST::Node> Factor();

    template<typename T>
    void Consume()
    {
        if (m_Lexer.GetCurrentToken().Is<T>())
            m_CurrentToken = m_Lexer.GetNextToken();
        else
            throw std::runtime_error("Unxpected Token at line");
    }

private:
    Lexer& m_Lexer;
    Token m_CurrentToken;
};
