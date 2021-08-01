#include <memory>
#include <vector>
#include "lexer.h"
#include "token.h"
#include "ast.h"

class Parser
{
public:
    Parser(Lexer& lexer)
        : m_Lexer(lexer), m_CurrentToken(m_Lexer.GetNextToken())
    {
    }

    std::unique_ptr<AST::Node> ParseProgram();

private:

    std::unique_ptr<AST::Node> ParseBoolExpr();
    std::unique_ptr<AST::Node> ParseExpr();
    std::unique_ptr<AST::Node> ParseTerm();
    std::unique_ptr<AST::Node> ParseFactor();
    std::unique_ptr<AST::Node> ParseStatement();
    std::unique_ptr<AST::Node> ParseSimpleStatement();
    std::unique_ptr<AST::Node> ParseAssignmentStatementOrCall();
    std::vector<std::unique_ptr<AST::Node>> ParseLogicalExprList();
    std::unique_ptr<AST::Node> ParseLogicalExpr();
    std::unique_ptr<AST::Node> ParseAndTest();
    std::unique_ptr<AST::Node> ParseNotTest();
    std::unique_ptr<AST::Node> ParseComparison();
    std::vector<std::string> ParseDottedIds();

    template<typename T>
    void Consume()
    {
        if (m_CurrentToken.Is<T>())
            m_CurrentToken = m_Lexer.GetNextToken();
        else
            throw std::runtime_error("Unxpected Token at line");
    }

private:
    Lexer& m_Lexer;
    Token m_CurrentToken;
};
