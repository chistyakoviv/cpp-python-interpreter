#include "lexer.h"
#include "token.h"

class Parser
{
public:
    Parser(Lexer& lexer)
        : m_Lexer(lexer), m_CurrentToken(m_Lexer.GetCurrentToken())
    {
    }

    int Factor();
    int Expr();

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
