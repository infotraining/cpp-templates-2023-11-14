#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

struct Id
{
    int value;
    std::string name;

    // Id(int v, const std::string& n) : value(v), name(n)
    // {}

    // bool operator==(const Id& other) const = default; // implicitly declared
    auto operator<=>(const Id& other) const = default;
};

template <typename T>
T max_value(T a, T b)
{
    return a < b ? b : a;
}

template <> // full specialization of template function
const char* max_value(const char* a, const char* b)
{
    std::cout << __PRETTY_FUNCTION__ << " - template full specialization!\n";
    return std::strcmp(a, b) < 0 ? b : a;
}

const char* max_value(const char* a, const char* b) // overloading
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    return std::strcmp(a, b) < 0 ? b : a;
}

namespace Cpp20
{
    auto max_value(auto a, auto b)
    {
        return a < b ? b : a;
    }

    namespace IsInterpretedAs
    {
        template <typename T1, typename T2>
        auto max_value(T1 a, T2 b)
        {
            return a < b ? b : a;
        }
    } // namespace IsInterpretedAs
} // namespace Cpp20

TEST_CASE("function templates")
{
    CHECK(max_value(42, 665) == 665);

    CHECK(max_value(43.0, 66.5) == 66.5);

    std::string s1 = "text";
    std::string s2 = "test";
    CHECK(max_value(s1, s2) == "text"s);

    CHECK(max_value(static_cast<double>(42), 66.5) == 66.5);
    CHECK(max_value<double>(42, 66.5) == 66.5);
    // CHECK(Cpp20::max_value(67, 66.5) == 67.0);

    CHECK(max_value(Id(42, "forty-two"), Id{665}) == Id{665});

    CHECK(max_value<const char*>("ala", "ola") == "ola"s);
    CHECK(max_value("ala", "ola") == "ola"s);

    const char* (*ptr_fun)(const char*, const char*) = max_value;
    CHECK(ptr_fun("ala", "ola") == "ola"s);

    ptr_fun = max_value<const char*>;
    CHECK(ptr_fun("ala", "ola") == "ola"s);

    auto ptr_fun_other = max_value<double>;
}

TEST_CASE("uniform syntax")
{
    auto ptr = std::make_shared<Id>(42, "forty-two"); // C++11 - C++17 - compiler error; C++20 - OK
}

///////////////////////////////////////////////////////////////////////////////

namespace ReturnType
{
    namespace ExplicitParam
    {
        template <typename TResult, typename T1, typename T2>
        TResult max_value(T1 a, T2 b)
        {
            return a < b ? b : a;
        }
    } // namespace ExplicitParam

    namespace Auto
    {
        template <typename T1, typename T2>
        auto max_value(T1 a, T2 b)
        {
            return a < b ? b : a;
        }
    }

    namespace TypeTraits
    {
        template <typename T1, typename T2>
        std::common_type_t<T1, T2> max_value(T1 a, T2 b)
        {
            return a < b ? b : a;
        }
    }

    namespace DefaultParam
    {
        template <typename T1, typename T2, typename TResult = std::common_type_t<T1, T2>>
        TResult max_value(T1 a, T2 b)
        {
            return a < b ? b : a;
        }
    }
} // namespace ReturnType

TEST_CASE("return type")
{ 
    CHECK(ReturnType::ExplicitParam::max_value<double>(42, 66.5) == 66.5);
    
    CHECK(ReturnType::Auto::max_value(42, 66.5) == 66.5);

    CHECK(ReturnType::TypeTraits::max_value(42, 66.5) == 66.5);
    CHECK(ReturnType::TypeTraits::max_value("text"s, "test") == "text"s);
    
    CHECK(ReturnType::DefaultParam::max_value(42, 66.5f) == 66.5f);
    CHECK(ReturnType::DefaultParam::max_value<int, float, double>(42, 66.5f) == 66.5f);
}