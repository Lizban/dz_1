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

    Amper operator""_mA(unsigned long long x) {return {static_cast <double>(x)/1000};}
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
    Volt operator*(Ohm r, Amper i) {
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
        std::uniform_real_distribution<double> distribution(mn, mx);
        double value = distribution(generator);
        double limit = std::fmax(std::abs(mn), std::abs(mx));
        double error = limit * (accuracy / 100);
        return Measurement<T>(T{value}, T{error});
    }
    
    class Voltmeter{
        double mn, mx, accuracy;
        std::string id;
    public:
        Voltmeter(Volt lower, Volt upper, double accuracy_class, const std::string& name) : mn(lower.value), mx(upper.value), accuracy(accuracy_class), id(name){
            check_parameters(mn, mx, accuracy, id);
        }
        Measurement<Volt> measure(){
            return take_measurement<Volt>(mn, mx, accuracy);
        }
    };
    class Ampermeter{
        double mn, mx, accuracy;
        std::string id;
    public:
        Ampermeter(Amper lower, Amper upper, double accuracy_class, const std::string& name) : mn(lower.value), mx(upper.value), accuracy(accuracy_class), id(name){
            check_parameters(mn, mx, accuracy, id);
        }
        Measurement<Amper> measure(){
            return take_measurement<Amper>(mn, mx, accuracy);
        }
    };
    class Multimeter{
        double mn, mx, accuracy;
        std::string id;
    public:
        Multimeter(double lower, double upper, double accuracy_class, const std::string& name) : mn(lower), mx(upper), accuracy(accuracy_class), id(name){
            check_parameters(mn, mx, accuracy, id);
        }
        template<class T>
        Measurement<T> measure(){
            return take_measurement<T>(mn, mx, accuracy);
        }
    };
}

namespace std{
    template<class T>
    struct formatter<instruments::Measurement<T>> : formatter<string>{
        auto format(const instruments::Measurement<T>& result, format_context& con) const{
            auto time = chrono::floor<chrono::seconds>(result.get_time()+chrono::hours(3));
            string conclusion= std::format("{:%H:%M:%S} / {:.2f} {} +- {:.2f} {}", time, result.get_value().value, T::unit, result.get_error().value, T::unit);
            return formatter<string>::format(conclusion, con);
        }
    };
}

void example_units(){
    using namespace units;
    Amper i = 500_mA;
    Volt u = 12_V;
    Ohm r = 24_Ohm;
    Watt p = 6_W;
    Joule e = 60_J;
    Second t = 10_s;
    std::cout << "500 mA = " << i.value << "A\n"; 
    std::cout << "i * r = " << (i * r).value << "V\n";
    std::cout << "p / i = " << (p / i).value << "V\n";
    std::cout << "u / r = " << (u / r).value << "A\n";
    std::cout << "p / u = " << (p / u).value << "A\n";
    std::cout << "u * i = " << (u * i).value << "W\n";
    std::cout << "e / t = " << (e / t).value << "W\n";
    std::cout << "u / i = " << (u / i).value << "Ohm\n";
    std::cout << "p * t = " << (p * t).value << "J\n";
    std::cout << "e / p = " << (e / p).value << "s\n";
}

void example_instruments(){
    using namespace units;
    using namespace instruments;
    Voltmeter voltmeter(0_V, 20_V, 1.5, "V");
    Ampermeter ampermeter(0_mA, 1000_mA, 1.0, "A");
    Multimeter multimeter(0, 100, 2.0, "M");
    std::cout << std::format("Voltmeter: {}\n", voltmeter.measure());
    std::cout << std::format("Ampermeter: {}\n", ampermeter.measure());
    std::cout << std::format("Multimeter V: {}\n", multimeter.measure<Volt>());
    std::cout << std::format("Multimeter A: {}\n", multimeter.measure<Amper>());
    std::cout << std::format("Multimeter Ohm: {}\n", multimeter.measure<Ohm>());
}

void example_measurement(){
    using namespace units;
    instruments::Measurement<Volt> result(12_V, Volt{0.3});
    std::cout << std::format("Measurement: {}\n", result);
}

void example_validation(){
    using namespace units;
    try {
        instruments::Voltmeter voltmeter(21_V, 0_V, 1.5, "V2");
    }
    catch (const std::invalid_argument& error) {
        std::cout << error.what() << "\n";
    }
    try {
        instruments::Measurement<Volt> result(12_V, Volt{-0.3});
    }
    catch (const std::invalid_argument& error) {
        std::cout << error.what() << "\n";
    }
    try {
        Amper i = 12_V / 0_Ohm;
        std::cout << i.value << "\n";
    }
    catch (const std::invalid_argument& error) {
        std::cout << error.what() << "\n";
    }
}

int main(){
    example_units();
    example_instruments();
    example_measurement();
    example_validation();
}