#pragma once

#include <variant>
#include <string>
#include <ostream>

namespace Tokens
{
    struct Integer
    {
        int value;
    };

    struct Id
    {
        std::string value;
    };

    struct Eof{};
    struct NewLine{};
    struct Plus{};
    struct Minus{};
    struct Mul{};
    struct Div{};
    struct Lparen{};
    struct Rparen{};
    struct Assign{};
}

using TokenType = std::variant<
    Tokens::Eof,
    Tokens::NewLine,
    Tokens::Plus,
    Tokens::Minus,
    Tokens::Mul,
    Tokens::Div,
    Tokens::Integer,
    Tokens::Lparen,
    Tokens::Rparen,
    Tokens::Id,
    Tokens::Assign
>;

class Token
{
public:
    explicit Token(TokenType type)
        : m_Type(std::move(type))
    {
    }

    template<typename T>
    bool Is() const
    {
        return std::holds_alternative<T>(m_Type);
    }

    template<typename T>
    const T& As() const
    {
        return std::get<T>(m_Type);
    }

    template<typename T>
    const T* TryAs() const
    {
        return std::get_if<T>(&m_Type);
    }

    friend bool operator==(const Token& lhs, const Token& rhs);
private:
    TokenType m_Type;
};

std::ostream& operator<<(std::ostream& os, const Token& token);
bool operator==(const Token& lhs, const Token& rhs);
