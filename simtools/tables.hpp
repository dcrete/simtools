#ifndef SIMTOOLS_TABLES_HPP
#define SIMTOOLS_TABLES_HPP

#include <map>
#include <memory>
#include <string>
#include "simtools_config.hpp"
#include "interpolator.hpp"

namespace simtools {

    using simtools::axis_search_result;

    class table { };

    template<dim_t N>
    class data_table : public table
    {
        using axis_array_type = axis_array<N>;
        using matrix_type = matrix<N>;

    public:
        data_table(const axis_array_type& axes, const matrix_type& data) : m_axes(axes), m_data(data) { }
        data_table(axis_array_type&& axes, matrix_type&& data) : m_axes(axes), m_data(data) { }

        void load(axis_array_type&& vars, matrix_type&& data) {
            this->m_axes = vars;
            this->m_data = data;
        }

        double_t get_value(const std::array<double_t, N>& targets) const {
            search_axes(this->m_searches, this->m_axes, targets);
            return interpolate<N>(this->m_data, this->m_searches);
        }

        const matrix_type* const data() const {
            return &this->m_data;
        }

        const axis_array_type* const axes() const {
            return &this->m_axes;
        }

    private:
        matrix_type m_data;
        axis_array_type m_axes;
        mutable std::vector<axis_search_result> m_searches = std::vector<axis_search_result>(N);
    };

    template<typename TKey>
    class data_table_map
    {
        using table_pointer = std::unique_ptr<table>;
        using table_map = std::map<TKey, table_pointer>;
        using dimensioned_table_map = std::map<dim_t, table_map>;

    public:
        template<dim_t N>
        void push(std::string name, const data_table<N>& table) {
            this->m_map[N][name] = table_pointer(new data_table<N>(std::forward<data_table<N>>(table)));
        }

        template<dim_t N>
        void emplace(std::string name, data_table<N>&& table) {
            this->m_map[N][name] = table_pointer(new data_table<N>(std::forward<data_table<N>>(table)));
        }

        template<dim_t N>
        double_t look_up(const TKey& name, const std::array<double_t, N>& targets) const {
            auto table = static_cast<const data_table<N>* const>(this->m_map.at(N).at(name).get());
            return table->get_value(targets);
        }

        template<typename... TVal>
        double_t look_up(const TKey& name, TVal... targets) const {
            return this->look_up<sizeof...(TVal)>({ static_cast<double_t>(targets)... });
        }

    private:
        dimensioned_table_map m_map;
    };

}

#endif // SIMTOOLS_TABLES_HPP