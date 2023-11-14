#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <array>

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

    // deduction guide
    Pair(const char*, const char*) -> Pair<std::string, std::string>;

    template <typename T1, typename T2>
    Pair(T1, T2) -> Pair<T1, T2>;
} // namespace ClassTemplates

TEST_CASE("class templates")
{
    SECTION("classic instantiation")
    {
        ClassTemplates::Pair<int, double> p1{42, 3.14};
        //ClassTemplates::Pair<> p2(42, 665);

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

        constexpr iterator begin()
        {
            return &items[0];
        }

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
}

TEST_CASE("Array")
{
    using namespace ClassTemplates;

    Array<int, 10> arr1 = {1, 2, 3, 4};

    for(const auto& item : arr1)
    {
        std::cout << item << " ";
    }
    std::cout << "\n";

    std::array<int, 10> arr2 = {};
}