#ifndef SIMTOOLS_TABLEMAP_HPP
#define SIMTOOLS_TABLEMAP_HPP

#include <string>
#include <map>
#include <memory>
#include "interpolator.hpp"

namespace simtools {

    using simtools::axis_search_result;

    class table { };

    template<ndim_t N>
    class data_table : public table
    {
    public:
        data_table() = default;
        data_table(const data_table&) = default;
        data_table(data_table&&) = default;

        data_table(axis_array<N>&& axes, matrix<N>&& data)
            : m_axes(axes),
            m_data(data)
        {
        }

        void load(axis_array<N>&& vars, matrix<N>&& data) {
            this->m_axes = vars;
            this->m_data = data;
        }

        double get_value(const std::array<double, N>& targets) const {
            search_axes(this->m_searches, this->m_axes, targets);
            return interpolate<N>(this->m_data, this->m_searches);
        }

        const matrix<N>* const data() const {
            return &this->m_data;
        }

        const axis_array<N>* const axes() const {
            return &this->m_axes;
        }

    private:
        matrix<N> m_data;
        axis_array<N> m_axes;
        mutable std::vector<axis_search_result> m_searches = std::vector<axis_search_result>(N);
    };

    template<typename TKey>
    class data_table_map
    {
        using table_pointer = std::unique_ptr<table>;
        using table_map = std::map<TKey, table_pointer>;
        using dimensioned_table_map = std::map<ndim_t, table_map>;

    public:
        template<ndim_t N>
        void push(std::string name, const data_table<N>& table) {
            this->m_map[N][name] = table_pointer(new data_table<N>(std::forward<data_table<N>>(table)));
        }

        template<ndim_t N>
        void emplace(std::string name, data_table<N>&& table) {
            this->m_map[N][name] = table_pointer(new data_table<N>(std::forward<data_table<N>>(table)));
        }

        template<typename... TVal>
        double look_up(const TKey& name, TVal... targets) const {
            constexpr auto N = sizeof...(TVal);
            auto table = static_cast<const data_table<N>* const>(this->m_map.at(N).at(name).get());
            return table->get_value({ static_cast<double>(targets)... });
        }

    private:
        dimensioned_table_map m_map;
    };

}

#endif // SIMTOOLS_TABLEMAP_HPP