#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std::literals;

template <typename... Types>
struct Row
{
    std::tuple<Types...> data;
};

template <typename... Types>
struct Pointers
{
    std::tuple<const Types*...> ptrs;
};

template <size_t... Indexes>
struct IndexSequence
{ };

template <typename F, typename... Args>
decltype(auto) call_wrapper(F&& f, Args&&... args) // arg1, arg2
{
    return f(std::forward<Args>(args)...); // std::forward<Args1>(args1), std::forward<Args2>(args2)
}

namespace Cpp20
{
    decltype(auto) call_wrapper(auto f, auto... args)
    {
    }
} // namespace Cpp20

int foo(int n)
{
    return n * 2;
}

int bar(int n, int m)
{
    return n * m;
}

TEST_CASE("variadic templates")
{
    Row<> tpl0;
    Row<int, double, const char*> tpl1;

    IndexSequence<0, 1, 2, 3, 4> is1{};
    IndexSequence<1, 1, 2> is2{};

    CHECK(call_wrapper(foo, 2) == 4);

    CHECK(call_wrapper(bar, 5, 8) == 40);
}

//////////////////////////////////////////////////////////
namespace VT
{
    // void print()
    // {
    //     std::cout << "\n";
    // }

    template <typename Head, typename... Tail>
    void print(Head head, Tail... tail)
    {
        std::cout << head << " ";

        if constexpr (sizeof...(tail) > 0)
            print(tail...); // recursion + pack_expansion
        else
            std::cout << "\n";
    }
} // namespace VT

TEST_CASE("head-tail idiom")
{
    VT::print(1, 3.14, "text"s);
    VT::print("text"s, "ctext", 66.5, 42);
}