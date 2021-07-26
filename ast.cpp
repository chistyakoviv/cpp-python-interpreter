#include "ast.h"
#include <iostream>

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

ObjectHolder VariableValue::Evaluate(Runtime::Closure& closure)
{
    Runtime::Closure* currentClosure = &closure;

    for (size_t i = 0; i + 1 < m_DottedIds.size(); i++)
    {
        if (auto it = currentClosure->find(m_DottedIds[i]); it == currentClosure->end())
        {
            throw std::runtime_error("Name " + m_DottedIds[i] + " not found in the scope");
        }
        else if (auto result = it->second.TryAs<Runtime::ClassInstance>())
        {
            currentClosure = &result->GetFields();
        }
        else
        {
            throw std::runtime_error(m_DottedIds[i] + " is not a class instance");
        }
    }

    if (auto it = currentClosure->find(m_DottedIds.back()); it != currentClosure->end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("Variable " + m_DottedIds.back() + " not found in closure");
    }
}

std::ostream* Print::s_Output = &std::cout;

std::unique_ptr<Print> Print::Variable(std::string name)
{
    return std::make_unique<Print>(std::make_unique<VariableValue>(std::move(name)));
}

ObjectHolder Print::Evaluate(Runtime::Closure& closure)
{
    bool first = true;
    for (const auto& arg : m_Args)
    {
        if (!first)
        {
            (*s_Output) << ' ';
        }
        first = false;

        if (ObjectHolder result = arg->Evaluate(closure))
        {
            result->Print(*s_Output);
        }
        else
        {
            (*s_Output) << "None";
        }
    }
    (*s_Output) << '\n';
    return ObjectHolder::None();
}

void Print::SetOutputStream(std::ostream& os)
{
    s_Output = &os;
}

}
