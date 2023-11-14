#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

void foo(int)
{ }

template <typename T>
void deduce1(T arg)
{
    puts(__PRETTY_FUNCTION__);
}

namespace Cpp20
{
    void deduce1(auto arg)
    {
    }
} // namespace Cpp20

template <typename T>
void deduce2(T& arg)
{
    puts(__PRETTY_FUNCTION__);
}

namespace Cpp20
{
    void deduce1(auto& arg)
    {
    }
} // namespace Cpp20

template <typename T>
void deduce3(T&& arg)
{
    puts(__PRETTY_FUNCTION__);
}

namespace Cpp20
{
    void deduce1(auto&& arg)
    {
    }
} // namespace Cpp20

TEST_CASE("type deduction rules")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];

    SECTION("Case 1")
    {
        deduce1(x);      // deduce1<int>(int)
        deduce1(cx);     // deduce1<int>(int)
        deduce1(ref_x);  // deduce1<int>(int)
        deduce1(cref_x); // deduce1<int>(int)
        deduce1(tab);    // deduce1<int*>(int*)
        deduce1(foo);    // deduce1<void(*)(int)>(void(*)(int))

        auto a1 = x;      // int
        auto a2 = cx;     // int
        auto a3 = ref_x;  // int
        auto a4 = cref_x; // int
        auto a5 = tab;    // int*
        auto a6 = foo;    // void(*a6)(int)

        auto a7 = {1, 2, 3}; // std::initializer_list<int>
        // deduce1({1, 2, 3}); // ERROR
    }

    SECTION("Case 2")
    {
        deduce2(x);      // deduce2<int>(int&)
        deduce2(cx);     // deduce2<const int>(const int&)
        deduce2(ref_x);  // deduce2<int>(int&)
        deduce2(cref_x); // deduce2<const int>(const int&)
        deduce2(tab);    // deduce2<int[10]>(int(&arg)[10])
        deduce2(foo);    // deduce2<void(int)>(void(&arg)(int))

        auto& a1 = x;      // int&
        auto& a2 = cx;     // const int&
        auto& a3 = ref_x;  // int&
        auto& a4 = cref_x; // const int&
        auto& a5 = tab;    // int(&a5)[10]
        auto& a6 = foo;    // void(&a6)(int)
    }

    SECTION("Case 3")
    {
        // l-value
        deduce3(x);      // deduce3<int&>(int& &&) -> deduce3<int&>(int&)
        deduce3(cx);     // deduce3<const int&>(const int& &&) -> deduce3<const int&>(const int&)
        deduce3(ref_x);  // deduce3<int&>(int& &&) -> deduce3<int&>(int&)
        deduce3(cref_x); // deduce3<const int&>(const int& &&) -> deduce3<const int&>(const int&)
        deduce3(tab);    // deduce3<int(&)[10]>(int(&)[10])
        deduce3(foo);    // deduce3<void(&)(int)>(void(&)(int))

        // r-value
        deduce3(string("text")); // deduce3<std::string>(std::string&&)

        auto&& a1 = x;              // int&
        auto&& a2 = cx;             // const int&
        auto&& a3 = ref_x;          // int&
        auto&& a4 = cref_x;         // const int&
        auto&& a5 = tab;            // int(&)[10]
        auto&& a6 = foo;            // void(&)(int)
        auto&& a7 = string("text"); // std::string&&
    }
}

namespace Auto
{
    template <typename TContainer>
    auto& get_nth(TContainer& container, size_t nth)
    {
        return container[nth]; 
    }
} // namespace Auto

namespace DecltypeAuto
{
    template <typename TContainer>
    decltype(auto) get_nth(TContainer& container, size_t nth)
    {
        return container[nth]; 
    }
} // namespace Auto

TEST_CASE("auto vs. decltype(auto)")
{
    std::vector<int> vec = {1, 2, 3, 4, 5};
    CHECK(Auto::get_nth(vec, 2) == 3);
    Auto::get_nth(vec, 3) = 42;
    CHECK(vec[3] == 42);

    std::vector<std::string> words = { "one", "two", "three" };
    Auto::get_nth(words, 1) = "two-2";
    CHECK(words[1] == "two-2"s);

    int tab[10] = {1, 2, 3, 4};
    Auto::get_nth(tab, 5) = 665;
    CHECK(tab[5] == 665);

    std::vector<bool> vec_bool = {1, 1, 0, 0};
    DecltypeAuto::get_nth(vec_bool, 2) = true;
    CHECK(vec_bool[2] == true);
}

inline void print_int(int n)
{
    std::cout << n << "\n";
}

template <typename InputIter, typename Function>
void my_for_each(InputIter first, InputIter last, Function f)
{
    for(InputIter it = first; it != last; ++it)
    {
        f(*it);
    }
}

template <typename F, typename... Args>
decltype(auto) call_wrapper(F f, Args&&... args)
{
    std::cout << "log: function call...\n";
    return f(std::forward<Args>(args)...);
}

TEST_CASE("template functions & functions as params")
{
    std::vector<int> vec = {1, 2, 3, 4};

    my_for_each(vec.begin(), vec.end(), print_int);

    auto f = [](int& n) { n *= 2; };
    my_for_each(vec.begin(), vec.end(), f);
    std::cout << "---\n";

    my_for_each(vec.begin(), vec.end(), print_int);
}