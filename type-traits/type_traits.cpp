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

TEST_CASE("useful type traits")
{
    static_assert(std::is_same_v<std::remove_const_t<const int>, int>);
    static_assert(std::is_same_v<std::decay_t<const int>, int>);
    static_assert(std::is_same_v<std::decay_t<const int&>, int>);
    static_assert(std::is_same_v<std::decay_t<int[10]>, int*>);
    static_assert(std::is_same_v<std::decay_t<int()>, int(*)()>);
}

/////////////////////////////////////////////////////////////////////
// type traits - predicates

template <typename T>
struct IsVoid
{
    static constexpr bool value = false;
};

template <>
struct IsVoid<void>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;

//////////////////////////////////////////////////////////////////////

template <typename T>
struct IsPointer : FalseType
{
};
    
template <typename T>
struct IsPointer<T*> : std::true_type
{
};

template <typename T>
constexpr bool IsPointer_v = IsPointer<T>::value;


TEST_CASE("type traits - predicates")
{
    using T = int;
    static_assert(IsVoid<T>::value == false);

    using U = void;
    static_assert(IsVoid_v<U> == true);

    static_assert(IsPointer_v<int> == false);
    static_assert(IsPointer_v<const int*> == true);
}

namespace UsingTraits
{
    template <typename T>
    T max_value(T a, T b)
    {
        static_assert(!IsPointer_v<T>, "T cannot be a pointer type");
        return a < b ? b : a;
    }
}

TEST_CASE("using traits")
{   
    int x = 10;
    int y = 20;

    CHECK(UsingTraits::max_value(x, y) == 20);
    //UsingTraits::max_value(&x, &y); // ERROR - static_assert failed
}    
