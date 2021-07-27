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

    struct String
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
    struct Indent{};
    struct Dedent{};
    struct Class{};
    struct Def{};
    struct Return{};
    struct If{};
    struct Else{};
    struct And{};
    struct Or{};
    struct Not{};
    struct Eq{};
    struct NotEq{};
    struct LessOrEq{};
    struct GreaterOrEq{};
    struct Less{};
    struct Greater{};
    struct None{};
    struct True{};
    struct False{};
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
    Tokens::Assign,
    Tokens::String,
    Tokens::Indent,
    Tokens::Dedent,
    Tokens::Class,
    Tokens::Def,
    Tokens::Return,
    Tokens::If,
    Tokens::Else,
    Tokens::And,
    Tokens::Or,
    Tokens::Not,
    Tokens::Eq,
    Tokens::NotEq,
    Tokens::LessOrEq,
    Tokens::GreaterOrEq,
    Tokens::Less,
    Tokens::Greater,
    Tokens::None,
    Tokens::True,
    Tokens::False
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
