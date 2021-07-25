#include "ast.h"

namespace AST {

ObjectHolder Add::Evaluate(Runtime::Closure& closure)
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

ObjectHolder Sub::Evaluate(Runtime::Closure& closure)
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

ObjectHolder Mul::Evaluate(Runtime::Closure& closure)
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

ObjectHolder Div::Evaluate(Runtime::Closure& closure)
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

ObjectHolder Negate::Evaluate(Runtime::Closure& closure)
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

ObjectHolder Positive::Evaluate(Runtime::Closure& closure)
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

ObjectHolder Compound::Evaluate(Runtime::Closure& closure)
{
    for (auto& node : m_Nodes)
    {
        node->Evaluate(closure);
    }
    return ObjectHolder::None();
}

ObjectHolder Assign::Evaluate(Runtime::Closure& closure)
{
    return closure[m_VarName] = m_Expr->Evaluate(closure);
}

ObjectHolder FieldAssign::Evaluate(Runtime::Closure& closure)
{
    ObjectHolder instance = m_Object.Evaluate(closure);
    if (auto p = instance.TryAs<Runtime::ClassInstance>())
    {
        return p->GetFields()[m_FieldName] = m_Expr->Evaluate(closure);
    }
    else
    {
        throw std::runtime_error("Cannot assign a value to the field " + m_FieldName + " of not an object");
    }
}

}
