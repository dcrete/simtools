#ifndef SIMTOOLS_TUPLE_MATH_HPP
#define SIMTOOLS_TUPLE_MATH_HPP

#include <tuple>
#include <numeric>
#include <algorithm>
#include <functional>
#include <type_traits>

namespace simtools {

    template<typename T1, typename T2, typename T>
    constexpr auto math_cast(T value) {
        using std::is_floating_point;
        using std::is_signed;
        if      constexpr (is_floating_point<T1>::value) { return static_cast<T1>(value); }
        else if constexpr (is_floating_point<T2>::value) { return static_cast<T2>(value); }
        else if constexpr (is_signed<T1>::value) { return static_cast<T1>(value); }
        else if constexpr (is_signed<T2>::value) { return static_cast<T2>(value); }
        else { return value; }
    }

    template<typename T1, typename T2>
    constexpr inline auto multiply(T1 left, T2 right) {
        return math_cast<T1, T2>(left * right);
    }

    template<typename T1, typename T2>
    constexpr inline auto divide(T1 left, T2 right) {
        return math_cast<T1, T2>(left / right);
    }

    template<typename T1, typename T2>
    constexpr inline auto add(T1 left, T2 right) {
        return math_cast<T1, T2>(left + right);
    }

    template<typename T1, typename T2>
    constexpr inline auto subtract(T1 left, T2 right) {
        return math_cast<T1, T2>(left - right);
    }

    template<typename T1, typename T2, typename Tuple = std::tuple<T1, T2>>
    constexpr inline auto multiply(const Tuple& t) {
        return multiply(std::get<0>(t), std::get<1>(t));
    }

    template<typename T1, typename T2, typename Tuple = std::tuple<T1, T2>>
    constexpr inline auto divide(const Tuple& t) {
        return divide(std::get<0>(t), std::get<1>(t));
    }

    template<typename T1, typename T2, typename Tuple = std::tuple<T1, T2>>
    constexpr inline auto add(const Tuple& t) {
        return add(std::get<0>(t), std::get<1>(t));
    }

    template<typename T1, typename T2, typename Tuple = std::tuple<T1, T2>>
    constexpr inline auto subtract(const Tuple& t) {
        return subtract(std::get<0>(t), std::get<1>(t));
    }
}

#endif // SIMTOOLS_TUPLE_MATH_HPP