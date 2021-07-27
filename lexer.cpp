#include <algorithm>
#include <unordered_map>
#include <string>
#include <iomanip>
#include "lexer.h"

const int Reader::Eof = std::istream::traits_type::eof();

int Reader::Next()
{
    if (!m_Input)
        return Eof;

    m_Column++;

    char ch;
    if (m_Line >> std::noskipws >> ch)
        return ch;

    return '\n';
}

void Reader::NextLine()
{
    auto isSpace = [](char c) { return std::isspace(c); };

    for (std::string line; std::getline(m_Input, line); )
    {
        m_Lineno++;

        auto it = std::find_if_not(line.begin(), line.end(), isSpace);
        if (it != line.end())
        {
            int spacesCount = it - line.begin();

            if (spacesCount % 2 == 1)
                throw std::runtime_error("Odd number of spaces in line: " + line);

            m_Indent = spacesCount / 2;
            m_Line = std::istringstream(std::string(it, line.end()));
            m_Column = 0;
            return;
        }
    }

    m_Column = 0;
    m_Indent = 0;
}

void Lexer::Advance()
{
    m_CurrentChar = m_Reader.Next();
}

Token Lexer::GetNextToken()
{
    static const std::unordered_map<std::string, Token> keywords = {
        {"class", Token{Tokens::Class{}}},
        {"def", Token{Tokens::Def{}}},
        {"return", Token{Tokens::Return{}}},
        {"print", Token{Tokens::Print{}}},
        {"if", Token{Tokens::If{}}},
        {"else", Token{Tokens::Else{}}},
        {"and", Token{Tokens::And{}}},
        {"or", Token{Tokens::Or{}}},
        {"not", Token{Tokens::Not{}}},
        {"True", Token{Tokens::True{}}},
        {"False", Token{Tokens::False{}}},
        {"None", Token{Tokens::None{}}},
    };

    while (m_CurrentChar != Reader::Eof)
    {
        if (m_CurrentIndent < m_Reader.GetIndent())
        {
            m_CurrentIndent++;
            return Token{Tokens::Indent{}};
        }
        else if (m_CurrentIndent > m_Reader.GetIndent())
        {
            m_CurrentIndent--;
            return Token{Tokens::Dedent{}};
        }
        else if (m_CurrentChar == '\n')
        {
            m_Reader.NextLine();
            Advance();
            return Token{Tokens::NewLine{}};
        }
        else if (std::isspace(m_CurrentChar))
        {
            do
            {
                Advance();
            } while (std::isspace(m_CurrentChar));
        }
        else if (std::isdigit(m_CurrentChar))
        {
            int value = 0;
            do
            {
                value = value * 10 + m_CurrentChar - '0';
                Advance();
            } while (std::isdigit(m_CurrentChar));

            return Token{Tokens::Integer{value}};
        }
        else if (std::isalpha(m_CurrentChar))
        {
            std::string value;
            do
            {
                value += m_CurrentChar;
                Advance();
            } while (std::isalnum(m_CurrentChar));

            if (auto it = keywords.find(value); it != keywords.end())
            {
                return it->second;
            }
            else
            {
                return Token{Tokens::Id{value}};
            }
        }
        else if (m_CurrentChar == '=')
        {
            Advance();
            return Token{Tokens::Assign{}};
        }
        else if (m_CurrentChar == '+')
        {
            Advance();
            return Token{Tokens::Plus{}};
        }
        else if (m_CurrentChar == '-')
        {
            Advance();
            return Token{Tokens::Minus{}};
        }
        else if (m_CurrentChar == '*')
        {
            Advance();
            return Token{Tokens::Mul{}};
        }
        else if (m_CurrentChar == '/')
        {
            Advance();
            return Token{Tokens::Div{}};
        }
        else if (m_CurrentChar == '(')
        {
            Advance();
            return Token{Tokens::Lparen{}};
        }
        else if (m_CurrentChar == ')')
        {
            Advance();
            return Token{Tokens::Rparen{}};
        }
        else if (m_CurrentChar == ':')
        {
            Advance();
            return Token{Tokens::Colon{}};
        }
        else
        {
            throw std::runtime_error("Unxpected token");
        }
    }

    return Token{Tokens::Eof{}};
}
