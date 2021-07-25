#include "object.h"
#include "object_holder.h"
#include "ast.h"
#include <sstream>

namespace Runtime {

void Bool::Print(std::ostream& os)
{
    os << (GetValue() ? "True" : "False");
}

void ClassInstance::Print(std::ostream& os)
{
    if (HasMethod("__str__", 0))
    {
        Call("__str__", {})->Print(os);
    }
    else
    {
        os << this;
    }
}

bool ClassInstance::HasMethod(const std::string& method, size_t argsCount) const
{
    const Method* m = m_Class.GetMethod(method);
    return m && (m->formalParams).size() == argsCount;
}

ObjectHolder ClassInstance::Call(const std::string& method, const std::vector<ObjectHolder>& actualParams)
{
    const Method* m = m_Class.GetMethod(method);
    if (!m)
    {
        throw std::runtime_error("Class " + m_Class.GetName() + " doesn't have method " + method);
    }
    else if (m->formalParams.size() != actualParams.size())
    {
        std::ostringstream msg;
        msg << "Method " << m_Class.GetName() << "::" << method << " requires " << m->formalParams.size()
            << " parameters, but " << actualParams.size() << " given";
        throw std::runtime_error(msg.str());
    }
    else
    {
        try
        {
            Closure closure;
            for (size_t i = 0; i < actualParams.size(); i++)
            {
                closure[m->formalParams[i]] = actualParams[i];
            }
            return m->body->Evaluate(closure);
        }
        catch (ObjectHolder& returnedValue)
        {
            return returnedValue;
        }
        return ObjectHolder::None();
    }
}

Class::Class(std::string name, std::vector<Method> methods, const Class* parent)
    : m_Name(std::move(name)), m_Parent(parent)
{
    for (auto& m : methods)
    {
        if (m_VMT.find(m.name) != m_VMT.end())
        {
            throw std::runtime_error("Class " + m_Name + " has duplicate method " + m.name);
        }
        else
        {
            m_VMT[m.name] = std::move(m);
        }
    }
}

const Method* Class::GetMethod(const std::string& name) const
{
    if (auto it = m_VMT.find(name); it != m_VMT.end())
    {
        return &it->second;
    }
    else if (m_Parent)
    {
        return m_Parent->GetMethod(name);
    }
    else
    {
        return nullptr;
    }
}

void Class::Print(std::ostream& os)
{
    os << "Class name " << m_Name;
}

}
