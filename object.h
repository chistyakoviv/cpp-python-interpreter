#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include "object_holder.h"

namespace AST {
    class Node;
}

namespace Runtime {

class Object
{
public:
    virtual ~Object() = default;
    virtual void Print(std::ostream& os) = 0;
};

template <typename T>
class ValueObject : public Object
{
public:
    ValueObject(T value)
        : m_Value(value)
    {
    }

    void Print(std::ostream& os) override
    {
        os << m_Value;
    }

    const T& GetValue() const
    {
        return m_Value;
    }
private:
    T m_Value;
};

using Number = ValueObject<int>;
using String = ValueObject<std::string>;

class Bool : public ValueObject<bool>
{
public:
    using ValueObject<bool>::ValueObject;
    void Print(std::ostream& os) override;
};

struct Method
{
    std::string name;
    std::vector<std::string> formalParams;
    std::unique_ptr<AST::Node> body;
};

class Class : public Object
{
public:
    Class(std::string name, std::vector<Method> methods, const Class* parent);

    const Method* GetMethod(const std::string& name) const;

    const std::string& GetName() const
    {
        return m_Name;
    }

    void Print(std::ostream& os) override;
private:
    std::string m_Name;
    const Class* m_Parent;
    std::unordered_map<std::string, Method> m_VMT;
};

class ClassInstance : public Object
{
public:
    ClassInstance(const Class& cls)
        : m_Class(cls)
    {
    }

    ObjectHolder Call(const std::string& method, const std::vector<ObjectHolder>& actualParams);
    bool HasMethod(const std::string& method, size_t argsCount) const;

    Closure& GetFields()
    {
        return m_Fields;
    }

    const Closure& GetFields() const
    {
        return m_Fields;
    }

    void Print(std::ostream& os) override;
private:
    const Class& m_Class;
    Closure m_Fields;
};

}
