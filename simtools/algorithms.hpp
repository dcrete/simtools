#ifndef SIMTOOLS_ALGORITHMS_HPP
#define SIMTOOLS_ALGORITHMS_HPP

#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <functional>
#include "simtools_config.hpp"
#include "matrix.hpp"
#include "tuple_math.hpp"
#include "factory.hpp"

namespace simtools {
    namespace detail {
        class index_repeater
        {
        public:
            constexpr void set_max_size(dim_t max_size) {
                this->m_max = max_size;
            }

            constexpr dim_t current() const {
                return this->m_current;
            }

            constexpr dim_t increment(dim_t inc) {
                this->m_current += inc;
                if (this->m_current < this->m_max) return static_cast<dim_t>(false);
                this->m_current = 0;
                return static_cast<dim_t>(true);
            }

        private:
            dim_t m_max = 0;
            dim_t m_current = 0;
        };

        template<dim_t N>
        class n_dim_indexer
        {
        public:
            constexpr n_dim_indexer(std::array<dim_t, N> max_sizes) {
                for (auto i = 0U; i < N; ++i) {
                    this->m_indices[i].set_max_size(max_sizes[i]);
                }
            }

            constexpr n_dim_indexer& operator++() {
                static_assert(N > 0, "N must be greater than 0");
                auto inc = static_cast<dim_t>(1);
                constexpr auto start = static_cast<int>(N - 1);
                for (auto i = start; i >= 0; --i) {
                    inc = this->m_indices[i].increment(inc);
                }
                return *this;
            }

            constexpr dim_t operator[](dim_t index) const {
                return this->m_indices[index].current();
            }

        private:
            std::array<index_repeater, N> m_indices;
        };
    }

    template<typename T, dim_t N>
    constexpr inline std::vector<T> to_vector(const std::array<T, N>& arr) {
        return std::vector<T>(arr.begin(), arr.end());
    }

    template<dim_t N>
    constexpr inline std::array<dim_t, N> get_sizes(const axis_array<N>& v) {
        return factory::make_array<dim_t, N>([&](auto index) { return static_cast<dim_t>(v[index].size()); });
    }

    template<typename T>
    constexpr inline std::vector<T> get_range(const std::vector<T>& from, dim_t offset, dim_t count = 0) {
        auto first = from.begin() + offset;
        auto last = (count == 0) ? from.end() : first + count;
        return std::vector<T>(first, last);
    }

    template<dim_t Offset, dim_t Count, typename T>
    constexpr inline std::vector<T> get_range(const std::vector<T>& from) {
        auto start = std::begin(from) + Offset;
        return std::vector<T>(start, start + Count);
    }

    template<dim_t Offset, dim_t Count, typename T, dim_t N>
    constexpr inline typename std::enable_if<(Offset + Count) <= N, std::array<T, Count>>::type get_range(const std::array<T, N>& from) {
        return factory::make_array<T, Count>([&](auto index) { return from[Offset + index]; });
    }

    template<typename T1, typename T2, dim_t N, typename tuple_type = std::tuple<T1, T2>>
    constexpr inline auto zip(const std::array<T1, N>& left, const std::array<T2, N>& right) {
        auto result = std::array<tuple_type, N>();
        for (auto i = 0U; i < N; ++i) {
            result[i] = tuple_type(left[i], right[i]);
        }
        return result;
    }

    template<typename Out, typename It, typename F>
    constexpr inline std::vector<Out> transform(It begin, It end, F func) {
        auto result = std::vector<Out>(std::distance(begin, end));
        for (auto it = begin; it != end; ++it) {
            result[std::distance(begin, it)] = func(*it);
        }
        return result;
    }

    template<typename T>
    constexpr inline T sum(const std::vector<T>& items) {
        return std::accumulate(std::begin(items), std::end(items), T(),
            [](auto left, auto right) { return left + right; });
    }

    template<typename T, typename T1, typename T2, dim_t N>
    constexpr inline typename std::enable_if<(std::is_same<T, T1>::value || std::is_same<T, T2>::value) && !std::is_same<T1, T2>::value, std::array<T, N>>::type splice(const std::array<std::tuple<T1, T2>, N>& tuple_array) {
        return factory::make_array<T, N>([&](auto index) { return std::get<T>(tuple_array[index]); });
    }

    namespace detail {

        template<dim_t N, typename T>
        constexpr inline std::vector<std::array<T, N>> get_permutations(const axis_array<N>& axes, std::function<T(dim_t, const matrix<1>&)> func) {
            auto sizes = get_sizes(axes);
            constexpr auto multiply = [](auto val, auto size) { return val * size; };
            auto count = std::accumulate(sizes.begin(), sizes.end(), static_cast<dim_t>(1), multiply);
            auto permutations = std::vector<std::array<T, N>>(count);
            auto indexer = detail::n_dim_indexer<N>(sizes);
            for (auto i = 0U; i < count; ++i) {
                for (auto j = 0U; j < N; ++j) {
                    auto index = indexer[j];
                    permutations[i][j] = func(index, axes[j]);
                }
                ++indexer;
            }
            return permutations;
        }
    }

    template<dim_t N>
    constexpr inline std::vector<std::array<double, N>> get_permutations(const axis_array<N>& axes) {
        auto func = [](dim_t index, const matrix<1>& vars) { return vars[index]; };
        return detail::get_permutations<N, double>(axes, func);
    }

    template<dim_t N>
    constexpr inline std::vector<std::array<std::tuple<dim_t, double>, N>> get_permutations_with_indices(const axis_array<N>& axes) {
        auto func = [](dim_t index, const matrix<1>& vars) { return std::make_tuple(index, vars[index]); };
        return detail::get_permutations<N, std::tuple<dim_t, double>>(axes, func);
    }

    template<dim_t N>
    const auto& get_ref(const matrix<N>& m, std::array<dim_t, N> indices) {
        if constexpr (N == 1) {
            return m.at(indices[0]);
        }
        else {
            return get_ref(m[indices[0]], get_range<1, N - 1>(indices));
        }
    }

    template<dim_t N>
    auto& get_ref(matrix<N>& m, std::array<dim_t, N> indices) {
        if constexpr (N == 1) {
            return m.at(indices[0]);
        }
        else {
            return get_ref(m[indices[0]], get_range<1, N - 1>(indices));
        }
    }

    template<dim_t N>
    constexpr void set_value(matrix<N>& m, std::array<dim_t, N> indices, double value) {
        get_ref(m, indices) = value;
    }

    template<dim_t N>
    constexpr inline bool operator!=(const matrix<N>& left, const matrix<N>& right) {
        auto left_sizes = get_sizes(left);
        auto right_sizes = get_size(right);
        if (left_sizes != right_sizes) return true;
        for (auto i = 0U; i < N; ++i) {
            if (left[i] != right[i]) return true;
        }
        return false;
    }

    template<dim_t N>
    constexpr inline bool operator==(const matrix<N>& left, const matrix<N>& right) {
        return !(left != right);
    }
}

#endif // SIMTOOLS_ALGORITHMS_HPP