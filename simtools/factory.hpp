#ifndef SIMTOOLS_FACTORY_HPP
#define SIMTOOLS_FACTORY_HPP

#include <array>
#include <vector>
#include "simtools_config.hpp"
#include "matrix.hpp"
#include "tuple_math.hpp"

namespace simtools { namespace factory {

    template<dim_t N>
    constexpr inline matrix<N> make_matrix(std::vector<dim_t>::const_iterator it) {
        if constexpr (N == 1) {
            return matrix<1>(*it);
        }
        else {
            return matrix<N>(*it, make_matrix<N - 1>(it + 1));
        }
    }

    template<dim_t N>
    constexpr inline matrix<N> make_matrix(std::array<dim_t, N> sizes) {
        auto v = std::vector<dim_t>(sizes.begin(), sizes.end());
        return make_matrix<N>(v.begin());
    }

    template<typename T, dim_t N>
    constexpr inline std::array<T, N> make_array(std::function<T(dim_t)> func) {
        auto result = std::array<T, N>();
        for (auto i = 0U; i < N; ++i) {
            result[i] = func(i);
        }
        return result;
    }

    template<typename T1, typename... T2>
    constexpr inline std::array<T1, sizeof...(T2)> make_array(T2... vars) {
        return { static_cast<T1>(vars)... };
    }

} }

#endif // SIMTOOLS_FACTORY_HPP