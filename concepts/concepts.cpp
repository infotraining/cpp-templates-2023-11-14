#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

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
} // namespace ver_4

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

template <typename T>
struct Data
{
    T value;

    void print() const
    {
        std::cout << "value: " << value << "\n";
    }

    void print() const
        requires std::ranges::range<T>
    {
        std::cout << "values: ";
        for (const auto& item : value)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    void zero()
    {
        value = T{};
    }

    void zero()
        requires std::ranges::range<T>
    {
        using TValue = std::ranges::range_value_t<T>;
        for (auto&& item : value)
            item = TValue{};
    }
};

TEST_CASE("requires clause for class method")
{
    Data ds1{42};
    ds1.print();

    Data ds2{std::vector{1, 2, 3}};
    ds2.print();
    ds2.zero();
    ds2.print();

    Data ds3{std::vector<bool>{1, 0, 0, 1}};
    ds3.print();
    ds3.zero();
    ds3.print();
}

template <typename T>
concept SizedContainer = requires(T& container) {
    {
        container.size()
    } noexcept -> std::same_as<size_t>;
    //{ container.size() } -> std::convertible_to<size_t>;
};

/////////////////////////////////////////////////////////
// concept subsuming

struct BoundingBox
{
    int w, h;
};

struct Color
{
    int r, g, b;
};

template <typename T>
concept Shape = requires(T obj) {
    {
        obj.box()
    } -> std::same_as<BoundingBox>;
    obj.draw();
};

template <typename T>
concept ShapeWithColor = Shape<T> && // ShapeWithColor subsumes Shape
    requires(T obj, Color c) {
        obj.set_color(c);
        {
            obj.get_color()
        } -> std::convertible_to<Color>;
    };

template <Shape T>
void render(T shp)
{
    shp.draw();
}

template <ShapeWithColor T>
void render(T shp)
{
    std::cout << "Setting color & draw\n";
    shp.set_color(Color{0, 255, 22});
    shp.draw();
}

struct Rect
{
    int w, h;
    Color color{};

    BoundingBox box() const
    {
        return {w, h};
    }

    void draw() const
    {
        std::cout << "Drawing rect: " << w << ", " << h << "\n";
    }

    void set_color(Color c)
    {
        color = c;
    }

    Color get_color() const
    {
        return color;
    }
};

static_assert(Shape<Rect>);
static_assert(ShapeWithColor<Rect>);

TEST_CASE("subsuming concepts")
{
    Rect rect{100, 200};
    render(rect);
}

////////////////////////////////////////////////////////
// subsuming

template <typename T>
    requires std::integral<T>
void calculation(T value)
{
    std::cout << "Calc for integral\n";
}

template <typename T>
    requires std::integral<T> && std::is_unsigned_v<T>
void calculation(T value)
{
    std::cout << "Calc for unsigned integral\n";
}

TEST_CASE("subsuming concepts - 2")
{
    calculation(42ull);
}

template <std::input_iterator Iter>
void some_algorithm(Iter first, Iter last)
{
    std::cout << "void some_algorithm(std::input_iterator auto first, std::input_iterator auto last)\n";
}

template <std::contiguous_iterator Iter>
void some_algorithm(Iter first, Iter last)
{
    std::cout << "void some_algorithm(std::contiguous_iterator auto first, std::contiguous_iterator auto last)\n";
}

namespace Cpp20Auto
{
    void some_algorithm(std::input_iterator auto first, std::input_iterator auto last)
    {
        std::cout << "void some_algorithm(std::input_iterator auto first, std::input_iterator auto last)\n";
    }

    void some_algorithm(std::contiguous_iterator auto first, std::contiguous_iterator auto last)
    {
        std::cout << "void some_algorithm(std::contiguous_iterator auto first, std::contiguous_iterator auto last)\n";
    }
} // namespace Cpp20Auto

TEST_CASE("subsuming concepts - iterators")
{
    std::list<int> lst{1, 2, 3};
    some_algorithm(lst.begin(), lst.end());

    std::vector<int> vec{1, 2, 3};
    some_algorithm(vec.begin(), vec.end());
}