#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std::literals;

template <auto N>
constexpr auto unroll = [](auto expr)
{
    [expr]<auto... Is>(std::index_sequence<Is...>) {
        ((expr(), void(Is)), ...);
    }(std::make_index_sequence<N>{});
};

TEST_CASE("loop unrolling")
{
    unroll<10>([] { std::cout << "Hello World!\n"; });
}