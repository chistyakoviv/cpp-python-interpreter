#include "ast.h"

namespace AST {

ObjectHolder Add::Evaluate(Closure& closure)
{
    ObjectHolder left = m_Left->Evaluate(closure);
    ObjectHolder right = m_Right->Evaluate(closure);

    const Runtime::Number* leftNumber = left.TryAs<Runtime::Number>();
    const Runtime::Number* rightNumber = right.TryAs<Runtime::Number>();

    if (leftNumber && rightNumber)
    {
        return ObjectHolder::Own<Runtime::Number>(
            Runtime::Number(leftNumber->GetValue() + rightNumber->GetValue())
        );
    }

    throw std::runtime_error("Addition isn't supported for these operands");
}

ObjectHolder Sub::Evaluate(Closure& closure)
{
    ObjectHolder left = m_Left->Evaluate(closure);
    ObjectHolder right = m_Right->Evaluate(closure);

    const Runtime::Number* leftNumber = left.TryAs<Runtime::Number>();
    const Runtime::Number* rightNumber = right.TryAs<Runtime::Number>();

    if (leftNumber && rightNumber)
    {
        return ObjectHolder::Own<Runtime::Number>(
            Runtime::Number(leftNumber->GetValue() - rightNumber->GetValue())
        );
    }

    throw std::runtime_error("Substraction isn't supported for these operands");
}

ObjectHolder Mul::Evaluate(Closure& closure)
{
    ObjectHolder left = m_Left->Evaluate(closure);
    ObjectHolder right = m_Right->Evaluate(closure);

    const Runtime::Number* leftNumber = left.TryAs<Runtime::Number>();
    const Runtime::Number* rightNumber = right.TryAs<Runtime::Number>();

    if (leftNumber && rightNumber)
    {
        return ObjectHolder::Own<Runtime::Number>(
            Runtime::Number(leftNumber->GetValue() * rightNumber->GetValue())
        );
    }

    throw std::runtime_error("Multiplication isn't supported for these operands");
}

ObjectHolder Div::Evaluate(Closure& closure)
{
    ObjectHolder left = m_Left->Evaluate(closure);
    ObjectHolder right = m_Right->Evaluate(closure);

    const Runtime::Number* leftNumber = left.TryAs<Runtime::Number>();
    const Runtime::Number* rightNumber = right.TryAs<Runtime::Number>();

    if (rightNumber->GetValue() == 0)
        throw std::runtime_error("Division by zero");

    if (leftNumber && rightNumber)
    {
        return ObjectHolder::Own<Runtime::Number>(
            Runtime::Number(leftNumber->GetValue() / rightNumber->GetValue())
        );
    }

    throw std::runtime_error("Division isn't supported for these operands");
}

ObjectHolder Negate::Evaluate(Closure& closure)
{
    ObjectHolder node = m_Arg->Evaluate(closure);
    const Runtime::Number* number = node.TryAs<Runtime::Number>();

    if (number)
    {
        return ObjectHolder::Own<Runtime::Number>(
            Runtime::Number(-number->GetValue())
        );
    }

    throw std::runtime_error("Operation isn't supported");
}

ObjectHolder Positive::Evaluate(Closure& closure)
{
    ObjectHolder node = m_Arg->Evaluate(closure);
    const Runtime::Number* number = node.TryAs<Runtime::Number>();

    if (number)
    {
        return ObjectHolder::Own<Runtime::Number>(
            Runtime::Number(number->GetValue())
        );
    }

    throw std::runtime_error("Operation isn't supported");
}

ObjectHolder Compound::Evaluate(Closure& closure)
{
    for (auto& node : m_Nodes)
    {
        node->Evaluate(closure);
    }
    return ObjectHolder::None();
}

ObjectHolder Assign::Evaluate(Closure& closure)
{
    return closure[m_VarName] = m_Expr->Evaluate(closure);
}

}
