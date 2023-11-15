#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

///////////////////////////////////////////
// enable_if - how it works

template <bool Condition, typename T = void>
struct EnableIf
{
    using type = T;
};

template <typename T>
struct EnableIf<false, T>
{
};

template <bool Condition, typename T = void>
using EnableIf_t = typename EnableIf<Condition, T>::type;

///////////////////////////////////////////////////////////////

template <typename T>
EnableIf_t<(sizeof(T) <= 8)> do_stuff(T obj)
{
    std::cout << "do_stuff(small obj)\n";
}

template <typename T>
EnableIf_t<(sizeof(T) > 8)> do_stuff(const T& obj)
{
    std::cout << "do_stuff(large obj)\n";
}

namespace ver_2
{
    template <typename T>
    auto do_stuff(T obj) -> EnableIf_t<(sizeof(T) <= 8)>
    {
        std::cout << "do_stuff(small obj)\n";
    }

    template <typename T>
    auto do_stuff(const T& obj) -> EnableIf_t<(sizeof(T) > 8)>
    {
        std::cout << "do_stuff(large obj)\n";
    }
} // namespace ver_2

namespace ver_3
{
    template <typename T, typename = std::enable_if_t<(sizeof(T) <= 8)>>
    void do_stuff(T obj)
    {
        std::cout << "do_stuff(small obj)\n";
    }

    template <typename T, typename = void, typename = std::enable_if_t<(sizeof(T) > 8)>>
    void do_stuff(const T& obj)
    {
        std::cout << "do_stuff(large obj)\n";
    }
} // namespace ver_2

///////////////////////////////////////////

TEST_CASE("SFINAE")
{
    int x = 10;
    do_stuff(x);

    std::vector vec = {1, 2, 3};
    ver_3::do_stuff(vec);
}

////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// enable_if & sfinae in class templates
template <typename T, typename Enabled = void>
struct Data
{
public:
    Data()
    {
        std::cout << "Data(generic)" << std::endl;
    }

    std::string info() const
    {
        return "Data(generic)";
    }
};

template <typename T>
using FloatingPoint = std::enable_if_t<std::is_floating_point_v<T>>;

template <typename T>
struct Data<T, FloatingPoint<T>>
{
public:
    Data()
    {
        std::cout << "Data(floats)" << std::endl;
    }

    std::string info() const
    {
        return "Data(floats)";
    }
};

TEST_CASE("enable_if + class templates")
{
    Data<int> ds1;
    ds1.info();

    Data<double> ds2;
    ds2.info();
}