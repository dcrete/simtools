#include <random>
#include <chrono>
#include "simtools/tables.hpp"
#include "test_in_bounds.h"

namespace simtools { namespace tests {

	TEST_CLASS(test_in_bounds)
	{
	public:

        TEST_METHOD(one_dim_interpolation)
        {
            constexpr auto N = 1;
            auto vars = axis_array<N>
            {
                make_range(5000, 2.0, 18.0),
            };
            auto coefs = std::array<double, N>{ 2.0 };
            run_test(vars, coefs);
        }

        TEST_METHOD(two_dim_interpolationd)
        {
            constexpr auto N = 2;
            auto vars = axis_array<N>
            {
                make_range(100, 2.0, 20.0),
                make_range(250, 0.0, 0.9),
            };
            auto coefs = std::array<double, N>{ 4.0, 9.0 };
            run_test(vars, coefs);
        }

        TEST_METHOD(three_dim_interpolation)
        {
            constexpr auto N = 3;
            auto vars = axis_array<N>
            {
                make_range(10, 0.5, 3.0),
                make_range(12, 0.0, 25.0),
                make_range(15, 100.0, 300.0),
            };
            auto coefs = std::array<double, 3>{ 6.0, 4.2, 0.7 };
            run_test(vars, coefs);
        }

        TEST_METHOD(four_dim_interpolation)
        {
            constexpr auto N = 4;
            auto vars = axis_array<N>
            {
                make_range(10, 0.5, 3.0),
                make_range(12, 0.0, 25.0),
                make_range(15, 100.0, 300.0),
                make_range(3, 0.0, 1.0),
            };
            auto coefs = std::array<double, N>{ -1.0, 3.3, -2.0, 9.0 };
            run_test(vars, coefs);
        }

        TEST_METHOD(nine_dim_interpolation)
        {
            constexpr auto N = 9;
            auto vars = axis_array<N>
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
            auto coefs = std::array<double, N>{ -0.1, 0.1, -1.0, 1.0, 3.3, 6.6, 9.9, 15.0, -25.0 };
            run_test(vars, coefs);
        }
	};
} }