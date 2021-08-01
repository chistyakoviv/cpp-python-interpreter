#include "comparators.h"
#include "object.h"

#include <functional>
#include <optional>
#include <string>

namespace Runtime {

template <typename T, typename Cmp>
std::optional<bool> TryCompare(const ObjectHolder& lhs, const ObjectHolder& rhs, Cmp cmp)
{
    const T* left = lhs.TryAs<T>();
    const T* right = rhs.TryAs<T>();

    if (left && right)
    {
        return cmp(left->GetValue(), right->GetValue());
    }
    else
    {
        return std::nullopt;
    }
}

bool Less(ObjectHolder lhs, ObjectHolder rhs)
{
    std::optional<bool> result = TryCompare<Runtime::Bool>(lhs, rhs, std::less<bool>());

    if (!result)
        result = TryCompare<Runtime::Number>(lhs, rhs, std::less<int>());

    if (!result)
        result = TryCompare<Runtime::String>(lhs, rhs, std::less<std::string>());

    if (result)
        return *result;

    if (auto p = lhs.TryAs<Runtime::ClassInstance>(); p && p->HasMethod("__lt__", 1))
    {
        return IsTrue(p->Call("__lt__", {rhs}));
    }

    throw std::runtime_error("Cannot compare objets for less");
}

bool Equal(ObjectHolder lhs, ObjectHolder rhs)
{
    std::optional<bool> result = TryCompare<Runtime::Bool>(lhs, rhs, std::equal_to<bool>());

    if (!result)
        result = TryCompare<Runtime::Number>(lhs, rhs, std::equal_to<int>());

    if (!result)
        result = TryCompare<Runtime::String>(lhs, rhs, std::equal_to<std::string>());

    if (result)
        return *result;

    if (auto p = lhs.TryAs<Runtime::ClassInstance>(); p && p->HasMethod("__eq__", 1))
        return IsTrue(p->Call("__eq__", {rhs}));

    if (!lhs && !rhs)
        return true;

    throw std::runtime_error("Cannot compare objets for equalty");
}

}
