#include "parser.h"

namespace {
    // bool operator==(const Token& token, char c)
    // {
    //     auto ptr = token.TryAs<Tokens::Char>();
    //     return ptr && ptr->value == c;
    // }

    // bool operator!=(const Token& token, char c)
    // {
    //     return !(token == c);
    // }
}

std::unique_ptr<AST::Node> Parser::ParseProgram()
{
    std::unique_ptr<AST::Compound> statements = std::make_unique<AST::Compound>();
    while (!m_CurrentToken.Is<Tokens::Eof>())
    {
        statements->Add(StatementList());
    }
    return statements;
}

std::unique_ptr<AST::Node> Parser::StatementList()
{
    std::unique_ptr<AST::Node> statement = Statement();
    Consume<Tokens::NewLine>();
    return statement;
}

std::unique_ptr<AST::Node> Parser::Statement()
{
    return AssignmentStatement();
}

std::unique_ptr<AST::Node> Parser::AssignmentStatement()
{
    std::string var = Variable();
    Consume<Tokens::Assign>();
    return std::make_unique<AST::Assign>(std::move(var), Expr());
}

std::string Parser::Variable()
{
    Token token = m_CurrentToken;
    Consume<Tokens::Id>();
    return token.As<Tokens::Id>().value;
}

std::unique_ptr<AST::Node> Parser::Expr()
{
    std::unique_ptr<AST::Node> node = Term();
    while (m_CurrentToken.Is<Tokens::Plus>() || m_CurrentToken.Is<Tokens::Minus>())
    {
        if (m_CurrentToken.Is<Tokens::Plus>())
        {
            Consume<Tokens::Plus>();
            node = std::make_unique<AST::Add>(std::move(node), Term());
        }
        else if (m_CurrentToken.Is<Tokens::Minus>())
        {
            Consume<Tokens::Minus>();
            node = std::make_unique<AST::Sub>(std::move(node), Term());
        }
    }

    return node;
}

std::unique_ptr<AST::Node> Parser::Term()
{
    std::unique_ptr<AST::Node> node = Factor();
    while (m_CurrentToken.Is<Tokens::Div>() || m_CurrentToken.Is<Tokens::Mul>())
    {
        if (m_CurrentToken.Is<Tokens::Mul>())
        {
            Consume<Tokens::Mul>();
            node = std::make_unique<AST::Mul>(std::move(node), Factor());
        }
        else if (m_CurrentToken.Is<Tokens::Div>())
        {
            Consume<Tokens::Div>();
            node = std::make_unique<AST::Div>(std::move(node), Factor());
        }
    }

    return node;
}

std::unique_ptr<AST::Node> Parser::Factor()
{
    std::unique_ptr<AST::Node> node;
    Token token = m_CurrentToken;
    if (m_CurrentToken.Is<Tokens::Plus>())
    {
        Consume<Tokens::Plus>();
        node = std::make_unique<AST::Positive>(Factor());
    }
    else if (m_CurrentToken.Is<Tokens::Minus>())
    {
        Consume<Tokens::Minus>();
        node = std::make_unique<AST::Negate>(Factor());
    }
    else if (m_CurrentToken.Is<Tokens::Integer>())
    {
        Consume<Tokens::Integer>();
        node = std::make_unique<AST::NumericConst>(token.As<Tokens::Integer>().value);
    }
    else if (m_CurrentToken.Is<Tokens::Lparen>())
    {
        Consume<Tokens::Lparen>();
        node = Expr();
        Consume<Tokens::Rparen>();
    }
    else
    {
        node = std::make_unique<AST::VariableValue>(Variable());
    }

    return node;
}
