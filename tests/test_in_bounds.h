#ifndef SIMTOOLS_TESTS_DETAIL_HPP
#define SIMTOOLS_TESTS_DETAIL_HPP

#include <array>
#include <random>
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

    template<dim_t N>
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
        double get_value(const array<std::tuple<dim_t, double>, N>& values) const {
            return get_value(simtools::splice<double>(values));
        }

    private:
        array<double, N> m_coef;
    };

    template<dim_t N>
    inline matrix<N> get_values(const axis_array<N>& axes, const polynomial_generator<N>& gen) {
        auto permutations = simtools::get_permutations_with_indices(axes);
        auto data = simtools::factory::make_matrix(simtools::get_sizes(axes));
        for (auto i = 0U; i < permutations.size(); ++i) {
            auto indices = simtools::splice<dim_t>(permutations[i]);
            auto values = simtools::splice<double>(permutations[i]);
            simtools::set_value(data, indices, gen.get_value(values));
        }
        return data;
    }

    template<dim_t N>
    inline std::vector<std::array<double, N>> gen_breakpoints(const axis_array<N>& axes) {
        auto breakpoint_vectors = axis_array<N>();
        for (auto i = 0U; i < N; ++i) {
            breakpoint_vectors[i] = detail::gen_breakpoints(axes[i]);
        }
        return simtools::get_permutations(breakpoint_vectors);
    }

    inline auto make_range(dim_t n, double start, double end) {
        auto inc = (end - start) / static_cast<double>(n);
        auto x = start - inc;
        auto results = std::vector<double>(n);
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

    template<typename T>
    inline T& operator+=(T& left, std::common_type_t<nanoseconds, nanoseconds>&& dt) {
        left += static_cast<T>(duration_cast<nanoseconds>(dt).count());
        return left;
    }

    template<dim_t N>
    inline void run_test(axis_array<N> vars, const std::array<double, N>& coefs) {
        constexpr auto THRESHOLD = 1.0E-12;

        polynomial_generator<N> gen(coefs);
        simtools::data_table<N> table(std::move(vars), get_values(vars, gen));
        auto bp = gen_breakpoints(vars);
        auto elapsed = 0.0;
        for (auto&& v : bp) {
            auto start = high_resolution_clock::now();
            auto result = table.get_value(v);
            auto x = high_resolution_clock::now() - start;
            elapsed += (high_resolution_clock::now() - start);
            auto expected = gen.get_value(v);
            Assert::AreEqual(expected, result, THRESHOLD);
        }
        auto num_1d_interpolations = bp.size()*calc_1d_interp_count<N>();
        auto ss = std::ostringstream();
        ss << "\n\n";
        ss << "Test_" << N << "D Details:" << "\n";
        ss << "\t" << "# of 1-D Interpolations: " << num_1d_interpolations << "\n";
        ss << "\t" << "Total Elapsed time (nanoseconds): " << elapsed << "\n";
        ss << "\t" << "Efficiency (nanosecond/interpolation): " << elapsed / num_1d_interpolations << "\n";
        ss << "\n\n";
        Logger::WriteMessage(ss.str().c_str());
    }
} }

#endif // SIMTOOLS_TESTS_DETAIL_HPP