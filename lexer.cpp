#include <algorithm>
#include "lexer.h"

const int Reader::Eof = std::istream::traits_type::eof();

int Reader::Next()
{
    if (!m_Input)
        return Eof;

    char ch;
    if (m_Line >> ch)
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
    while (m_CurrentChar != Reader::Eof)
    {
        if (m_CurrentChar == '\n')
        {
            m_Reader.NextLine();
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
        else
        {
            throw std::runtime_error("Unxpected token");
        }
    }

    return Token{Tokens::Eof{}};
}
