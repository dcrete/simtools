#ifndef SIMTOOLS_CONFIG_HPP
#define SIMTOOLS_CONFIG_HPP

//#define SIMTOOLS_EXPORTS
#define SIMTOOLS_INLINE

#if defined(SIMTOOLS_INLINE)
#define SIMTOOLS_API inline
#elif defined(SIMTOOLS_EXPORTS)
#define SIMTOOLS_API _declspec(dllexport)
#endif

namespace simtools {
    using dim_t = std::size_t;
    using double_t = std::double_t;
}

#endif // SIMTOOLS_CONFIG_HPP