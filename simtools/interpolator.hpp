#ifndef SIMTOOLS_INTERPOLATOR_HPP
#define SIMTOOLS_INTERPOLATOR_HPP

#include <array>
#include <algorithm>
#include "matrix.hpp"

namespace simtools {

    struct axis_search_result
    {
        double target = std::numeric_limits<double>::min();
        ndim_t lower = 0;
        ndim_t upper = 0;
        double slope = 0.0;
        bool is_singular = false;
    };

    inline void search(const matrix<1>& ind_var, const double& target, axis_search_result& result)
    {
        if (result.target == target) return;
        result.target = target;
        auto it = std::find_if(ind_var.begin(), ind_var.end(), [&](const auto& v) {return v >= target;});
        auto index = it - ind_var.begin();
        result.lower = (index == 0) ? 0 : index - 1;
        //std::distance(ind_var.begin(), it);
        auto max = ind_var.size() - 1;
        result.upper = (result.lower == max) ? max : result.lower + 1;
        auto lower_value = ind_var[result.lower];
        auto upper_value = ind_var[result.upper];
        result.is_singular = (result.lower == result.upper || lower_value == upper_value);
        result.slope = result.is_singular ? 0.0 : (target - lower_value) / (upper_value - lower_value);
    }

    template<typename T>
    constexpr inline auto interpolate(T lower, T upper, T slope)
    {
        return slope*(upper - lower) + lower;
    }

    constexpr inline double interpolate(const double& x0, const double& x1, const double& x2, const double& y0, const double& y2)
    {
        auto slope = (x2 == x0) ? 0.0 : (x1 - x0) / (x2 - x0);
        return interpolate(y0, y2, slope);
    }

    namespace detail {
        using axis_iterator = std::vector<axis_search_result>::const_iterator;

        template<ndim_t N>
        inline double interpolate(const matrix<N>& values, axis_iterator it)
        {
            return simtools::interpolate(interpolate<N - 1>(values[it->lower], it + 1),
                                         interpolate<N - 1>(values[it->upper], it + 1),
                                         it->slope);
        }

        template<>
        inline double interpolate<1>(const matrix<1>& values, axis_iterator it)
        {
            return it->is_singular
                ? values[it->lower]
                : simtools::interpolate(values[it->lower], values[it->upper], it->slope);
        }
    }

    template<ndim_t N>
    inline double interpolate(const matrix<N>& data, const std::vector<axis_search_result>& axes) {
        return detail::interpolate<N>(data, axes.begin());
    }

    template<ndim_t N>
    void search_axes(std::vector<axis_search_result>& searches, const axis_array<N>& axes, const std::array<double, N>& targets) { 
        for (auto i = 0U; i < N; ++i) {
            search(axes[i], targets[i], searches[i]);
        }
    }

    template<ndim_t N>
    inline double interpolate(const matrix<N>& data, const axis_array<N>& axes, const std::array<double, N>& targets) {
        auto search_results = std::vector<axis_search_result>(N);
        search_axes(search_results, axes, targets);
        return detail::interpolate(data, search_results);
    }

}

#endif // SIMTOOLS_INTERPOLATOR_HPP