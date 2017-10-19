#ifndef SIMTOOLS_TESTS_DETAIL_HPP
#define SIMTOOLS_TESTS_DETAIL_HPP

#include <array>
#include <random>
#include "simtools/matrix.hpp"
#include "simtools/tuple_math.hpp"
#include "simtools/algorithms.hpp"

namespace tests {
    using std::array;
    using simtools::matrix;
    using simtools::axis_array;
    using simtools::ndim_t;

    namespace detail {
        inline matrix<1> gen_breakpoints(const matrix<1>& var_values) {
            auto min = *std::min_element(var_values.begin(), var_values.end());
            auto max = *std::max_element(var_values.begin(), var_values.end());
            auto result = matrix<1>{ /*0.95*min, 1.05*max*/ };
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_real_distribution<> dist(min, max);
            for (auto i = 0U; i < var_values.size(); ++i) {
                result.emplace_back(dist(eng));
            }
            return result;
        }
    }

    template<ndim_t N>
    class polynomial_generator
    {
    public:
        polynomial_generator(const array<double, N>& coefs) : m_coef(coefs) { }
        double get_value(const array<double, N>& values) const {
            auto multiply_tuple_elems = [](auto& t) {return simtools::multiply<double, double>(t);};
            auto pairs = simtools::zip<double, double, N>(m_coef, values);
            auto multiplied_values = simtools::transform<double>(pairs.begin(), pairs.end(), multiply_tuple_elems);
            return simtools::sum(multiplied_values);
        }
        double get_value(const array<std::tuple<ndim_t, double>, N>& values) const {
            return get_value(simtools::splice<double>(values));
        }

    private:
        array<double, N> m_coef;
    };

    template<ndim_t N>
    inline matrix<N> get_values(const axis_array<N>& axes, const polynomial_generator<N>& gen) {
        auto permutations = simtools::get_permutations_with_indices(axes);
        auto data = simtools::make_matrix(simtools::get_sizes(axes));
        for (auto i = 0U; i < permutations.size(); ++i) {
            auto indices = simtools::splice<ndim_t>(permutations[i]);
            auto values = simtools::splice<double>(permutations[i]);
            simtools::set_value(data, indices, gen.get_value(values));
        }
        return data;
    }

    template<ndim_t N>
    inline std::vector<std::array<double, N>> gen_breakpoints(const axis_array<N>& axes) {
        auto breakpoint_vectors = axis_array<N>();
        for (auto i = 0U; i < N; ++i) {
            breakpoint_vectors[i] = detail::gen_breakpoints(axes[i]);
        }
        return simtools::get_permutations(breakpoint_vectors);
    }

    inline auto make_range(ndim_t n, double start, double end) {
        auto inc = (end - start) / static_cast<double>(n);
        auto x = start - inc;
        auto results = std::vector<double>(n);
        for (auto& v : results) {
            v = (x += inc);
        }
        return results;
    }
}

#endif // SIMTOOLS_TESTS_DETAIL_HPP