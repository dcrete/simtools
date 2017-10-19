#ifndef SIMTOOLS_IO_HPP
#define SIMTOOLS_IO_HPP

#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <tuple>
#include "matrix.hpp"

namespace simtools {

    template<ndim_t N>
    inline void print(std::ostream& stream, const matrix<N>& mat) {
        for (auto i = 0U; i < mat.size(); ++i) {
            stream << format("// %i-D (%i)", N, i) << "\n";
            print<N - 1>(stream, data[i]);
        }
    }

    template<>
    inline void print<1>(std::ostream& stream, const matrix<1>& mat) {
        for (auto& value : mat) {
            stream << value;
        }
        stream << "\n";
    }

    template<>
    inline void print<2>(std::ostream& stream, const matrix<2>& mat) {
        for (auto& row : mat) {
            print<1>(stream, row);
        }
    }

    template<ndim_t N>
    inline std::string to_string(const matrix<N>& mat) {
        auto ss = std::ostringstream();
        print(ss, mat);
        return ss.str();
    }

    template<ndim_t N>
    inline void print(std::ostream& stream, const axis_array<N>& axes) {
        for (auto i = 0U; i < N; ++i) {
            stream << "Dim" << std::to_string(i) << " = " << to_string(stream, axes[i]) << "\n";
        }
    }

    template<ndim_t N>
    inline void read(std::istream& stream, matrix<N>& mat) {
        for (auto& row : mat) {
            std::string dummy;
            std::getline(stream, dummy);
            read<N - 1>(stream, row);
        }
    }

    template<>
    inline void read<2>(std::istream& stream, matrix<2>& mat) {
        for (auto& row : mat) {
            read<1>(stream, row);
        }
    }

    template<>
    inline void read<1>(std::istream& stream, matrix<1>& mat) {
        mat.clear();
        while (!stream.eof()) {
            auto value = 0.0;
            stream >> value;
            mat.emplace_back(value);
        }
    }

    template<ndim_t N>
    inline void read(std::istream& stream, axis_array<N>& axes) {
        for (auto i = 0U; i < N; ++i) {
            std::string line;
            std::getline(stream, line);
            auto ss = std::istringstream(line.substr(line.find('=') + 1);
            read(ss, axes[i]);
        }
    }
}

#endif // SIMTOOLS_IO_HPP