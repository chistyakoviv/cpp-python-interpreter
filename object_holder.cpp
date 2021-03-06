#include "object_holder.h"
#include "object.h"

namespace Runtime {

std::ostream& operator<<(std::ostream& os, const Closure& closure)
{
    os << "Global scope" << '\n';
    for (auto it : closure)
    {
        os << it.first << ": ";
        it.second->Print(os);
        os << '\n';
    }
    return os << '\n';
}

ObjectHolder ObjectHolder::Share(Object& object)
{
    return ObjectHolder(std::shared_ptr<Object>(&object, [](Object*) { }));
}

ObjectHolder ObjectHolder::None()
{
    return ObjectHolder();
}

Object& ObjectHolder::operator*()
{
    return *Get();
}

const Object& ObjectHolder::operator*() const
{
    return *Get();
}

Object* ObjectHolder::operator->()
{
    return Get();
}

const Object* ObjectHolder::operator->() const
{
    return Get();
}

Object* ObjectHolder::Get()
{
    return m_Data.get();
}

const Object* ObjectHolder::Get() const
{
    return m_Data.get();
}

ObjectHolder::operator bool() const
{
    return Get();
}

bool IsTrue(ObjectHolder object)
{
    if (!object)
        return false;

    if (auto p = object.TryAs<Number>(); p && p->GetValue() != 0)
        return true;

    if (auto p = object.TryAs<String>(); p && !p->GetValue().empty())
        return true;

    if (auto p = object.TryAs<Bool>(); p && p->GetValue())
        return true;

    return false;
}

}
