#include "parser.h"

std::unique_ptr<AST::Node> Parser::ParseProgram()
{
    std::unique_ptr<AST::Compound> statements = std::make_unique<AST::Compound>();
    while (!m_CurrentToken.Is<Tokens::Eof>())
    {
        statements->Add(ParseStatement());
    }
    return statements;
}

std::unique_ptr<AST::Node> Parser::ParseStatement()
{
    std::unique_ptr<AST::Node> statement = ParseSimpleStatement();
    Consume<Tokens::NewLine>();
    return statement;
}

std::unique_ptr<AST::Node> Parser::ParseSimpleStatement()
{
    return ParseAssignmentStatementOrCall();
}

std::unique_ptr<AST::Node> Parser::ParseAssignmentStatementOrCall()
{
    std::vector<std::string> idList = ParseDottedIds();
    std::string varName = idList.back();
    idList.pop_back();

    if (m_CurrentToken.Is<Tokens::Assign>())
    {
        Consume<Tokens::Assign>();
        if (idList.empty())
        {
            return std::make_unique<AST::Assign>(std::move(varName), ParseExpr());
        }
        else
        {
            return std::make_unique<AST::FieldAssign>(
                std::make_unique<AST::VariableValue>(std::move(idList)),
                std::move(varName),
                ParseExpr()
            );
        }
    }
    else
    {
        Consume<Tokens::Lparen>();

        if (idList.empty())
        {
            throw std::runtime_error("The language doesn't suppor functions");
        }

        std::vector<std::unique_ptr<AST::Node>> args;
        if (!m_CurrentToken.Is<Tokens::Rparen>())
        {
            // args = ParseTestList();
        }

        Consume<Tokens::Rparen>();

        return std::make_unique<AST::MethodCall>(
            std::make_unique<AST::VariableValue>(std::move(idList)),
            std::move(varName),
            std::move(args)
        );
    }
}

std::vector<std::string> Parser::ParseDottedIds()
{
    std::vector<std::string> result(1, m_CurrentToken.As<Tokens::Id>().value);
    Consume<Tokens::Id>();
    while (m_CurrentToken.Is<Tokens::Dot>())
    {
        Consume<Tokens::Dot>();
        Token token = m_CurrentToken;
        Consume<Tokens::Id>();
        result.push_back(token.As<Tokens::Id>().value);
    }
    return result;
}

std::unique_ptr<AST::Node> Parser::ParseExpr()
{
    std::unique_ptr<AST::Node> node = ParseTerm();
    while (m_CurrentToken.Is<Tokens::Plus>() || m_CurrentToken.Is<Tokens::Minus>())
    {
        if (m_CurrentToken.Is<Tokens::Plus>())
        {
            Consume<Tokens::Plus>();
            node = std::make_unique<AST::Add>(std::move(node), ParseTerm());
        }
        else if (m_CurrentToken.Is<Tokens::Minus>())
        {
            Consume<Tokens::Minus>();
            node = std::make_unique<AST::Sub>(std::move(node), ParseTerm());
        }
    }

    return node;
}

std::unique_ptr<AST::Node> Parser::ParseTerm()
{
    std::unique_ptr<AST::Node> node = ParseFactor();
    while (m_CurrentToken.Is<Tokens::Div>() || m_CurrentToken.Is<Tokens::Mul>())
    {
        if (m_CurrentToken.Is<Tokens::Mul>())
        {
            Consume<Tokens::Mul>();
            node = std::make_unique<AST::Mul>(std::move(node), ParseFactor());
        }
        else if (m_CurrentToken.Is<Tokens::Div>())
        {
            Consume<Tokens::Div>();
            node = std::make_unique<AST::Div>(std::move(node), ParseFactor());
        }
    }

    return node;
}

std::unique_ptr<AST::Node> Parser::ParseFactor()
{
    std::unique_ptr<AST::Node> node;
    Token token = m_CurrentToken;
    if (m_CurrentToken.Is<Tokens::Plus>())
    {
        Consume<Tokens::Plus>();
        node = std::make_unique<AST::Positive>(ParseFactor());
    }
    else if (m_CurrentToken.Is<Tokens::Minus>())
    {
        Consume<Tokens::Minus>();
        node = std::make_unique<AST::Negate>(ParseFactor());
    }
    else if (m_CurrentToken.Is<Tokens::Integer>())
    {
        Consume<Tokens::Integer>();
        node = std::make_unique<AST::NumericConst>(token.As<Tokens::Integer>().value);
    }
    else if (m_CurrentToken.Is<Tokens::Lparen>())
    {
        Consume<Tokens::Lparen>();
        node = ParseExpr();
        Consume<Tokens::Rparen>();
    }
    else
    {
        node = std::make_unique<AST::VariableValue>(ParseDottedIds().back());
    }

    return node;
}
