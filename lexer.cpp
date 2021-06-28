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

    NextLine();

    return '\n';
}

int Reader::Get()
{
    if (!m_Input)
        return Eof;

    char ch = m_Line.get();

    if (ch == Eof)
    {
        return '\n';
    }

    return ch;
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

Token Lexer::GetNextToken()
{
}

void Lexer::Advance()
{
    m_CurrentChar = m_Reader.Next();
}