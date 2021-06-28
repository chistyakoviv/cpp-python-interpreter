#pragma once

#include <variant>
#include <string>
#include <ostream>

enum class TokenType
{
    Eof,
    PLUS,
    INTEGER
};

using TokenValue = std::variant<int, char, std::string>;

class Token
{
public:
    Token(TokenType type, TokenValue value)
        : m_Type(type), m_Value(value)
    {
    }

    TokenType GetType() const
    {
        return m_Type;
    }

    TokenValue GetValue() const
    {
        return m_Value;
    }
private:
    TokenType m_Type;
    TokenValue m_Value;
};

std::ostream& operator<<(std::ostream& os, const Token& token);
