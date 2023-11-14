#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

namespace TODO
{
    template <typename Iter, typename Function>
    Iter find_if(Iter begin, Iter end, Function find_function)
    {
        for (Iter i{begin}; i != end; ++i)
        {
            if (find_function(*i))
            {
                return i;
            }
        }

        return end;
    }
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

namespace TODO
{
    template <typename InputIter>
    auto accumulate(InputIter first, InputIter last)
    {
        using T = std::remove_cvref_t<decltype(*first)>; // & and const should be removed from type
        T init{};

        for (; first != last; ++first)
        {
            init += *first;
        }

        return init;
    }

    namespace StdLike
    {
        template <typename InputIter, typename T>
        T accumulate(InputIter first, InputIter last, T init)
        {            
            for (; first != last; ++first)
            {
                init += *first;
            }

            return init;
        }
    } // namespace StdLike
} // namespace TODO

TEST_CASE("my accumulate")
{
    SECTION("ints")
    {
        const vector<int> data = {1, 2, 3, 4, 5};

        auto result = TODO::accumulate(begin(data), end(data));

        static_assert(std::is_same_v<decltype(result), int>);

        REQUIRE(result == 15);
    }

    SECTION("strings")
    {
        std::string words[] = {"one", "two", "three"};

        auto result = TODO::StdLike::accumulate(begin(words), end(words), "Text: "s);

        REQUIRE(result == "Text: onetwothree");
    }

    SECTION("double + accumulate")
    {
        std::vector<double> vec = { 3.14, 1.0, 1.0 };

        CHECK(TODO::StdLike::accumulate(vec.begin(), vec.end(), 0.0) == Catch::Approx(5.14));
    }
}
