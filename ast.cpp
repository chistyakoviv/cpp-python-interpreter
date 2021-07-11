#include "ast.h"

namespace AST {

ObjectHolder Add::Evaluate()
{
    ObjectHolder left = m_Left->Evaluate();
    ObjectHolder right = m_Right->Evaluate();

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

ObjectHolder Sub::Evaluate()
{
    ObjectHolder left = m_Left->Evaluate();
    ObjectHolder right = m_Right->Evaluate();

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

ObjectHolder Mul::Evaluate()
{
    ObjectHolder left = m_Left->Evaluate();
    ObjectHolder right = m_Right->Evaluate();

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

ObjectHolder Div::Evaluate()
{
    ObjectHolder left = m_Left->Evaluate();
    ObjectHolder right = m_Right->Evaluate();

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

}
