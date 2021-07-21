#pragma once

#include <memory>
#include <unordered_map>
#include "object.h"

class ObjectHolder
{
public:
    ObjectHolder() = default;

    template <typename T>
    static ObjectHolder Own(T&& object)
    {
        return ObjectHolder(
            std::make_shared<T>(std::forward<T>(object))
        );
    }

    static ObjectHolder Share(Object& object);
    static ObjectHolder None();

    Object& operator*();
    const Object& operator*() const;
    Object* operator->();
    const Object* operator->() const;

    Object* Get();
    const Object* Get() const;

    template <typename T>
    T* TryAs()
    {
        return dynamic_cast<T*>(Get());
    }

    template <typename T>
    const T* TryAs() const
    {
        return dynamic_cast<const T*>(Get());
    }

    explicit operator bool() const;

private:
    ObjectHolder(std::shared_ptr<Object> data)
        : m_Data(data)
    {
    }

    std::shared_ptr<Object> m_Data;
};

using Closure = std::unordered_map<std::string, ObjectHolder>;

std::ostream& operator<<(std::ostream& os, const Closure& closure);
