#include <variant>
#include <iostream>

int main()
{
    std::variant<int, float> v;
    v = 42;
    std::cout << std::get<int>(v) << std::endl;
}
