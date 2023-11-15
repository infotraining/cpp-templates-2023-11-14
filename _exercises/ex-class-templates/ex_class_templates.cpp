#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

// TODO: Stack

// static_assert(std::is_same_v<Stack<int>::container_type, std::deque<int>>);
// static_assert(std::is_same_v<Stack<int, std::vector<int>>::container_type, std::vector<int>>);

// template class Stack<int>;
// template class Stack<std::string>;

// TEST_CASE("After construction", "[stack]")
// {
//     Stack<int> s;

//     SECTION("is empty")
//     {
//         REQUIRE(s.empty());
//     }

//     SECTION("size is zero")
//     {
//         REQUIRE(s.size() == 0);
//     }
// }

// TEST_CASE("Pushing an item", "[stack,push]")
// {
//     Stack<int> s;

//     SECTION("is no longer empty")
//     {
//         s.push(1);

//         REQUIRE(!s.empty());
//     }

//     SECTION("size is increased")
//     {
//         auto size_before = s.size();

//         s.push(1);

//         REQUIRE(s.size() - size_before == 1);
//     }

//     SECTION("recently pushed item is on a top")
//     {
//         s.push(4);

//         REQUIRE(s.top() == 4);
//     }
// }

// template <typename TStack>
// std::vector<typename TStack::value_type> pop_all(TStack& s)
// {
//     std::vector<typename TStack::value_type> values(s.size());

//     for (auto& item : values)
//         s.pop(item);

//     return values;
// }

// TEST_CASE("Popping an item", "[stack,pop]")
// {
//     Stack<int> s;

//     s.push(1);
//     s.push(4);

//     int item;

//     SECTION(" an item from a top to an argument passed by ref")
//     {
//         s.pop(item);

//         REQUIRE(item == 4);
//     }

//     SECTION("size is decreased")
//     {
//         size_t size_before = s.size();

//         s.pop(item);

//         REQUIRE(size_before - s.size() == 1);
//     }

//     SECTION("LIFO order")
//     {
//         int a, b;

//         s.pop(a);
//         s.pop(b);

//         REQUIRE(a == 4);
//         REQUIRE(b == 1);
//     }
// }

