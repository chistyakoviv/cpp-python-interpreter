#pragma once

#include <memory>
#include <vector>
#include "token.h"
#include "object.h"
#include "object_holder.h"

namespace AST {

class Node
{
public:
    virtual ~Node() = default;
    virtual ObjectHolder Evaluate(Runtime::Closure& closure) = 0;
};

template<typename T>
class ValueNode : public Node
{
public:
    ValueNode(T value)
        : m_Value(std::move(value))
    {
    }

    ObjectHolder Evaluate(Runtime::Closure& closure) override {
        return ObjectHolder::Share(m_Value);
    }

private:
    T m_Value;
};

using NumericConst = ValueNode<Runtime::Number>;
using StringConst = ValueNode<Runtime::String>;
using BoolConst = ValueNode<Runtime::Bool>;

class VariableValue : public Node
{
public:
    VariableValue(std::string varName)
    {
        m_DottedIds.push_back(std::move(varName));
    }

    VariableValue(std::vector<std::string> dottedIds)
        : m_DottedIds(std::move(dottedIds))
    {
    }

    ObjectHolder Evaluate(Runtime::Closure& closure) override;
private:
    std::vector<std::string> m_DottedIds;
};

class BinaryOp : public Node
{
public:
    BinaryOp(std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : m_Left(std::move(left)), m_Right(std::move(right))
    {
    }

protected:
    std::unique_ptr<Node> m_Left, m_Right;
};

class Add : public BinaryOp
{
public:
    using BinaryOp::BinaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class Sub : public BinaryOp
{
public:
    using BinaryOp::BinaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class Mul : public BinaryOp
{
public:
    using BinaryOp::BinaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class Div : public BinaryOp
{
public:
    using BinaryOp::BinaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class And : public BinaryOp
{
    using BinaryOp::BinaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class Or : public BinaryOp
{
    using BinaryOp::BinaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class UnaryOp : public Node
{
public:
    UnaryOp(std::unique_ptr<Node> arg)
        : m_Arg(std::move(arg))
    {
    }
protected:
    std::unique_ptr<Node> m_Arg;
};

class Negate : public UnaryOp
{
    using UnaryOp::UnaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class Positive : public UnaryOp
{
    using UnaryOp::UnaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class Not : public UnaryOp
{
    using UnaryOp::UnaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class Compound : public Node
{
public:
    template<typename ...Args>
    Compound(Args&& ...args)
    {
        (m_Nodes.push_back(std::forward<Args>(args)), ...);
    }

    void Add(std::unique_ptr<Node> node)
    {
        m_Nodes.push_back(std::move(node));
    }

    ObjectHolder Evaluate(Runtime::Closure& closure) override;
private:
    std::vector<std::unique_ptr<Node>> m_Nodes;
};

class Assign : public Node
{
public:
    Assign(std::string varName, std::unique_ptr<AST::Node> expr)
        : m_VarName(std::move(varName)), m_Expr(std::move(expr))
    {
    }

    ObjectHolder Evaluate(Runtime::Closure& closure) override;
private:
    std::string m_VarName;
    std::unique_ptr<AST::Node> m_Expr;
};

class FieldAssign : public Node
{
public:
    FieldAssign(VariableValue object, std::string fieldName, std::unique_ptr<Node> expr)
        : m_Object(std::move(object)), m_FieldName(std::move(fieldName)), m_Expr(std::move(expr))
    {
    }

    ObjectHolder Evaluate(Runtime::Closure& closure) override;
private:
    VariableValue m_Object;
    std::string m_FieldName;
    std::unique_ptr<Node> m_Expr;
};

struct None : Node
{
    ObjectHolder Evaluate(Runtime::Closure& closure) override
    {
        return ObjectHolder::None();
    }
};

class Print : public Node
{
public:
    Print(std::unique_ptr<Node> arg)
    {
        m_Args.push_back(std::move(arg));
    }

    Print(std::vector<std::unique_ptr<Node>> args)
        : m_Args(std::move(args))
    {
    }

    static std::unique_ptr<Print> Variable(std::string name);

    ObjectHolder Evaluate(Runtime::Closure& closure) override;

    static void SetOutputStream(std::ostream& os);
private:
    std::vector<std::unique_ptr<Node>> m_Args;
    static std::ostream* s_Output;
};

class MethodCall : public Node
{
public:
    MethodCall(std::unique_ptr<Node> object, std::string method, std::vector<std::unique_ptr<Node>> args)
        : m_Object(std::move(object)), m_Method(std::move(method)), m_Args(std::move(args))
    {
    }

    ObjectHolder Evaluate(Runtime::Closure& closure) override;
private:
    std::unique_ptr<Node> m_Object;
    std::string m_Method;
    std::vector<std::unique_ptr<Node>> m_Args;
};

class NewInstance : public Node
{
public:
    NewInstance(const Runtime::Class& cls)
        : NewInstance(cls, {})
    {
    }

    NewInstance(const Runtime::Class& cls, std::vector<std::unique_ptr<Node>> args)
        : m_Class(cls), m_Args(std::move(args))
    {
    }

    ObjectHolder Evaluate(Runtime::Closure& closure) override;
private:
    const Runtime::Class& m_Class;
    std::vector<std::unique_ptr<Node>> m_Args;
};

class Stringify : public UnaryOp
{
public:
    using UnaryOp::UnaryOp;
    ObjectHolder Evaluate(Runtime::Closure& closure) override;
};

class Return : public Node
{
public:
    Return(std::unique_ptr<Node> node)
        : m_Node(std::move(node))
    {
    }

    ObjectHolder Evaluate(Runtime::Closure& closure) override;
private:
    std::unique_ptr<Node> m_Node;
};

class ClassDefinition : public Node
{
public:
    ClassDefinition(ObjectHolder cls)
        : m_Class(std::move(cls)), m_ClassName(dynamic_cast<const Runtime::Class&>(*cls).GetName())
    {
    }

    ObjectHolder Evaluate(Runtime::Closure& closure) override;
private:
    ObjectHolder m_Class;
    std::string m_ClassName;
};

}
