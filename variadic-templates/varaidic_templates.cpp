#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>
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

    //////////////////////////////////////////////////////

    template <typename... Types>
    struct Count;

    template <typename Head, typename... Tail>
    struct Count<Head, Tail...>
    {
        constexpr static int value = 1 + Count<Tail...>::value; // expansion pack
    };

    template <>
    struct Count<>
    {
        constexpr static int value = 0;
    };

    //...
    static_assert(Count<int, double, std::string&>::value == 3, "must be 3");

} // namespace VT

TEST_CASE("head-tail idiom")
{
    VT::print(1, 3.14, "text"s);
    VT::print("text"s, "ctext", 66.5, 42);
}

//////////////////////////////////////////////////////////
// Fold expressions

namespace VT::Folds
{
    template <typename... TArgs>
    auto sum(TArgs... args)
    {
        return (... + args); // return ((((1 + 2) + 3) + 4) + 5)
    }

    template <typename... Args>
    void print(const Args&... args)
    {
        //(std::cout << ... << args) << "\n";

        (..., (std::cout << args << " ")) << "\n";
    }

    auto call_foreach(auto&& f, auto&&... param)
    {
        (..., f(std::forward<decltype(param)>(param)));
    }
} // namespace VT::Folds

TEST_CASE("sum")
{
    CHECK(VT::Folds::sum(1, 2, 3, 4, 5) == 15);

    VT::Folds::print(1, 42, 3.14, "text");

    auto printer = [](const auto& value) {
        std::cout << "value: " << value << "\n";
    };

    VT::Folds::call_foreach(printer, 1, 2, 3, 4, "text"s);
}

/////////////////////////////////////////////////////////////

namespace VT::Folds
{
    namespace ver_1
    {
        template <typename Arg, typename... Args>
        auto make_vector(Arg arg, Args... args)
        {
            std::vector vec = {arg};
            vec.reserve(sizeof...(args) + 1);

            (..., (vec.push_back(args)));

            return vec;
        }
    } // namespace ver_1

    namespace ver_2
    {
        template <typename Arg, typename... Args>
        auto make_vector(Arg arg, Args... args) requires (... && std::is_same_v<Arg, Args>)
        {
            std::vector vec = {arg};
            vec.reserve(sizeof...(args) + 1);

            (..., (vec.push_back(args)));

            return vec;
        }
    } // namespace ver_1

    inline namespace ver_3
    {
        template <typename... Args>
        auto make_vector(Args&&... args)
        {
            using T = std::common_type_t<Args...>;
            
            std::vector<T> vec;
            vec.reserve(sizeof...(args));

            (..., (vec.push_back(std::forward<Args>(args))));

            return vec;
        }
    } // namespace ver_1
} // namespace VT::Folds

TEST_CASE("make_vector")
{
    const std::vector<int> vec1 = {1, 2, 3, 4};
    const std::vector<int> vec2 = VT::Folds::make_vector(1, 2, 3, 4);

    CHECK(vec1 == vec2);

    // std::vector<std::unique_ptr<int>> vec_ptrs = { std::make_unique<int>(42), std::make_unique<int>(665) };

    const std::vector<std::unique_ptr<int>> vec_ptrs = VT::Folds::make_vector(std::make_unique<int>(42), std::make_unique<int>(665));

    // std::vector<std::unique_ptr<int>> vec_ptrs;
    // vec_ptrs.push_back(std::make_unique<int>(42));
    // vec_ptrs.push_back(std::make_unique<int>(665));
}