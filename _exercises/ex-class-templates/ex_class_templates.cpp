#include <algorithm>
#include <array>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
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

namespace ver_1
{
    template <typename T, typename TContainer = std::deque<T>>
    class Stack
    {
    public:
        using container_type = TContainer;

        size_t size() { return elems.size(); }

        bool empty() { return elems.empty(); }

        void push(const T& elem) { elems.push_back(elem); }

        T& top() { return elems.back(); }

        void pop(T& out)
        {
            out = top(); // copy
            elems.pop_back();
        }

    private:
        TContainer elems;
    };

} // namespace ver_1
/////////////////////////////////////////////

inline namespace ver_2
{
    template <typename T, typename U = std::deque<T>>
    class Stack
    {
    public:
        using container_type = U;
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;

        auto empty() const
        {
            return data.empty();
        }

        auto size() const
        {
            return data.size();
        }

        // void push(value_type t_value)
        //{
        //     data.emplace_back(t_value);
        // }

        // void push(const T& elem) // cc
        // { 
        //     data.push_back(elem); 
        // }

        // void push(T&& elem) // mv
        // {
        //     data.push_back(std::move(elem));
        // }

        template <typename TElem> 
        void push(TElem&& elem)
        {
            data.push_back(std::forward<TElem>(elem));
        }

        const_reference top() const
        {
            return data.back();
        }

        void pop(reference t_value)
        {
            t_value = top();
            data.pop_back();
        }

    private:
        U data;
    };

} // namespace ver_2

namespace ver_3
{
    template <typename T, template<typename, typename> class Container = std::deque, typename TAllocator = std::allocator<T>>
    class Stack
    {
    public:
        using container_type = Container<T, TAllocator>;
        using value_type = T;
        using reference = value_type&;
        using const_reference = const value_type&;

        auto empty() const
        {
            return data.empty();
        }

        auto size() const
        {
            return data.size();
        }

        void push(auto&& elem)
        {
            data.push_back(std::forward<decltype(elem)>(elem));
        }

        const_reference top() const
        {
            return data.back();
        }

        void pop(reference t_value)
        {
            t_value = top();
            data.pop_back();
        }

    private:
        container_type data;
    };

} // namespace ver_2

static_assert(std::is_same_v<Stack<int>::container_type, std::deque<int>>);
static_assert(std::is_same_v<Stack<int, std::vector<int>>::container_type, std::vector<int>>);

// explicit template instantiation
template class Stack<int>;
template class Stack<std::string>;

TEST_CASE("After construction", "[stack]")
{
    Stack<int> s;

    SECTION("is empty")
    {
        REQUIRE(s.empty());
    }

    SECTION("size is zero")
    {
        REQUIRE(s.size() == 0);
    }
}

TEST_CASE("Pushing an item", "[stack,push]")
{
    Stack<int> s;

    SECTION("is no longer empty")
    {
        s.push(1);

        REQUIRE(!s.empty());
    }

    SECTION("size is increased")
    {
        auto size_before = s.size();

        s.push(1);

        REQUIRE(s.size() - size_before == 1);
    }

    SECTION("recently pushed item is on a top")
    {
        s.push(4);

        REQUIRE(s.top() == 4);
    }
}

template <typename TStack>
std::vector<typename TStack::value_type> pop_all(TStack& s)
{
    std::vector<typename TStack::value_type> values(s.size());

    for (auto& item : values)
        s.pop(item);

    return values;
}

TEST_CASE("Pop all items")
{
    Stack<int> s;

    s.push(1);
    s.push(4);

    auto values = pop_all<Stack<int>>(s);
    REQUIRE(values.size() == 2);
}


TEST_CASE("Popping an item", "[stack,pop]")
{
    Stack<int> s;

    int x = 1;
    s.push(x);
    s.push(4);

    int item;

    SECTION(" an item from a top to an argument passed by ref")
    {
        s.pop(item);

        REQUIRE(item == 4);
    }

    SECTION("size is decreased")
    {
        size_t size_before = s.size();

        s.pop(item);

        REQUIRE(size_before - s.size() == 1);
    }

    SECTION("LIFO order")
    {
        int a, b;

        s.pop(a);
        s.pop(b);

        REQUIRE(a == 4);
        REQUIRE(b == 1);
    }
}

TEST_CASE("Popping all items")
{
    ver_3::Stack<int, std::vector> s;

    s.push(1);
    s.push(4);

    auto values = pop_all(s);
    REQUIRE(values.size() == 2);
}
