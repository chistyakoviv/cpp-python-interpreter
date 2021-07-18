#pragma once

#include <iostream>
#include <sstream>
#include "token.h"

class Reader
{
public:
    Reader(std::istream& input)
        : m_Input(input), m_Lineno(0), m_Column(0), m_Indent(0)
    {
        NextLine();
    }

    int Next();
    void NextLine();

    int GetLineno() const
    {
        return m_Lineno;
    }

    int GetColumn() const
    {
        return m_Column;
    }

    int GetIndent() const
    {
        return m_Indent;
    }

    static const int Eof;
private:
private:
    std::istream& m_Input;
    std::istringstream m_Line;
    int m_Lineno;
    int m_Column;
    int m_Indent;
};

class Lexer
{
public:
    Lexer(std::istream& input)
     : m_Reader(input), m_CurrentChar(m_Reader.Next())
    {
    }

    void Advance();
    Token GetNextToken();

private:
    Reader m_Reader;
    char m_CurrentChar;
};
