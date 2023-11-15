#include <array>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

using namespace std::literals;

namespace ClassTemplates
{
    template <typename T1, typename T2>
    struct Pair
    {
        T1 first;
        T2 second;

        template <typename U1, typename U2>
        Pair(U1&& fst, U2&& snd)
            : first(std::forward<U1>(fst))
            , second(std::forward<U2>(snd))
        { }

        auto operator<=>(const Pair& other) const = default;
    };

    // helper function
    template <typename T1, typename T2>
    Pair<T1, T2> make_pair(T1 fst, T2 snd)
    {
        return Pair<T1, T2>(fst, snd);
    }

    template <typename T1, typename T2>
    Pair(T1, T2) -> Pair<T1, T2>;

    // partial specialization Pair<T, T>
    template <typename T>
    struct Pair<T, T>
    {
        T first, second;

        template <typename U1, typename U2>
        Pair(U1&& fst, U2&& snd)
            : first(std::forward<U1>(fst))
            , second(std::forward<U2>(snd))
        { }

        auto operator<=>(const Pair& other) const = default;

        const T& max_value() const
        {
            return first < second ? second : first;
        }
    };

    // full specialization
    template <>
    struct Pair<const char*, const char*>
    {
        std::string first, second;

        Pair(const char* str1, const char* str2)
            : first(str1)
            , second(str2)
        { }

        auto operator<=>(const Pair& other) const = default;

        const std::string& max_value() const
        {
            return first < second ? second : first;
        }
    };

} // namespace ClassTemplates

TEST_CASE("class templates")
{
    SECTION("classic instantiation")
    {
        ClassTemplates::Pair<int, double> p1{42, 3.14};
        // ClassTemplates::Pair<> p2(42, 665);

        auto p3 = ClassTemplates::make_pair(42, 3.14); // Pair<int, double>
    }

    SECTION("modern CTAD - since C++17")
    {
        ClassTemplates::Pair p1{42, 3.14}; // Pair<int, double>

        ClassTemplates::Pair p2{"text", "text"s}; // Pair<const char*, std::string>

        const int cx = 10;
        const int& ref_cx = cx;
        ClassTemplates::Pair p3(cx, ref_cx); // Pair<int, int>

        ClassTemplates::Pair p4{"abc", "def"};
    }

    SECTION("specialization")
    {
        ClassTemplates::Pair<int, int> p1(42, 665);
        CHECK(p1.max_value() == 665);

        std::string str = "text";
        ClassTemplates::Pair<std::string, std::string> p2(str, "test");
    }
}

////////////////////////////////////////////////////

namespace ClassTemplates
{
    template <typename T, size_t N>
    struct Array
    {
        T items[N];

        using iterator = T*;
        using const_iterator = const T*;
        using pointer = T*;
        using const_pointer = const T*;

        constexpr size_t size() const
        {
            return N;
        }

        constexpr iterator begin();

        constexpr iterator end()
        {
            return &items[0] + N;
        }

        constexpr const_iterator begin() const
        {
            return &items[0];
        }

        constexpr const_iterator end() const
        {
            return &items[0] + N;
        }

        constexpr pointer data()
        {
            return &items[0];
        }

        constexpr const_pointer data() const
        {
            return &items[0];
        }
    };

    template <typename T, size_t N>
    constexpr typename Array<T, N>::iterator Array<T, N>::begin()
    {

        return &items[0];
    }
} // namespace ClassTemplates

TEST_CASE("Array")
{
    using namespace ClassTemplates;

    Array<int, 10> arr1 = {1, 2, 3, 4};

    for (const auto& item : arr1)
    {
        std::cout << item << " ";
    }
    std::cout << "\n";

    std::array<int, 10> arr2 = {};
}

////////////////////////////////////////////////
// dependent names

struct X
{
    // static int A(int n)
    // {
    //     std::cout << __PRETTY_FUNCTION__ << "\n";

    //     return n;
    // }

    struct A
    {
        A(int n)
        {
            std::cout << __PRETTY_FUNCTION__ << "\n";
        }
    };
};

template <typename T>
auto dependent_typename_context(int x)
{
    auto value = typename T::A(x);

    return value;
}

template <typename Container>
typename Container::value_type sum(const Container& container)
{
    using result_type = typename Container::value_type;

    result_type result{};

    for (typename Container::const_iterator it = container.begin(); it != container.end(); ++it)
        result += *it;

    return result;
}

struct S1
{
    static constexpr int A = 0; // S1::A is an object
}; 

struct S2
{
    template <int N>
    static void A(int) { }  // S2::A is a function template
};

struct S3
{
    template <int N> // S3::A is a class template
    struct A
    { 
        A(int x) {}
    };
}; 
int x;

template <class T>
void dependent_template_context(int x)
{
    //auto a = T::A<0>(x);                 // if T::A is an object, this is a pair of comparisons;         
    //auto a = T::template A<0>(x);        // if T::A is a function template, this is a function call;
    auto a = typename T::template A<0>(x); // if T::A is a class or alias template, this is a declaration.
}

TEST_CASE("dependent names")
{
    dependent_typename_context<X>(42);

    std::vector vec = {1, 2, 3};
    CHECK(sum(vec) == 6);

    dependent_template_context<S3>(42);
}