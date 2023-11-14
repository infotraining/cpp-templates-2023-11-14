#include <catch2/catch_test_macros.hpp>

#include <algorithm>
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
{}

template <typename T>
void deduce1(T arg)
{
    puts(__PRETTY_FUNCTION__);
}

template <typename T>
void deduce2(T& arg)
{
    puts(__PRETTY_FUNCTION__);
}

template <typename T>
void deduce3(T&& arg)
{
    puts(__PRETTY_FUNCTION__);
}

TEST_CASE("type deduction rules")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];

    SECTION("Case 1")
    {
        deduce1(x);       // deduce1<int>(int)
        deduce1(cx);      // deduce1<int>(int)
        deduce1(ref_x);   // deduce1<int>(int)
        deduce1(cref_x);  // deduce1<int>(int)
        deduce1(tab);     // deduce1<int*>(int*)
        deduce1(foo);     // deduce1<void(*)(int)>(void(*)(int))

        auto a1 = x;      // int
        auto a2 = cx;     // int
        auto a3 = ref_x;  // int
        auto a4 = cref_x; // int
        auto a5 = tab;    // int*
        auto a6 = foo;    // void(*a6)(int)

        auto a7 = {1, 2, 3};   // std::initializer_list<int>
        // deduce1({1, 2, 3}); // ERROR
    }

    SECTION("Case 2")
    {
        deduce2(x);        // deduce2<int>(int&)
        deduce2(cx);       // deduce2<const int>(const int&)
        deduce2(ref_x);    // deduce2<int>(int&)
        deduce2(cref_x);   // deduce2<const int>(const int&)
        deduce2(tab);      // deduce2<int[10]>(int(&arg)[10])
        deduce2(foo);      // deduce2<void(int)>(void(&arg)(int))
        
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
        deduce3(x);               // deduce3<int&>(int& &&) -> deduce3<int&>(int&)
        deduce3(cx);              // deduce3<const int&>(const int& &&) -> deduce3<const int&>(const int&)
        deduce3(ref_x);           // deduce3<int&>(int& &&) -> deduce3<int&>(int&)
        deduce3(cref_x);          // deduce3<const int&>(const int& &&) -> deduce3<const int&>(const int&)
        deduce3(tab);             // deduce3<int(&)[10]>(int(&)[10])
        deduce3(foo);             // deduce3<void(&)(int)>(void(&)(int))

        // r-value
        deduce3(string("text"));  // deduce3<std::string>(std::string&&)

        auto&& a1 = x;                   // int&
        auto&& a2 = cx;                  // const int&
        auto&& a3 = ref_x;               // int&
        auto&& a4 = cref_x;              // const int&
        auto&& a5 = tab;                 // int(&)[10]
        auto&& a6 = foo;                 // void(&)(int)
        auto&& a7 = string("text");      // std::string&&
    }
}

TEST_CASE("decltype(auto)")
{ 
}