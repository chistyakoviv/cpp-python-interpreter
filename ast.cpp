#include "ast.h"
#include <iostream>
#include <sstream>

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

ObjectHolder Or::Evaluate(Runtime::Closure& closure)
{
    if (Runtime::IsTrue(m_Left->Evaluate(closure)) || Runtime::IsTrue(m_Right->Evaluate(closure)))
    {
        return ObjectHolder::Own(Runtime::Bool(true));
    }
    else
    {
        return ObjectHolder::Own(Runtime::Bool(false));
    }
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

ObjectHolder Not::Evaluate(Runtime::Closure& closure)
{
    return ObjectHolder::Own(Runtime::Bool(!Runtime::IsTrue(m_Arg->Evaluate(closure))));
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

ObjectHolder MethodCall::Evaluate(Runtime::Closure& closure)
{
    std::vector<ObjectHolder> actualParams;
    for (const auto& arg : m_Args)
    {
        actualParams.push_back(arg->Evaluate(closure));
    }

    ObjectHolder calee = m_Object->Evaluate(closure);
    if (Runtime::ClassInstance* instance = calee.TryAs<Runtime::ClassInstance>())
    {
        return instance->Call(m_Method, actualParams);
    }
    else
    {
        throw std::runtime_error("Trying to call method " + m_Method + " on an object that is not a class instance");
    }
}

ObjectHolder NewInstance::Evaluate(Runtime::Closure& closure)
{
    Runtime::ClassInstance instance(m_Class);

    if (const Runtime::Method* m = m_Class.GetMethod("__init__"); m)
    {
        std::vector<ObjectHolder> actualParams;
        for (const auto& arg : m_Args)
        {
            actualParams.push_back(arg->Evaluate(closure));
        }

        instance.Call("__init__", actualParams);
    }

    return ObjectHolder::Own(std::move(instance));
}

ObjectHolder Stringify::Evaluate(Runtime::Closure& closure)
{
    ObjectHolder value = m_Arg->Evaluate(closure);

    std::ostringstream os;
    value->Print(os);

    return ObjectHolder::Own(Runtime::String(os.str()));
}

ObjectHolder Return::Evaluate(Runtime::Closure& closure)
{
    return m_Node->Evaluate(closure);
}

ObjectHolder ClassDefinition::Evaluate(Runtime::Closure& closure)
{
    closure[m_ClassName] = m_Class;
    return ObjectHolder::None();
}

}
