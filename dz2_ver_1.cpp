#include <cstdio> 
#include <iostream>
#include <format>
#include <chrono>
#include <random>
#include <string>
#include <stdexcept>

namespace units {
    struct Amper {double value = 0; static constexpr const char* unit = "A";};
    struct Volt {double value = 0; static constexpr const char* unit = "V";};
    struct Ohm {double value = 0; static constexpr const char* unit = "Ohm";};
    struct Joule {double value = 0; static constexpr const char* unit = "J";};
    struct Watt {double value = 0; static constexpr const char* unit = "W";};
    struct Second {double value = 0; static constexpr const char* unit = "s";};

    Amper operator""_mA(unsigned long long x) {return {static_cast <double>(x)};}
    Volt operator""_V(unsigned long long x) {return {static_cast <double>(x)};}
    Ohm operator""_Ohm(unsigned long long x) {return {static_cast <double>(x)};}
    Joule operator""_J(unsigned long long x) {return {static_cast <double>(x)};}
    Watt operator""_W(unsigned long long x) {return {static_cast <double>(x)};}
    Second operator""_s(unsigned long long x) {return {static_cast <double>(x)};}

    double division(double a, double b){
        if (b == 0){
            throw std::invalid_argument("division by zero");
        }
        return a / b;
    }

    Volt operator*(Amper i, Ohm r) {
        return {i.value * r.value};
    }
    Volt operator*(Amper i, Ohm r) {
        return {r.value * i.value};
    }
    Volt operator/(Watt p, Amper i) {
        return {division(p.value, i.value)};
    }
    Amper operator/(Volt u, Ohm r) {
        return {division(u.value, r.value)};
    }
    Amper operator/(Watt p, Volt u) {
        return {division(p.value, u.value)};
    }
    Ohm operator/(Volt u, Amper i) {
        return {division(u.value, i.value)};
    }
    Watt operator*(Volt u, Amper i) {
        return {u.value * i.value};
    }
    Watt operator*(Amper i, Volt u) {
        return {i.value * u.value};
    }
    Watt operator/(Joule e, Second t) {
        return {division(e.value, t.value)};
    }
    Joule operator*(Watt p, Second t) {
        return {p.value * t.value};
    }
    Joule operator*(Second t, Watt p) {
        return {t.value * p.value};
    }
    Second operator/(Joule e, Watt p) {
        return {division(e.value, p.value)};
    }
}

namespace instruments {
    using namespace units;
    template <class T>
    class Measurement{
        std::chrono::system_clock::time_point timestamp;
        T reading;
        T uncertainty;
    public:
        Measurement(T value, T error)
            : timestamp(std::chrono::system_clock::now()), reading(value), uncertainty(error) {
                if (!std::isfinite(value.value) || !std::isfinite(error.value) || error.value < 0) {
                    throw std::invalid_argument("Invalid measurement");
                }
        }
        std::chrono::system_clock::time_point get_time() const {
            return timestamp;
        }
        T get_value() const {
            return reading;
        }
        T get_error() const {
            return uncertainty;
        }
    };
    void check_parameters(double mn, double mx, double accuracy, const std::string& id){
        if (mn >= mx) {
            throw std::invalid_argument("Invalid range");
        }
        if (accuracy <= 0) {
            throw std::invalid_argument("Invalid class accuracy");
        }
        if (id.empty()) {
            throw std::invalid_argument("Invalid instrument id");
        }
    }

    template <class T>
    Measurement<T> take_measurement(double mn, double mx, double accuracy){
        static std::default_random_engine generator(std::random_device{}());
    }
}