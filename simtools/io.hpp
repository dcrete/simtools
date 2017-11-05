#ifndef SIMTOOLS_IO_HPP
#define SIMTOOLS_IO_HPP

#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <ostream>
#include <istream>
#include "simtools_config.hpp"
#include "matrix.hpp"

namespace simtools {

    template<typename T>
    inline std::ostream& operator<<(std::ostream& stream, const std::vector<T>& data) {
        for (auto i = 0U; i < data.size(); ++i) {
            if (i > 0) {
                stream << " ";
            }
            stream << data[i];
        }
        stream << "\n";
        return stream;
    }

    template<typename T, dim_t N>
    inline std::ostream& operator<<(std::ostream& stream, const base_matrix<T, N>& data) {
        for (auto i = 0U; i < data.size(); ++i) {
            if constexpr (N > 2) {
                stream << "// " << std::to_string(N) << "-D (" << std::to_string(i) << ")" << "\n";
            }
            stream << data[i];
        }
        return stream;
    }

    template<dim_t N>
    inline std::ostream& operator<<(std::ostream& stream, const axis_array<N>& axes) {
        for (auto i = 0U; i < N; ++i) {
            stream << "Dim" << std::to_string(i) << " = " << axes[i] << "\n";
        }
    }

    template<dim_t N>
    inline std::string to_string(const matrix<N>& data) {
        std::ostringstream ss;
        ss << data;
        return ss.str();
    }

    template<dim_t N>
    inline void read(std::istream& stream, matrix<N>& data) {
        if constexpr (N == 1) {
            data.clear();
            while (!stream.eof()) {
                auto value = 0.0;
                stream >> value;
                data.emplace_back(value);
            }
        }
        else {
            for (auto& row : data) {
                if constexpr (N > 2) {
                    std::string _;
                    std::getline(stream, _);
                }
                read<N - 1>(stream, row);
            }
        }
    }

    template<dim_t N>
    inline void read(std::istream& stream, axis_array<N>& axes) {
        for (auto& axis : axes) {
            std::string line;
            std::getline(stream, line);
            std::istringstream stream(line.substr(line.find('=') + 1));
            read(stream, axis);
        }
    }
}

#endif // SIMTOOLS_IO_HPP