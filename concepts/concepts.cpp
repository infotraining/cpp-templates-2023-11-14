#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>


using namespace std::literals;

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

    return *a < *b ? *b : *a;
}

TEST_CASE("concepts")
{
    int x = 10;
    int y = 665;
    CHECK(max_value(x, y) == 665);

    CHECK(max_value(&x, &y) == 665);
}