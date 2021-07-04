#include "parser.h"

namespace {
    // bool operator==(const Token& token, char c)
    // {
    //     auto ptr = token.TryAs<Tokens::Char>();
    //     return ptr && ptr->value == c;
    // }

    // bool operator!=(const Token& token, char c)
    // {
    //     return !(token == c);
    // }
}

int Parser::Expr()
{
    int result = Factor();
    while (m_CurrentToken.Is<Tokens::Plus>() || m_CurrentToken.Is<Tokens::Minus>() || m_CurrentToken.Is<Tokens::Div>() || m_CurrentToken.Is<Tokens::Mul>())
    {
        if (m_CurrentToken.Is<Tokens::Plus>())
        {
            Consume<Tokens::Plus>();
            result += Factor();
        }
        else if (m_CurrentToken.Is<Tokens::Minus>())
        {
            Consume<Tokens::Minus>();
            result -= Factor();
        }
        else if (m_CurrentToken.Is<Tokens::Mul>())
        {
            Consume<Tokens::Mul>();
            result *= Factor();
        }
        else if (m_CurrentToken.Is<Tokens::Div>())
        {
            Consume<Tokens::Div>();
            result /= Factor();
        }
    }

    return result;
}

int Parser::Factor()
{
    Token token = m_CurrentToken;
    Consume<Tokens::Integer>();
    return token.As<Tokens::Integer>().value;
}
