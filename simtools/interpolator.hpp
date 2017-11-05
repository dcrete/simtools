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
        bool can_interpolate = false;
        double target = std::numeric_limits<double>::min();
    };

    inline void search(const matrix<1>& ind_var, const double& target, axis_search_result& result) {
        if (result.target == target) {
            return;
        }
        if (ind_var.size() < 2) {
            result.can_interpolate = false;
            return;
        }
        if (target <= ind_var[result.lower] || target > ind_var[result.upper]) {
            auto it = std::find_if(ind_var.begin(), ind_var.end(), comp::greater_than_or_equal(target));
            auto index = it - ind_var.begin();
            result.lower = (index == 0) ? 0 : index - 1;
            auto max = ind_var.size() - 1;
            result.upper = (result.lower == max) ? max : result.lower + 1;
        }
        auto lower_value = ind_var[result.lower];
        auto upper_value = ind_var[result.upper];
        result.can_interpolate = (!(result.lower == result.upper || lower_value == upper_value));
        result.slope = result.can_interpolate ? (target - lower_value) / (upper_value - lower_value) : 0.0;
    }

    template<typename T>
    constexpr inline auto interpolate(T lower, T upper, T slope) {
        return slope*(upper - lower) + lower;
    }

    constexpr inline double interpolate(const double& x0, const double& x1, const double& x2, const double& y0, const double& y2) {
        auto slope = (x2 == x0) ? 0.0 : (x1 - x0) / (x2 - x0);
        return interpolate(y0, y2, slope);
    }

    namespace detail {
        using axis_iterator = std::vector<axis_search_result>::const_iterator;

        template<dim_t N>
        inline double interpolate(const matrix<N>& values, axis_iterator it) {
            if constexpr (N == 1) {
                if (!it->can_interpolate) {
                    return values[it->lower];
                }
                return simtools::interpolate(values[it->lower], values[it->upper], it->slope);
            }
            else {
                if (!it->can_interpolate) {
                    return interpolate<N - 1>(values[it->lower], it + 1);
                }
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
    void search_axes(std::vector<axis_search_result>& searches, const axis_array<N>& axes, const std::array<double, N>& targets) { 
        for (auto i = 0U; i < N; ++i) {
            search(axes[i], targets[i], searches[i]);
        }
    }

    template<dim_t N>
    inline double interpolate(const matrix<N>& data, const axis_array<N>& axes, const std::array<double, N>& targets) {
        auto search_results = std::vector<axis_search_result>(N);
        search_axes(search_results, axes, targets);
        return detail::interpolate(data, search_results);
    }
}

#endif // SIMTOOLS_INTERPOLATOR_HPP