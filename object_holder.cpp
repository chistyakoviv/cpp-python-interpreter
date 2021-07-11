#include "object_holder.h"

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
