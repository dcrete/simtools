#ifndef SIMTOOLS_COMPARATORS_HPP
#define SIMTOOLS_COMPARATORS_HPP

#include <xstddef>
#include <functional>
#include "simtools_config.hpp"

namespace simtools { namespace comp {

    namespace detail {
        template<typename T, typename Comparison>
        constexpr inline auto make_comparer(const T& left) {
            Comparison comp;
            return [&](const auto& right) { return comp(left, right); };
        }
    }

    template<typename T>
    constexpr inline auto less_than(const T& left) {
        using comparison_type = typename std::less<T>;
        return detail::make_comparer<T, comparison_type>(left);
    }

    template<typename T>
    constexpr inline auto less_than_or_equal(const T& left) {
        using comparison_type = typename std::less_equal<T>;
        return detail::make_comparer<T, comparison_type>(left);
    }

    template<typename T>
    constexpr inline auto greater_than(const T& left) {
        using comparison_type = typename std::greater<T>;
        return detail::make_comparer<T, comparison_type>(left);
    }

    template<typename T>
    constexpr inline auto greater_than_or_equal(const T& left) {
        using comparison_type = typename std::greater_equal<T>;
        return detail::make_comparer<T, comparison_type>(left);
    }

    template<typename T>
    constexpr inline auto equal(const T& left) {
        using comparison_type = typename std::equal_to<T>;
        return detail::make_comparer<T, comparison_type>(left);
    }

    template<typename T>
    constexpr inline auto not_equal(const T& left) {
        using comparison_type = typename std::not_equal_to<T>;
        return detail::make_comparer<T, comparison_type>(left);
    }

} }

#endif // SIMTOOLS_COMPARATORS_HPP