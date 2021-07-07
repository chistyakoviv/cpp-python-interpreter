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
    int result = Term();
    while (m_CurrentToken.Is<Tokens::Plus>() || m_CurrentToken.Is<Tokens::Minus>())
    {
        if (m_CurrentToken.Is<Tokens::Plus>())
        {
            Consume<Tokens::Plus>();
            result += Term();
        }
        else if (m_CurrentToken.Is<Tokens::Minus>())
        {
            Consume<Tokens::Minus>();
            result -= Term();
        }
    }

    return result;
}

int Parser::Term()
{
    int result = Factor();
    while (m_CurrentToken.Is<Tokens::Div>() || m_CurrentToken.Is<Tokens::Mul>())
    {
        if (m_CurrentToken.Is<Tokens::Mul>())
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
