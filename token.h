#include <variant>

enum class TokenType
{
    Eof,
    PLUS,
    INTEGER
};

using TokenValue = std::variant<int, char>;

class Token
{
public:
    Token(TokenType type, TokenValue value);
};
