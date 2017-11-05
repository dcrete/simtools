#ifndef SIMTOOLS_TEST_CONFIG_HPP
#define SIMTOOLS_TEST_CONFIG_HPP

#include <SDKDDKVer.h>
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "simtools/simtools_config.hpp"

namespace simtools { namespace tests {
    using namespace std::string_literals;
    using namespace Microsoft::VisualStudio::CppUnitTestFramework;

    namespace detail {
        template<typename Left, typename Right>
        inline bool equals(const Left& left, const Right& right) {
            if (left.size() != right.size()) return false;
            for (auto i = 0U; i < left.size(); ++i) {
                if (left[i] != right[i]) return false;
            }
            return true;
        }
    }

    template<typename T>
    inline bool operator==(const std::vector<T>& left, const std::vector<T>& right) {
        return detail::equals(left, right);
    }

    template<typename T, dim_t N>
    inline bool operator==(const std::array<T, N>& left, const std::vector<T>& right) {
        return detail::equals(left, right);
    }
} }

#endif // SIMTOOLS_TEST_CONFIG_HPP