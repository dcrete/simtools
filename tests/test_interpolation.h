#ifndef SIMTOOLS_TESTS_DETAIL_HPP
#define SIMTOOLS_TESTS_DETAIL_HPP

#include <array>
#include <random>
#include <chrono>
#include "simtools/matrix.hpp"
#include "simtools/tuple_math.hpp"
#include "simtools/algorithms.hpp"
#include "simtools/factory.hpp"
#include "test_config.hpp"

namespace simtools { namespace tests {
    using std::array;
    using simtools::matrix;
    using simtools::axis_array;
    using simtools::dim_t;
    using namespace std::chrono;
    
    namespace detail {
        inline auto make_breakpoint_set(const matrix<1>& var_values) {
            auto min = *std::min_element(var_values.begin(), var_values.end());
            auto max = *std::max_element(var_values.begin(), var_values.end());
            matrix<1> result{ };
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_real_distribution<> dist(min, max);
            for (auto i = 0U; i < var_values.size(); ++i) {
                result.emplace_back(dist(eng));
            }
            return result;
        }
    }

    template<dim_t N>
    class polynomial_generator
    {
    public:
        polynomial_generator() = default;
        polynomial_generator(const array<double_t, N>& coefs) : m_coef(coefs) { }
        double_t get_value(const array<double_t, N>& values) const {
            auto multiply_tuple_elems = [](auto& t) {return simtools::multiply<double_t, double_t>(t);};
            auto pairs = simtools::zip<double_t, double_t, N>(m_coef, values);
            auto multiplied_values = simtools::transform<double_t>(pairs.begin(), pairs.end(), multiply_tuple_elems);
            return simtools::sum(multiplied_values);
        }
        double_t get_value(const array<std::tuple<dim_t, double_t>, N>& values) const {
            return get_value(simtools::splice<double_t>(values));
        }

    private:
        array<double_t, N> m_coef;
    };

    template<dim_t N>
    inline matrix<N> get_values(const axis_array<N>& axes, const polynomial_generator<N>& gen) {
        auto permutations = simtools::get_permutations_with_indices(axes);
        auto data = simtools::factory::make_matrix(simtools::get_sizes(axes));
        for (auto i = 0U; i < permutations.size(); ++i) {
            auto indices = simtools::splice<dim_t>(permutations[i]);
            auto values = simtools::splice<double_t>(permutations[i]);
            simtools::set_value(data, indices, gen.get_value(values));
        }
        return data;
    }

    template<dim_t N>
    inline std::vector<std::array<double_t, N>> make_breakpoint_set_permutations(const axis_array<N>& axes) {
        axis_array<N> breakpoint_vectors;
        for (auto i = 0U; i < N; ++i) {
            breakpoint_vectors[i] = detail::make_breakpoint_set(axes[i]);
        }
        return simtools::get_permutations(breakpoint_vectors);
    }

    inline auto make_range(dim_t n, double_t start, double_t end) {
        auto inc = (end - start) / static_cast<double_t>(n);
        auto x = start - inc;
        std::vector<double_t> results(n);
        for (auto& v : results) {
            v = (x += inc);
        }
        return results;
    }

    template<dim_t N>
    inline auto calc_1d_interp_count() {
        if constexpr (N == 1) {
            return 1;
        }
        else {
            auto dim_minus_1_count = calc_1d_interp_count<N - 1>();
            return 2 * dim_minus_1_count + 1;
        }
    }

} }

#endif // SIMTOOLS_TESTS_DETAIL_HPP