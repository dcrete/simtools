#include <vector>
#include "test_config.hpp"
#include "simtools/algorithms.hpp"

namespace simtools { namespace tests {

    TEST_CLASS(test_algorithms)
    {
    public:
        TEST_METHOD(to_vector)
        {
            auto array_values = std::array<double, 5>{1.3, 2.6, 23.5, 355.0, 29394.2};
            auto result = simtools::to_vector(array_values);
            Assert::IsTrue(array_values == result);
        }

        TEST_METHOD(get_sizes)
        {
            auto expected = std::vector<dim_t>{ 2, 22, 9  };
            auto axes = simtools::axis_array<3> { matrix<1>(expected[0]), matrix<1>(expected[1]), matrix<1>(expected[2]) };
            auto result = simtools::get_sizes(axes);
            Assert::IsTrue(result == expected);
        }

        TEST_METHOD(get_range_runtime)
        {
            auto expected = std::vector<int>{ 2, 3, 4, 5 };
            auto values = std::vector<int>{ 1, 2, 3, 4, 5 };
            auto result = simtools::get_range(values, 1);
            Assert::IsTrue(result == expected);
        }

        TEST_METHOD(get_range_static_vector)
        {
            auto expected = std::vector<int>{ 2, 3, 4, 5 };
            auto values = std::vector<int>{ 1, 2, 3, 4, 5 };
            auto result = simtools::get_range<1, 4>(values);
            Assert::IsTrue(result == expected);
        }

        TEST_METHOD(get_range_static_array)
        {
            auto expected = std::vector<int>{ 2, 3, 4, 5 };
            auto values = std::array<int, 5>{ 1, 2, 3, 4, 5 };
            auto result = simtools::get_range<1, 4>(values);
            Assert::IsTrue(result == expected);
        }

        TEST_METHOD(zip)
        {
            auto expected = std::array<std::tuple<int, int>, 5> { { {1, 1}, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }, } };
            auto set1 = std::array<int, 5>();
            for (auto i = 0; i < 5; ++i) set1[i] = std::get<0>(expected[i]);
            auto set2 = std::array<int, 5>();
            for (auto i = 0; i < 5; ++i) set2[i] = std::get<1>(expected[i]);
            auto result = simtools::zip(set1, set2);
            Assert::IsTrue(result == expected);
        }

        TEST_METHOD(transform)
        {
            auto expected = std::vector<int>{ 2*1, 2*2, 2*3, 2*4, 2*5 };
            auto original = std::vector<int>{ 1, 2, 3, 4, 5 };
            auto transform_func = [](auto val) { return 2 * val; };
            auto result = simtools::transform<int>(original.begin(), original.end(), transform_func);
            Assert::IsTrue(result == expected);
        }

        TEST_METHOD(sum)
        {
            auto values = std::vector<int>{ 1, 2, 3, 4, 5 };
            auto result = simtools::sum(values);
            Assert::IsTrue(result == 15);
        }

        TEST_METHOD(splice)
        {
            auto expected = std::array<std::string, 3>{"1", "2", "3"};
            auto tuples = std::array<std::tuple<int, std::string>, 3> { { {1, "1"}, { 2, "2" }, { 3, "3" }, } };
            auto strings = simtools::splice<std::string>(tuples);
            Assert::IsTrue(strings == expected);
        }

        TEST_METHOD(get_permutations)
        {
            auto axes = axis_array<2>{ matrix<1>{1, 2}, matrix<1>{3, 4, 5} };
            auto expected = std::vector<std::array<double, 2>>
            {
                {{ axes[0][0], axes[1][0] }},
                {{ axes[0][0], axes[1][1] }},
                {{ axes[0][0], axes[1][2] }},
                {{ axes[0][1], axes[1][0] }},
                {{ axes[0][1], axes[1][1] }},
                {{ axes[0][1], axes[1][2] }},
            };
            auto result = simtools::get_permutations(axes);
            Assert::IsTrue(result == expected);
        }

        TEST_METHOD(get_permutations_with_indices)
        {
            auto axes = axis_array<2>{ matrix<1>{1, 2}, matrix<1>{3, 4, 5} };
            auto expected = std::vector<std::array<std::tuple<dim_t, double>, 2>>
            {
                { { {0, axes[0][0]}, {0, axes[1][0]} } },
                { { {0, axes[0][0]}, {1, axes[1][1]} } },
                { { {0, axes[0][0]}, {2, axes[1][2]} } },
                { { {1, axes[0][1]}, {0, axes[1][0]} } },
                { { {1, axes[0][1]}, {1, axes[1][1]} } },
                { { {1, axes[0][1]}, {2, axes[1][2]} } },
            };
            auto result = simtools::get_permutations_with_indices(axes);
            Assert::IsTrue(result == expected);
        }

        TEST_METHOD(get_ref)
        {
            auto data = matrix<2>
            {
                matrix<1>{1, 2, 3},
                matrix<1>{4, 5, 6},
                matrix<1>{7, 8, 9}
            };
            auto indices = std::array<dim_t, 2>{1, 1};
            auto result = simtools::get_ref(data, indices);
            Assert::IsTrue(result == 5);
        }

        TEST_METHOD(set_value)
        {
            auto data = matrix<2>
            {
                matrix<1>{1, 2, 3},
                matrix<1>{4, 5, 6},
                matrix<1>{7, 8, 9}
            };
            auto indices = std::array<dim_t, 2>{1, 1};
            simtools::set_value(data, indices, 10.0);
            Assert::IsTrue(data[1][1] == 10);
        }

        TEST_METHOD(matrix_equal)
        {
            auto data1 = matrix<2>
            {
                matrix<1>{1, 2, 3},
                matrix<1>{4, 5, 6},
                matrix<1>{7, 8, 9}
            };
            auto data2 = matrix<2>
            {
                matrix<1>{1, 2, 3},
                matrix<1>{4, 5, 6},
                matrix<1>{7, 8, 9}
            };
            Assert::IsTrue(data1 == data2);
            auto data3 = matrix<2>
            {
                matrix<1>{1, 2, 3},
                matrix<1>{4, 5, 6},
            };
            Assert::IsFalse(data1 == data3);
        }

        TEST_METHOD(matrix_not_equal)
        {
            auto data1 = matrix<2>
            {
                matrix<1>{1, 2, 3},
                matrix<1>{4, 5, 6},
                matrix<1>{7, 8, 9}
            };
            auto data2 = matrix<2>
            {
                matrix<1>{1, 2, 3},
                matrix<1>{4, 5, 6},
                matrix<1>{7, 8, 9}
            };
            Assert::IsFalse(data1 != data2);
            auto data3 = matrix<2>
            {
                matrix<1>{1, 2, 3},
                matrix<1>{4, 5, 6},
            };
            Assert::IsTrue(data1 != data3);
        }
    };
} }