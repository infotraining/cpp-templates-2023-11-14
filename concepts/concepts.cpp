#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <list>

using namespace std::literals;

namespace ver_1
{
    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <typename T>
        requires std::is_pointer_v<T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return max_value(*a, *b);
    }
} // namespace ver_1

template <typename T>
concept Pointer = requires(T ptr) {
    *ptr;
    ptr == nullptr;
    ptr != nullptr;
};

static_assert(Pointer<int*>);
static_assert(!Pointer<int>);

namespace ver_2
{
    template <typename T>
    concept Pointer = std::is_pointer_v<T>;

    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <typename T>
        requires Pointer<T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return max_value(*a, *b);
    }
} // namespace ver_2

inline namespace ver_3
{
    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <Pointer T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return max_value(*a, *b);
    }
} // namespace ver_3

inline namespace ver_4
{
    auto max_value(auto a, auto b)
    {
        return a < b ? b : a;
    }

    auto max_value(Pointer auto a, Pointer auto b)
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return max_value(*a, *b);
    }
} // namespace ver_3

TEST_CASE("concepts")
{
    int x = 10;
    int y = 665;
    CHECK(max_value(x, y) == 665);

    CHECK(max_value(&x, &y) == 665);

    CHECK(max_value(std::make_shared<int>(42), std::make_shared<int>(665)) == 665);

    std::vector<int> vec = {1, 2, 3};

    std::random_access_iterator auto it = vec.begin(); // std::random_access_iterator<decltype(it)>
}

struct X
{
    int x;

    bool operator==(const X& other) const = default;
};

TEST_CASE("requires expressions")
{
    static_assert(requires(X x, X y) { x == y; });
}