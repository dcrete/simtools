
#include "test_config.hpp"
#include "simtools/comparators.hpp"

namespace simtools { namespace tests {
    using namespace simtools::comp;

    TEST_CLASS(test_comparators)
    {
    public:
        TEST_METHOD(test_less_than)
        {
            auto func = less_than(1.0);
            auto result = func(1.5);
            Assert::IsTrue(result);
        }

        TEST_METHOD(test_less_than_or_equal)
        {
            auto func = less_than_or_equal(1.0);
            auto result = func(1.0);
            Assert::IsTrue(result);
        }

        TEST_METHOD(test_greater_than)
        {
            auto func = greater_than(1.0);
            auto result = func(0.5);
            Assert::IsTrue(result);
        }

        TEST_METHOD(test_greater_than_or_equal)
        {
            auto func = greater_than_or_equal(1.0);
            auto result = func(1.0);
            Assert::IsTrue(result);
        }

        TEST_METHOD(test_equals)
        {
            auto func = equal(1.0);
            auto result = func(1.0);
            Assert::IsTrue(result);
        }

        TEST_METHOD(test_not_equals)
        {
            auto func = not_equal(1.0);
            auto result = func(0.5);
            Assert::IsTrue(result);
        }
    };
} }