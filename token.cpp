#include "token.h"

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    #define PRINT_TOKEN_WITH_VALUE(type) if (const type* ptr = token.TryAs<type>()) \
                                            os << #type << " {" << ptr->value << "}"

    PRINT_TOKEN_WITH_VALUE(Tokens::Integer);

    #undef PRINT_TOKEN_WITH_VALUE

    #define PRINT_TOKEN(type) if (token.Is<type>()) \
                                os << #type

    PRINT_TOKEN(Tokens::Eof);
    PRINT_TOKEN(Tokens::NewLine);
    PRINT_TOKEN(Tokens::Plus);
    PRINT_TOKEN(Tokens::Minus);

    #undef PRINT_TOKEN

    return os << '\n';
}
