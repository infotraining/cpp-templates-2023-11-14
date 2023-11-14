#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

namespace TODO
{
} // namespace TODO

TEST_CASE("my find if")
{
    SECTION("happy path")
    {
        vector<int> vec = {1, 2, 3, 4, 665, 124, 4, 55, 234};

        auto is_lesser_evil = [](int x) {
            return x == 665;
        };

        auto pos = TODO::find_if(begin(vec), end(vec), is_lesser_evil);
        // auto pos = TODO::find_if(begin(vec), end(vec), [](int n) { return std::to_string(n); });

        REQUIRE(pos != end(vec));
        REQUIRE(*pos == 665);
    }

    SECTION("sad path")
    {
        vector<int> vec = {1, 2, 3, 4, 124, 4, 55, 234};

        auto is_lesser_evil = [](int x) {
            return x == 665;
        };

        auto pos = TODO::find_if(begin(vec), end(vec), is_lesser_evil);

        REQUIRE(pos == end(vec));
    }
}

TEST_CASE("my accumulate")
{
    const vector<int> data = {1, 2, 3, 4, 5};

    auto result = TODO::accumulate(begin(data), end(data));

    static_assert(std::is_same_v<decltype(result), int>);
    // static_assert(std::is_same_v<decltype(result), double>);

    REQUIRE(result == 15);
}