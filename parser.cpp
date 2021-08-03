#include "parser.h"
#include "comparators.h"

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
    if (m_CurrentToken.Is<Tokens::Class>())
    {
        Consume<Tokens::Class>();
        return ParseClassDefinition();
    }
    else if (m_CurrentToken.Is<Tokens::If>())
    {
        return ParseCondition();
    }
    std::unique_ptr<AST::Node> statement = ParseSimpleStatement();
    Consume<Tokens::NewLine>();
    return statement;
}

std::unique_ptr<AST::Node> Parser::ParseClassDefinition()
{
    Consume<Tokens::Class>();
    Token classToken = m_CurrentToken;
    Consume<Tokens::Id>();
    std::string className = classToken.As<Tokens::Id>().value;

    const Runtime::Class* baseClass = nullptr;
    if (m_CurrentToken.Is<Tokens::Lparen>())
    {
        Consume<Tokens::Lparen>();
        Token baseClassToken = m_CurrentToken;
        Consume<Tokens::Id>();
        Consume<Tokens::Rparen>();

        std::string baseClassName = baseClassToken.As<Tokens::Id>().value;

        if (auto it = m_DeclaredClasses.find(baseClassName); it == m_DeclaredClasses.end())
        {
            throw std::runtime_error("Base class " + baseClassName + " not found for class " + className);
        }
        else
        {
            baseClass = static_cast<const Runtime::Class*>(it->second.Get());
        }
    }

    Consume<Tokens::Colon>();
    Consume<Tokens::NewLine>();
    Consume<Tokens::Indent>();
    std::vector<Runtime::Method> methods = ParseMethods();
    Consume<Tokens::Dedent>();

    auto [it, inserted] = m_DeclaredClasses.insert({
        className,
        ObjectHolder::Own(
            Runtime::Class(className, std::move(methods), baseClass)
        )
    });

    if (!inserted)
        throw std::runtime_error("Class " + className + " already exists");

    return std::make_unique<AST::ClassDefinition>(it->second);
}

std::vector<Runtime::Method> Parser::ParseMethods()
{
    std::vector<Runtime::Method> methods;

    while (m_CurrentToken.Is<Tokens::Def>())
    {
        Runtime::Method method;
        Consume<Tokens::Def>();
        Token methodNameToken = m_CurrentToken;
        Consume<Tokens::Id>();
        method.name = methodNameToken.As<Tokens::Id>().value;
        Consume<Tokens::Lparen>();

        if (m_CurrentToken.Is<Tokens::Id>())
        {
            do
            {
                Token paramToken = m_CurrentToken;
                Consume<Tokens::Id>();
                method.formalParams.push_back(paramToken.As<Tokens::Id>().value);
            } while (m_CurrentToken.Is<Tokens::Comma>());
        }

        Consume<Tokens::Rparen>();
        Consume<Tokens::Colon>();

        method.body = ParseBlock();
        methods.push_back(std::move(method));
    }
    return methods;
}

std::unique_ptr<AST::Node> Parser::ParseCondition()
{
    Consume<Tokens::If>();
    std::unique_ptr<AST::Node> condition = ParseLogicalExpr();

    Consume<Tokens::Colon>();
    std::unique_ptr<AST::Node> ifBody = ParseBlock();

    std::unique_ptr<AST::Node> elseBody = ParseBlock();
    if (m_CurrentToken.Is<Tokens::Else>())
    {
        Consume<Tokens::Else>();
        Consume<Tokens::Colon>();
        elseBody = ParseBlock();
    }

    return std::make_unique<AST::IfElse>(std::move(condition), std::move(ifBody), std::move(elseBody));
}

std::unique_ptr<AST::Node> Parser::ParseBlock()
{
    Consume<Tokens::NewLine>();
    Consume<Tokens::Indent>();

    std::unique_ptr<AST::Compound> block = std::make_unique<AST::Compound>();

    while (!m_CurrentToken.Is<Tokens::Dedent>())
    {
        block->Add(ParseStatement());
    }

    Consume<Tokens::Dedent>();
    return block;
}

