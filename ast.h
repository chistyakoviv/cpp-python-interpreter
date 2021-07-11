#pragma once

#include <memory>
#include "token.h"
#include "object.h"
#include "object_holder.h"

namespace AST {

class Node
{
public:
    virtual ~Node() = default;
    virtual ObjectHolder Evaluate() = 0;
};

template<typename T>
class ValueNode : public Node
{
public:
    ValueNode(T value)
        : m_Value(std::move(value))
    {
    }

    ObjectHolder Evaluate() override {
        return ObjectHolder::Share(m_Value);
    }

private:
    T m_Value;
};

using NumericConst = ValueNode<Runtime::Number>;

class BinaryOperation : public Node
{
public:
    BinaryOperation(std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : m_Left(std::move(left)), m_Right(std::move(right))
    {
    }

protected:
    std::unique_ptr<Node> m_Left, m_Right;
};

class Add : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    ObjectHolder Evaluate() override;
};

class Sub : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    ObjectHolder Evaluate() override;
};

class Mul : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    ObjectHolder Evaluate() override;
};

class Div : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    ObjectHolder Evaluate() override;
};

}
