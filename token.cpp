#include "token.h"

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    #define PRINT_TOKEN_WITH_VALUE(type) if (const type* ptr = token.TryAs<type>()) \
                                            os << #type << " {" << ptr->value << "}"

    PRINT_TOKEN_WITH_VALUE(Tokens::Integer);
    PRINT_TOKEN_WITH_VALUE(Tokens::Id);
    PRINT_TOKEN_WITH_VALUE(Tokens::String);

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
    PRINT_TOKEN(Tokens::Indent);
    PRINT_TOKEN(Tokens::Dedent);
    PRINT_TOKEN(Tokens::Print);
    PRINT_TOKEN(Tokens::Class);
    PRINT_TOKEN(Tokens::Def);
    PRINT_TOKEN(Tokens::Return);
    PRINT_TOKEN(Tokens::If);
    PRINT_TOKEN(Tokens::Else);
    PRINT_TOKEN(Tokens::And);
    PRINT_TOKEN(Tokens::Or);
    PRINT_TOKEN(Tokens::Not);
    PRINT_TOKEN(Tokens::Eq);
    PRINT_TOKEN(Tokens::NotEq);
    PRINT_TOKEN(Tokens::LessOrEq);
    PRINT_TOKEN(Tokens::GreaterOrEq);
    PRINT_TOKEN(Tokens::Less);
    PRINT_TOKEN(Tokens::Greater);
    PRINT_TOKEN(Tokens::None);
    PRINT_TOKEN(Tokens::True);
    PRINT_TOKEN(Tokens::False);
    PRINT_TOKEN(Tokens::Colon);
    PRINT_TOKEN(Tokens::Comma);

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

    if (lhs.Is<Tokens::String>())
        return lhs.As<Tokens::String>().value == lhs.As<Tokens::String>().value;

    return true;
}
