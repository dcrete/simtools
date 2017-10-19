#ifndef SIMTOOLS_ALGORITHMS_HPP
#define SIMTOOLS_ALGORITHMS_HPP

#include <vector>
#include <array>
#include "matrix.hpp"
#include "tuple_math.hpp"

namespace simtools {
    namespace detail {
        class index_repeater
        {
        public:
            constexpr void set_max_size(ndim_t max_size) {
                this->m_max = max_size;
            }

            constexpr ndim_t current() const {
                return this->m_current;
            }

            constexpr ndim_t increment(ndim_t inc) {
                this->m_current += inc;
                if (this->m_current < this->m_max) return static_cast<ndim_t>(false);
                this->m_current = 0;
                return static_cast<ndim_t>(true);
            }

        private:
            ndim_t m_max = 0;
            ndim_t m_current = 0;
        };

        template<ndim_t N>
        class n_dim_indexer
        {
        public:
            n_dim_indexer(std::array<ndim_t, N> max_sizes) {
                for (auto i = 0U; i < N; ++i) {
                    this->m_indices[i].set_max_size(max_sizes[i]);
                }
            }

            constexpr n_dim_indexer& operator++() {
                static_assert(N > 0, "N must be greater than 0");
                auto inc = static_cast<ndim_t>(1);
                constexpr auto start = static_cast<int>(N - 1);
                for (auto i = start; i >= 0; --i) {
                    inc = this->m_indices[i].increment(inc);
                }
                return *this;
            }

            ndim_t operator[](ndim_t index) const {
                return this->m_indices[index].current();
            }

        private:
            std::array<index_repeater, N> m_indices;
        };
    }

    template<typename T, ndim_t N>
    constexpr inline std::vector<T> to_vector(const std::array<T, N>& arr) {
        return std::vector<T>(arr.begin(), arr.end());
    }

    template<ndim_t N>
    constexpr inline matrix<N> make_matrix(std::vector<ndim_t>::const_iterator it) {
        auto m = matrix<N>(*it);
        std::for_each(std::begin(m), std::end(m), [&](auto& row) { row = make_matrix<N - 1>(it + 1); });
        return m;
    }

    template<>
    inline matrix<1> make_matrix<1>(std::vector<ndim_t>::const_iterator it)
    {
        return matrix<1>(*it);
    }

    template<ndim_t N>
    constexpr inline matrix<N> make_matrix(std::array<ndim_t, N> sizes) {
        auto v = to_vector(sizes);
        return make_matrix<N>(v.begin());
    }

    template<typename... T, ndim_t N = sizeof...(T)>
    constexpr inline matrix<N + 1> make_matrix(ndim_t size, T... sizes) {
        return matrix<N + 1>(size, make_matrix(sizes...));
    }

    inline matrix<1> make_matrix(ndim_t size) {
        return matrix<1>(size);
    }

    template<typename T, ndim_t N>
    constexpr inline std::array<T, N> make_array(std::function<T(ndim_t)> func) {
        auto result = std::array<T, N>();
        for (auto i = 0U; i < N; ++i) {
            result[i] = func(i);
        }
        return result;
    }

    template<ndim_t N>
    constexpr inline std::array<ndim_t, N> get_sizes(const axis_array<N>& v) {
        return make_array<ndim_t, N>([&](auto i) { return v[i].size(); });
    }

    template<typename T1, typename... T2>
    constexpr inline std::array<T1, sizeof...(T2)> make_array(T2... vars) {
        return { static_cast<T1>(vars)... };
    }

    template<typename T>
    constexpr inline std::vector<T> get_range(const std::vector<T>& from, ndim_t offset, ndim_t count = 0) {
        auto first = from.begin() + offset;
        auto last = (count == 0) ? from.end() : first + count;
        return std::vector<T>(first, last);
    }

    template<ndim_t Offset, ndim_t Count, typename T>
    constexpr inline std::vector<T> get_range(const std::vector<T>& from) {
        auto start = std::begin(from) + Offset;
        return std::vector<T>(start, start + Count);
    }

    template<ndim_t Offset, ndim_t Count, typename T, ndim_t N>
    constexpr inline typename std::enable_if<(Offset + Count) <= N, std::array<T, Count>>::type get_range(const std::array<T, N>& from) {
        return make_array<T, Count>([&](auto i) { return from[Offset + i]; });
    }

    template<typename T1, typename T2, ndim_t N, typename tuple_type = std::tuple<T1, T2>>
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

    template<typename T, ndim_t N, ndim_t M>
    constexpr inline std::array<T, N*M> duplicate(const std::array<T, N>& items) {
        return make_array<T, N*M>([&](auto i) { return items[N%M]; });
    }

    template<typename T, typename T1, typename T2, ndim_t N>
    constexpr inline typename
        std::enable_if<(std::is_same<T, T1>::value || std::is_same<T, T2>::value) && !std::is_same<T1, T2>::value, std::array<T, N>>::type
        splice(const std::array<std::tuple<T1, T2>, N>& in) {
        auto result = std::array<T, N>();
        for (auto i = 0U; i < N; ++i) {
            result[i] = std::get<T>(in[i]);
        }
        return result;
    }

    namespace detail {

        template<ndim_t N, typename T>
        constexpr inline std::vector<std::array<T, N>> get_permutations(const axis_array<N>& axes, std::function<T(ndim_t, const matrix<1>&)> func) {
            auto sizes = get_sizes(axes);
            constexpr auto multiply = [](auto val, auto size) { return val * size; };
            auto count = std::accumulate(sizes.begin(), sizes.end(), static_cast<ndim_t>(1), multiply);
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

    template<ndim_t N>
    constexpr inline std::vector<std::array<double, N>> get_permutations(const axis_array<N>& axes) {
        auto func = [](ndim_t index, const matrix<1>& vars) { return vars[index]; };
        return detail::get_permutations<N, double>(axes, func);
    }

    template<ndim_t N>
    constexpr inline std::vector<std::array<std::tuple<ndim_t, double>, N>> get_permutations_with_indices(const axis_array<N>& axes) {
        auto func = [](ndim_t index, const matrix<1>& vars) { return std::make_tuple(index, vars[index]); };
        return detail::get_permutations<N, std::tuple<ndim_t, double>>(axes, func);
    }

    template<ndim_t N>
    const auto& get_ref(const matrix<N>& m, std::array<ndim_t, N> indices) {
        return get_ref(m[indices[0]], get_range<1, N - 1>(indices));
    }

    template<>
    const auto& get_ref<1>(const matrix<1>& m, std::array<ndim_t, 1> indices) {
        return m.at(indices[0]);
    }

    template<ndim_t N>
    auto& get_ref(matrix<N>& m, std::array<ndim_t, N> indices) {
        return get_ref(m[indices[0]], get_range<1, N - 1>(indices));
    }

    template<>
    auto& get_ref<1>(matrix<1>& m, std::array<ndim_t, 1> indices) {
        return m.at(indices[0]);
    }

    template<ndim_t N>
    constexpr void set_value(matrix<N>& m, std::array<ndim_t, N> indices, double value) {
        get_ref(m, indices) = value;
    }
}

#endif // SIMTOOLS_ALGORITHMS_HPP