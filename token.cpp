#include "token.h"

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    #define PRINT_TOKEN_WITH_VALUE(type) if (const type* ptr = token.TryAs<type>()) \
                                            os << #type << " {" << ptr->value << "}"

    PRINT_TOKEN_WITH_VALUE(Tokens::Integer);
    PRINT_TOKEN_WITH_VALUE(Tokens::Id);

    #undef PRINT_TOKEN_WITH_VALUE

    #define PRINT_TOKEN(type) if (token.Is<type>()) \
                                os << #type

    PRINT_TOKEN(Tokens::Eof);
    PRINT_TOKEN(Tokens::NewLine);
    PRINT_TOKEN(Tokens::Plus);
    PRINT_TOKEN(Tokens::Minus);
    PRINT_TOKEN(Tokens::Mul);
    PRINT_TOKEN(Tokens::Div);
    PRINT_TOKEN(Tokens::Lparen);
    PRINT_TOKEN(Tokens::Rparen);
    PRINT_TOKEN(Tokens::Assign);

    #undef PRINT_TOKEN

    return os << '\n';
}

bool operator==(const Token& lhs, const Token& rhs)
{
    if (lhs.m_Type.index() != rhs.m_Type.index())
        return false;

    if (lhs.Is<Tokens::Id>())
        return lhs.As<Tokens::Id>().value == lhs.As<Tokens::Id>().value;

    if (lhs.Is<Tokens::Integer>())
        return lhs.As<Tokens::Integer>().value == lhs.As<Tokens::Integer>().value;

    return true;
}
