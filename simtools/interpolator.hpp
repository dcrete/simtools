#ifndef SIMTOOLS_INTERPOLATOR_HPP
#define SIMTOOLS_INTERPOLATOR_HPP

#include <array>
#include <algorithm>
#include "simtools_config.hpp"
#include "matrix.hpp"
#include "comparators.hpp"

namespace simtools {

    struct axis_search_result
    {
        size_t lower = 0;
        size_t upper = 0;
        double slope = 0.0;
    };

    inline void search(const matrix<1>& ind_var, axis_search_result& result, double target) {
        if (ind_var.size() < 2) {
            return;
        }
        if (target <= ind_var[result.lower] || target > ind_var[result.upper]) {
            // ** unit tests with lower_bound all pass, but are marginally slower???
            //auto it = std::lower_bound(ind_var.begin(), ind_var.end(), target);
            auto it = std::find_if(ind_var.begin(), ind_var.end(), comp::greater_than_or_equal(target));
            auto index = it - ind_var.begin();
            result.lower = (index == 0) ? 0 : index - 1;
            auto max = ind_var.size() - 1;
            result.upper = (result.lower == max) ? max : result.lower + 1;
        }
        if (result.lower == result.upper) {
            result.slope = 0.0;
        }
        else {
            const auto lower_value = ind_var[result.lower];
            result.slope = (target - lower_value) / (ind_var[result.upper] - lower_value);
        }
    }

    template<typename T>
    constexpr inline auto interpolate(T lower, T upper, T slope) -> decltype(auto) {
        return (slope*(upper - lower) + lower);
    }

    namespace detail {
        using axis_iterator = std::vector<axis_search_result>::const_iterator;

        template<dim_t N>
        inline auto interpolate(const matrix<N>& values, axis_iterator it) {
            if constexpr (N == 1) {
                return simtools::interpolate(values[it->lower], values[it->upper], it->slope);
            }
            else {
                return simtools::interpolate(interpolate<N - 1>(values[it->lower], it + 1),
                                             interpolate<N - 1>(values[it->upper], it + 1),
                                             it->slope);
            }
        }
    }

    template<dim_t N>
    inline double interpolate(const matrix<N>& data, const std::vector<axis_search_result>& axes) {
        return detail::interpolate<N>(data, axes.begin());
    }

    template<dim_t N>
    inline void search_axes(std::vector<axis_search_result>& searches, const axis_array<N>& axes, const std::array<double, N>& targets) {
        for (auto i = 0U; i < N; ++i) {
            search(axes[i], searches[i], targets[i]);
        }
    }
}

#endif // SIMTOOLS_INTERPOLATOR_HPP