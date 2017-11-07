#include <random>
#include <chrono>
#include "simtools/tables.hpp"
#include "test_interpolation.h"

namespace simtools {
    namespace tests {

        template<dim_t N>
        inline void run_test(axis_array<N> axes, const std::array<double, N>& coefs, double threshold = 1.0E-12) {
            polynomial_generator<N> gen(coefs);
            simtools::data_table<N> table(axes, get_values(axes, gen));
            auto breakpoint_permutations = make_breakpoint_set_permutations(axes);
            auto elapsed = 0.0;
            for (auto& breakpoints : breakpoint_permutations) {
                auto start = high_resolution_clock::now();
                auto result = table.get_value(breakpoints);
                elapsed += static_cast<double>(duration_cast<nanoseconds>(high_resolution_clock::now() - start).count());
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

        TEST_CLASS(test_table_lookup)
        {
        public:

            TEST_METHOD(table_lookup)
            {
                constexpr auto N = 2;
                using axis_array_type = axis_array<N>;
                using coef_array_type = std::array<double, N>;
                using generator_type = polynomial_generator<N>;
                using breakpoint_set_type = std::vector<std::array<double, N>>;

                std::array<std::string, 3> keys{ "key1", "key2", "key3" };

                std::map<std::string, generator_type> generator_map
                {
                    { keys[0], generator_type(coef_array_type{ 2.0, 5.0 }) },
                    { keys[1], generator_type(coef_array_type{ 10.0, 25.0 }) },
                    { keys[2], generator_type(coef_array_type{ 0.5, 1.2 }) },
                };

                std::map<std::string, axis_array_type> axes_map
                {
                    { keys[0], axis_array_type{ make_range(10, 25, 50), make_range(10, 1.0, 2.0) } },
                    { keys[1], axis_array_type{ make_range(15, 10, 25), make_range(6, 12.0, 18.0) } },
                    { keys[2], axis_array_type{ make_range(5, 1.0, 10.0), make_range(3, 0.4, 0.8) } },
                };

                std::map<std::string, breakpoint_set_type> breakpoint_set_map;
                simtools::data_table_map<std::string> table_map;
                for (auto& key : keys) {
                    breakpoint_set_map.insert(std::make_pair(key, make_breakpoint_set_permutations(axes_map[key])));
                    table_map.emplace(key, data_table<N>{ axes_map[key], get_values(axes_map[key], generator_map[key]) });
                }

                auto run_test = [&](const std::string& key) {
                    for (auto& breakpoint_set : breakpoint_set_map[key]) {
                        auto result = table_map.look_up(key, breakpoint_set);
                        auto expected = generator_map[key].get_value(breakpoint_set);
                        Assert::AreEqual(expected, result, 1E-12);
                    }
                };

                run_test(keys[1]);
                run_test(keys[2]);
                run_test(keys[0]);
            }
        };
    }
}