#include <SDKDDKVer.h>
#include "CppUnitTest.h"
#include "simtools/tables.hpp"
#include "detail.hpp"
#include <random>
#include <string>

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
    using simtools::axis_array;

    template<ndim_t N>
    void run_test(axis_array<N> vars, const std::array<double, N>& coefs) {
        auto gen = polynomial_generator<N>(coefs);
        simtools::data_table<N> table(std::move(vars), get_values(vars, gen));
        auto bp = gen_breakpoints(vars);
        for (auto&& v : bp) {
            auto result = table.get_value(v);
            auto expected = gen.get_value(v);
            Assert::AreEqual(expected, result, 1.0E-9);
        }
    }

	TEST_CLASS(UnitTest1)
	{
	public:

        TEST_METHOD(Test_1D)
        {
            constexpr auto N = 1;
            auto vars = axis_array<N>
            {
                make_range(9, 2.0, 18.0),
            };
            auto coefs = std::array<double, N>{ 2.0 };
            run_test(vars, coefs);
        }

        TEST_METHOD(Test_2D)
        {
            constexpr auto N = 2;
            auto vars = axis_array<N>
            {
                make_range(10, 2.0, 20.0),
                make_range(9, 0.0, 0.9),
            };
            auto coefs = std::array<double, N>{ 4.0, 9.0 };
            run_test(vars, coefs);
        }

        TEST_METHOD(Test_3D)
        {
            constexpr auto N = 3;
            auto vars = axis_array<N>
            {
                make_range(10, 0.5, 3.0),
                make_range(12, 0.0, 25.0),
                make_range(15, 1000.0, 30000.0),
            };
            auto coefs = std::array<double, 3>{ 6.0, 4.2, 0.7 };
            run_test(vars, coefs);
        }

        TEST_METHOD(Test_4D)
        {
            constexpr auto N = 4;
            auto vars = axis_array<N>
            {
                make_range(10, 0.5, 3.0),
                make_range(12, 0.0, 25.0),
                make_range(15, 1000.0, 30000.0),
                make_range(3, 0.0, 1.0),
            };
            auto coefs = std::array<double, N>{ -1.0, 3.3, -2.0, 9.0 };
            run_test(vars, coefs);
        }

        TEST_METHOD(Test_9D)
        {
            constexpr auto N = 9;
            auto vars = axis_array<N>
            {
                make_range(2, 0.5, 3.0),
                make_range(4, 0.0, 25.0),
                make_range(3, 1000.0, 30000.0),
                make_range(3, 0.0, 1.0),
                make_range(2, 0.5, 3.0),
                make_range(4, 0.0, 25.0),
                make_range(3, 1000.0, 30000.0),
                make_range(3, 0.0, 1.0),
                make_range(3, 0.0, 25.0),
            };
            auto coefs = std::array<double, N>{ -0.1, 0.1, -1.0, 1.0, 3.3, 6.6, 9.9, 15.0, -25.0 };
            run_test(vars, coefs);
        }

	};
}