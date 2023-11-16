#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <functional>

using namespace std::literals;

namespace DynamicPolymorphism
{
    class Pickup
    {
    public:
        virtual std::string convert_signal(const std::string& string_vibrations) const = 0;
        virtual ~Pickup() = default;
    };
    
    class SingleCoil : public Pickup
    {
    public:
        std::string convert_signal(const std::string& string_vibrations) const override
        {
            return "humming "s + string_vibrations;
        }
    };

    class Humbucker : public Pickup
    {
    public:
        std::string convert_signal(const std::string& string_vibrations) const override
        {
            return "strong, noiseless "s + string_vibrations;
        }
    };

    class Guitar
    {
        std::unique_ptr<Pickup> pickup_;

    public:
        Guitar(std::unique_ptr<Pickup> pickup)
            : pickup_{std::move(pickup)}
        {
            assert(pickup_ != nullptr);
        }

        void play(const std::string& sound)
        {
            std::cout << "Playing " << pickup_->convert_signal(sound) << std::endl;
        }
    };
}

namespace StaticPolymorphism
{
    class PickupBase
    {
    public:
        std::string serial_number;

        std::string get_serial_number() const
        {
            return serial_number;
        }
    };

    class SingleCoil : public PickupBase
    {
    public:        
        std::string convert_signal(const std::string& string_vibrations) const
        {
            return "humming "s + string_vibrations;
        }
    };

    class Humbucker : public PickupBase
    {
    public:
        std::string convert_signal(const std::string& string_vibrations) const
        {
            return "strong, noiseless "s + string_vibrations;
        }
    };

    template <typename T>
    concept Pickup = requires(T pickup, const std::string& sound)  { pickup.convert_signal(sound); };

    template <Pickup TPickup_>
    class Guitar
    {
        TPickup_ pickup_;

    public:
        Guitar() = default;

        Guitar(TPickup_ pickup)
            : pickup_{std::move(pickup)}
        {
        }

        void play(const std::string& sound)
        {
            std::cout << "Playing " << pickup_.convert_signal(sound) << std::endl;
        }
    };
}

namespace DuckTyping
{
    using Pickup = std::function<std::string(const std::string&)>;

    class SingleCoil
    {
    public:
        std::string operator()(const std::string& string_vibrations) const
        {
            return "humming "s + string_vibrations;
        }
    };

    struct Guitar
    {
        Pickup pickup;

        Guitar(Pickup pickup)
            : pickup{std::move(pickup)}
        {      
        }

        void play(const std::string& sound)
        {
            std::cout << "Playing " << pickup(sound) << std::endl;
        }
    };
}

int main()
{
    {
        using namespace DynamicPolymorphism;

        Guitar strat{std::make_unique<SingleCoil>()};
        strat.play("D-power-cord");

        Guitar les_paul{std::make_unique<Humbucker>()};
        les_paul.play("D-power-cord");
    }

    {
        using namespace StaticPolymorphism;

        Guitar strat{SingleCoil{}};
        strat.play("D-power-cord");

        Guitar<Humbucker> les_paul;
        les_paul.play("D-power-cord");
    }

    {
        using namespace DuckTyping;

        SingleCoil sc;

        Guitar g{std::move(sc)};

        g.play("D-power-cord");

        g.pickup = [](const std::string& string_vibrations)
        {
            return "strong, noiseless "s + string_vibrations;
        };

        g.play("A-power-cord");
    }
}