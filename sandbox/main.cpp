#include <string>
#include <iostream>
#include "simtools/io.hpp"
#include "variable_container.hpp"

using namespace std::string_literals;

enum class vehicle_type : int { car, truck, suv };

std::ostream& operator<<(std::ostream& stream, vehicle_type type) {
    switch (type) {
    case vehicle_type::car:
        stream << "car";
        break;
    case vehicle_type::truck:
        stream << "truck";
        break;
    case vehicle_type::suv:
        stream << "suv";
        break;
    default:
        stream << "unknown";
        break;
    }
    return stream;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& stream, const simtools::variable<T>& var) {
    stream << var.get_name() << ": " << var.get_value();
    return stream;
}

template<typename T>
void print_types(const simtools::variable_container& container) {
    std::cout << "[" << typeid(T).name() << "]:" << "\n";
    for (const auto& var : container.find_all<T>()) {
        std::cout << "\t" << var << "\n";
    }
}

void print_variables_by_type() {
    simtools::variable_container car;
    car.emplace("type", vehicle_type::car);
    car.emplace("model", "coupe"s);
    car.emplace("cylinders", 4);
    car.emplace("horsepower", 267.8);

    print_types<vehicle_type>(car);
    print_types<std::string>(car);
    print_types<int>(car);
    print_types<double_t>(car);
}

void print_single_variables() {
    simtools::variable_container car;
    car.emplace("type", vehicle_type::car);
    car.emplace("model", "coupe"s);
    car.emplace("cylinders", 4);
    car.emplace("horsepower", 267.8);

    std::cout << car.find<vehicle_type>("type") << "\n";
    std::cout << car.find<std::string>("model") << "\n";
    std::cout << car.find<int>("cylinders") << "\n";
    std::cout << car.find<double_t>("horsepower") << "\n";
}


int main(unsigned argv, char** argc) {
    using namespace simtools;

    print_variables_by_type();
    std::cout << std::vector<std::string>(3, "\n");
    print_single_variables();

    return 0;
}

