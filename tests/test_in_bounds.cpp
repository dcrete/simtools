#include <random>
#include <chrono>
#include "simtools/tables.hpp"
#include "test_interpolation.h"

namespace simtools::tests {

    template<dim_t N>
    inline void run_test(axis_array<N> axes, const std::array<double_t, N>& coefs, double_t threshold = 1.0E-12) {
        polynomial_generator<N> gen(coefs);
        simtools::data_table<N> table(axes, get_values(axes, gen));
        auto breakpoint_permutations = make_breakpoint_set_permutations(axes);
        auto elapsed = 0.0;
        for (auto& breakpoints : breakpoint_permutations) {
            auto start = high_resolution_clock::now();
            auto result = table.get_value(breakpoints);
            elapsed += static_cast<double_t>(duration_cast<nanoseconds>(high_resolution_clock::now() - start).count());
            auto expected = gen.get_value(breakpoints);
            Assert::AreEqual(expected, result, threshold);
        }
        auto num_1d_interpolations = breakpoint_permutations.size()*calc_1d_interp_count<N>();
        std::ostringstream ss;
        ss << "\n\n";
        ss << "Test_" << N << "D Details:" << "\n";
        ss << "\t" << "# of 1-D Interpolations: " << num_1d_interpolations << "\n";
        ss << "\t" << "Total Elapsed time (nanoseconds): " << elapsed << "\n";
        ss << "\t" << "Efficiency (nanosecond/interpolation): " << elapsed / num_1d_interpolations << "\n";
        ss << "\n\n";
        Logger::WriteMessage(ss.str().c_str());
    }

	TEST_CLASS(test_in_bounds)
	{
	public:

        TEST_METHOD(one_dim_interpolation)
        {
            constexpr auto N = 1;
            using axis_array_type = axis_array<N>;
            using coef_array_type = std::array<double_t, N>;

            auto vars = axis_array_type
            {
                make_range(20000, 2.0, 18.0),
            };
            auto coefs = coef_array_type{ 2.0 };
            run_test(vars, coefs);
        }

        TEST_METHOD(two_dim_interpolation)
        {
            constexpr auto N = 2;
            using axis_array_type = axis_array<N>;
            using coef_array_type = std::array<double_t, N>;

            auto vars = axis_array_type
            {
                make_range(100, 2.0, 20.0),
                make_range(250, 0.0, 0.9),
            };
            auto coefs = coef_array_type{ 4.0, 9.0 };
            run_test(vars, coefs);
        }

        TEST_METHOD(three_dim_interpolation)
        {
            constexpr auto N = 3;
            using axis_array_type = axis_array<N>;
            using coef_array_type = std::array<double_t, N>;

            auto vars = axis_array_type
            {
                make_range(10, 0.5, 3.0),
                make_range(12, 0.0, 25.0),
                make_range(15, 100.0, 300.0),
            };
            auto coefs = coef_array_type{ 6.0, 4.2, 0.7 };
            run_test(vars, coefs);
        }

        TEST_METHOD(four_dim_interpolation)
        {
            constexpr auto N = 4;
            using axis_array_type = axis_array<N>;
            using coef_array_type = std::array<double_t, N>;

            auto vars = axis_array_type
            {
                make_range(10, 0.5, 3.0),
                make_range(12, 0.0, 25.0),
                make_range(15, 100.0, 300.0),
                make_range(3, 0.0, 1.0),
            };
            auto coefs = coef_array_type{ -1.0, 3.3, -2.0, 9.0 };
            run_test(vars, coefs);
        }

        TEST_METHOD(nine_dim_interpolation)
        {
            constexpr auto N = 9;
            using axis_array_type = axis_array<N>;
            using coef_array_type = std::array<double_t, N>;

            auto vars = axis_array_type
            {
                make_range(2, 0.5, 3.0),
                make_range(4, 0.0, 25.0),
                make_range(3, 10.0, 30.0),
                make_range(3, 0.0, 1.0),
                make_range(2, 0.5, 3.0),
                make_range(4, 0.0, 25.0),
                make_range(3, 10.0, 30.0),
                make_range(3, 0.0, 1.0),
                make_range(3, 0.0, 25.0),
            };
            auto coefs = coef_array_type{ -0.1, 0.1, -1.0, 1.0, 3.3, 6.6, 9.9, 15.0, -25.0 };
            run_test(vars, coefs);
        }
	};
}