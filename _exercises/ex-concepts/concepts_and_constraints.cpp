#include <catch2/catch_test_macros.hpp>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <source_location>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std::literals;

/*********************
Iterator concept
1. iterator is dereferencable: *iter
2. can be pre-incremented - returns reference to iterator
3. can be post-incremented
4. is equality comparable: supports == and !=
**********************/

constexpr static bool TODO = false;

template <typename I>
concept Iterator = TODO;

TEST_CASE("Iterator")
{
    // static_assert(Iterator<int*>);
    // static_assert(Iterator<const int*>);
    // static_assert(Iterator<std::vector<int>::iterator>);
    // static_assert(Iterator<std::vector<int>::const_iterator>);
    // static_assert(Iterator<std::forward_list<int>::const_iterator>);
}

/*********************
StdContainer concept
1. std::begin(C&) returns iterator
2. std::end(C&) returns iterator
3. has inner type iterator
4. has inner type const_iterator
5. has inner type value type
**********************/

template <typename Container>
concept StdContainer = TODO;

TEST_CASE("StdContainer")
{
    // static_assert(StdContainer<std::vector<int>>);
    // static_assert(StdContainer<std::list<int>>);
    // static_assert(StdContainer<std::set<int>>);
    // static_assert(StdContainer<std::map<int, std::string>>);
    // static_assert(StdContainer<std::unordered_map<int, int>>);
    // static_assert(StdContainer<std::vector<bool>>);
    // static_assert(StdContainer<std::string>);

    // int arr[32];
    // static_assert(!StdContainer<decltype(arr)>);
}

/*********************
IndexableContainer concept
1. is StdContainer
2. can be indexed: c[index]
**********************/

template <typename C>
concept Indexable = TODO;

TEST_CASE("Indexable")
{
    // static_assert(Indexable<std::vector<int>>);
    // static_assert(Indexable<std::map<int, std::string>>);
    // static_assert(Indexable<std::map<std::string, std::string>>);
    // static_assert(Indexable<std::unordered_map<int, int>>);
    // static_assert(Indexable<std::vector<bool>>);
    // static_assert(Indexable<std::string>);
    // int arr[32];
    // static_assert(Indexable<decltype(arr)>);
    // static_assert(!Indexable<std::list<int>>);
    // static_assert(!Indexable<std::set<int>>);
}

template <typename C>
concept IndexableStdContainer = TODO;

TEST_CASE("IndexableStdContainer")
{
    // static_assert(IndexableStdContainer<std::vector<int>>);
    // static_assert(!IndexableStdContainer<std::list<int>>);
    // static_assert(!IndexableStdContainer<std::set<int>>);
    // static_assert(IndexableStdContainer<std::map<int, std::string>>);
    // static_assert(IndexableStdContainer<std::map<std::string, std::string>>);
    // static_assert(IndexableStdContainer<std::unordered_map<int, int>>);
    // static_assert(IndexableStdContainer<std::vector<bool>>);
    // static_assert(IndexableStdContainer<std::string>);
    // static_assert(!IndexableStdContainer<int[10]>);
}

void print_all(const StdContainer auto& container)
{
    std::cout << "void print_all(const StdContainer auto& container)\n";

    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

void print_all(const IndexableStdContainer auto& container)
{
    std::cout << "void print_all(const IndexableContainer auto& container)\n";

    for (size_t i = 0; i < std::size(container); ++i)
    {
        std::cout << container[i] << " ";
    }
    std::cout << "\n";
}

TEST_CASE("container concepts")
{
    // std::vector vec = {1, 2, 3, 4};
    // print_all(vec);

    // std::list lst{1, 2, 3};
    // print_all(lst);
}
