#ifndef SIMTOOLS_MATRIX_HPP
#define SIMTOOLS_MATRIX_HPP

#include <vector>
#include <array>
#include <functional>
#include <type_traits>
#include "simtools_config.hpp"

namespace simtools {
    template<typename T> using axis = std::vector<T>;

    namespace detail {
        template<typename T, dim_t N>
        struct matrix_types {
            using inner = typename matrix_types<T, N - 1>::type;
            using type = typename axis<inner>;
        };

        template<typename T>
        struct matrix_types<T, 1> {
            using inner = T;
            using type = axis<T>;
        };
    }

    template<typename T, dim_t N> using base_matrix = typename detail::matrix_types<T, N>::type;
    template<dim_t N> using matrix = typename base_matrix<double_t, N>;
    template<dim_t N> using axis_array = typename std::array<matrix<1>, N>;
    template<typename T, dim_t N>
    struct dim_traits
    {
        using matrix = typename detail::matrix_types<T, N>::type;
        using axes = std::array<axis<T>, N>;
    };
}

#endif // SIMTOOLS_MATRIX_HPP