#pragma once

#include <iostream>

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

}

using Object = Runtime::Object;
