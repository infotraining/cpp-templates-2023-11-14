#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

int foo(int x)
{
    return x * 2;
}

template <auto X>
struct Foo
{
    static constexpr int value = X * 2;
};

template <auto X>
constexpr auto Foo_v = Foo<X>::value;

////////////////////////////////////////////////////////

template <typename T>
struct Identity
{
    using type = T;
};

template <typename T>
using Identity_t = typename Identity<T>::type;

TEST_CASE("type traits")
{
    static_assert(Foo<4>::value == 8);
    static_assert(Foo_v<8> == 16);

    Identity<int>::type x = 42;
    Identity_t<int> y = 42;
}

//////////////////////////////////////////////////////////

template <typename T, T v>
struct IntegralConstant
{
    static constexpr T value = v;
};

static_assert(IntegralConstant<int, 4>::value == 4);

template <bool v>
using BoolConstant = IntegralConstant<bool, v>;

static_assert(BoolConstant<true>::value == true);

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

static_assert(TrueType::value);
static_assert(!FalseType::value);

////////////////////////////////////////////////////////////

template <typename T>
struct RemoveRef
{
    using type = T;
};

template <typename T>
struct RemoveRef<T&>
{
    using type = T;
};

template <typename T>
struct RemoveRef<T&&>
{
    using type = T;
};

template <typename T>
using RemoveRef_t = typename RemoveRef<T>::type;

TEST_CASE("type traits - remove ref")
{
    using T = int&;
    RemoveRef_t<T> obj{};

    using U = std::string;
    RemoveRef_t<U> word;

    auto pusher = [](auto&& item, size_t count) {
        using T = RemoveRef_t<decltype(item)>;
        std::vector<T> items;
        items.reserve(count);

        items.push_back(std::forward<decltype(item)>(item));
        //...
        return items;
    };

    auto vec = pusher("test"s, 100);
}