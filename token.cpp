#include "token.h"

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    os << "token type: " << static_cast<std::underlying_type<TokenType>::type>(token.GetType()) << " token value: ";
    if (std::holds_alternative<int>(token.GetValue()))
    {
        os << std::get<int>(token.GetValue());
    }
    else if (std::holds_alternative<char>(token.GetValue()))
    {
        os << std::get<char>(token.GetValue());
    }
    else if (std::holds_alternative<std::string>(token.GetValue()))
    {
        os << std::get<std::string>(token.GetValue());
    }

    return os << '\n';
}