std::unique_ptr<AST::Node> Parser::ParseSimpleStatement()
{
    if (m_CurrentToken.Is<Tokens::Return>())
    {
        Consume<Tokens::Return>();
        return std::make_unique<AST::Return>(ParseLogicalExpr());
    }
    else if (m_CurrentToken.Is<Tokens::Print>())
    {
        Consume<Tokens::Print>();
        Consume<Tokens::Lparen>();

        std::vector<std::unique_ptr<AST::Node>> args;
        if (!m_CurrentToken.Is<Tokens::Rparen>())
        {
            args = ParseLogicalExprList();
        }

        Consume<Tokens::Rparen>();
        return std::make_unique<AST::Print>(std::move(args));
    }
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
            throw std::runtime_error("The language doesn't support functions");
        }

        std::vector<std::unique_ptr<AST::Node>> args;
        if (!m_CurrentToken.Is<Tokens::Rparen>())
        {
            args = ParseLogicalExprList();
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

std::vector<std::unique_ptr<AST::Node>> Parser::ParseLogicalExprList()
{
    std::vector<std::unique_ptr<AST::Node>> result;
    result.push_back(ParseLogicalExpr());
    while (m_CurrentToken.Is<Tokens::Comma>())
    {
        Consume<Tokens::Comma>();
        result.push_back(ParseLogicalExpr());
    }
    return result;
}

std::unique_ptr<AST::Node> Parser::ParseLogicalExpr()
{
    std::unique_ptr<AST::Node> node = ParseAndTest();
    while (m_CurrentToken.Is<Tokens::Or>())
    {
        Consume<Tokens::Or>();
        node = std::make_unique<AST::Or>(std::move(node), ParseAndTest());
    }
    return node;
}

std::unique_ptr<AST::Node> Parser::ParseAndTest()
{
    std::unique_ptr<AST::Node> node = ParseNotTest();
    while (m_CurrentToken.Is<Tokens::And>())
    {
        Consume<Tokens::And>();
        node = std::make_unique<AST::And>(std::move(node), ParseNotTest());
    }
    return node;
}

std::unique_ptr<AST::Node> Parser::ParseNotTest()
{
    if (m_CurrentToken.Is<Tokens::Not>())
    {
        Consume<Tokens::Not>();
        return std::make_unique<AST::Not>(ParseNotTest());
    }
    else
    {
        return ParseComparison();
    }
}

std::unique_ptr<AST::Node> Parser::ParseComparison()
{
    std::unique_ptr<AST::Node> node = ParseExpr();

    if (m_CurrentToken.Is<Tokens::Less>())
    {
        Consume<Tokens::Less>();
        node = std::make_unique<AST::Comparison>(Runtime::Less, std::move(node), ParseExpr());
    }
    else if (m_CurrentToken.Is<Tokens::LessOrEq>())
    {
        Consume<Tokens::LessOrEq>();
        node = std::make_unique<AST::Comparison>(Runtime::LessOrEqual, std::move(node), ParseExpr());
    }
    else if (m_CurrentToken.Is<Tokens::Greater>())
    {
        Consume<Tokens::Greater>();
        node = std::make_unique<AST::Comparison>(Runtime::Greater, std::move(node), ParseExpr());
    }
    else if (m_CurrentToken.Is<Tokens::GreaterOrEq>())
    {
        Consume<Tokens::GreaterOrEq>();
        node = std::make_unique<AST::Comparison>(Runtime::GreaterOrEqual, std::move(node), ParseExpr());
    }
    else if (m_CurrentToken.Is<Tokens::Eq>())
    {
        Consume<Tokens::Eq>();
        node = std::make_unique<AST::Comparison>(Runtime::Equal, std::move(node), ParseExpr());
    }
    else if (m_CurrentToken.Is<Tokens::NotEq>())
    {
        Consume<Tokens::NotEq>();
        node = std::make_unique<AST::Comparison>(Runtime::NotEqual, std::move(node), ParseExpr());
    }

    return node;
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
