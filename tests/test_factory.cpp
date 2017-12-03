#include "test_config.hpp"
#include "simtools/factory.hpp"

namespace simtools::tests {

    TEST_CLASS(test_factory)
    {
    public:
        TEST_METHOD(make_matrix_from_size_array)
        {
            auto expected = std::array<dim_t, 3>{2, 4, 3};
            auto data = simtools::factory::make_matrix(expected);
            auto result = std::array<dim_t, 3>{ data.size(), data[0].size(), data[0][0].size() };
            Assert::IsTrue(expected == result);
        }

        TEST_METHOD(make_matrix_from_size_array_iterator)
        {
            auto expected = std::vector<dim_t>{2, 4, 3};
            auto data = simtools::factory::make_matrix<3>(expected.begin());
            auto result = std::vector<dim_t>{ data.size(), data[0].size(), data[0][0].size() };
            Assert::IsTrue(expected == result);
        }

        TEST_METHOD(make_array_from_func)
        {
            auto expected = std::array<double_t, 3>{1, 2, 3};
            auto func = [](dim_t index) { return static_cast<double_t>(index + 1); };
            auto result = simtools::factory::make_array<double_t, 3>(func);
            Assert::IsTrue(expected == result);
        }

        TEST_METHOD(make_array_from_args)
        {
            auto expected = std::array<double_t, 3>{1, 2, 3};
            auto result = simtools::factory::make_array<double_t>(expected[0], expected[1], expected[2]);
            Assert::IsTrue(expected == result);
        }
    };
}