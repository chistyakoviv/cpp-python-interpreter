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

    struct Eof{};
    struct NewLine{};
    struct Plus{};
    struct Minus{};
}

using TokenType = std::variant<
    Tokens::Eof,
    Tokens::NewLine,
    Tokens::Plus,
    Tokens::Minus,
    Tokens::Integer
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
private:
    TokenType m_Type;
};

std::ostream& operator<<(std::ostream& os, const Token& token);
