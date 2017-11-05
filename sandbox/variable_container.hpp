#ifndef SIMTOOLS_VARIABLE_CONTAINER_HPP
#define SIMTOOLS_VARIABLE_CONTAINER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <any>
#include <exception>
#include "simtools/simtools_config.hpp"
#include "simtools/matrix.hpp"
#include "simtools/algorithms.hpp"

namespace simtools {

    template<typename T>
    class variable
    {
    public:
        constexpr variable() noexcept = default;
        constexpr variable(std::string_view name, T value) noexcept : m_name(name), m_value(value) { }
        constexpr const std::string& get_name() const {
            return this->m_name;
        }
        constexpr void set_name(std::string_view name) {
            this->m_name = name;
        }
        constexpr T get_value() const {
            return this->m_value;
        }
        constexpr void set_value(T value) {
            this->m_value = value;
        }
    private:
        std::string m_name;
        T m_value;
    };

    template<typename T> struct variable_traits
    {
        using inner_type = typename T;
        using variable_type = typename variable<inner_type>;
        using vector_type = typename std::vector<variable_type>;
        static constexpr auto name() {
            return typeid(inner_type).name();
        }
    };

    class variable_container
    {
    public:
        template<typename T>
        constexpr std::vector<variable<T>> find_all() const {
            using traits = typename variable_traits<T>;
            this->ensure_type_exists<T>();
            const auto& items = this->m_map.at(traits::name());
            constexpr auto variable_cast = [](const std::any& var) {
                return std::any_cast<traits::variable_type>(var);
            };
            return transform<traits::variable_type>(items.begin(), items.end(), variable_cast);
        }

        template<typename T>
        constexpr variable<T> find(std::string_view name) const {
            using traits = typename variable_traits<T>;
            this->ensure_type_exists<T>();
            constexpr auto is_match = [&](const auto& var) {
                return std::any_cast<traits::variable_type>(var).get_name() == name;
            };
            const auto& items = this->m_map.at(traits::name());
            auto it = std::find_if(items.begin(), items.end(), is_match);
            if (it == items.end()) {
                auto msg = std::string(name) + " not found.";
                throw std::exception();
            }
            return std::any_cast<traits::variable_type>(*it);
        }

        template<typename T>
        constexpr void emplace(const std::string& name, T value) {
            using traits = typename variable_traits<T>;
            if (!this->contains_type<T>()) {
                this->m_map.insert(std::make_pair(traits::name(), std::vector<std::any>{}));
            }
            this->m_map.at(traits::name()).emplace_back(std::any{ traits::variable_type(name, value) });
        }

    private:
        std::unordered_map<std::string, std::vector<std::any>> m_map;

        template<typename T>
        constexpr void ensure_type_exists() const {
            using traits = typename variable_traits<T>;
            if (this->contains_type<T>()) return;
            auto msg = "No variables with type "s + traits::name() + " currently stored in this instance.";
            throw std::exception(msg.c_str());
        }

        template<typename T>
        constexpr inline bool contains_type() const {
            using traits = typename variable_traits<T>;
            return this->m_map.find(traits::name()) != this->m_map.end();
        }
    };
}

#endif // SIMTOOLS_VARIABLE_CONTAINER_HPP