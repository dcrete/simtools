# simtools: C++17 library providing tools for simulation-based applications.

MOTIVATION:

Through my work as an aerospace simulation engineer (currently going on 5 years), I've started to notice several generalized concepts that are often inefficient / poorly implemented. Furthermore, each simulation architecture that I've worked with was targeted towards C++03 or C++11. New features included in the C++14 / C++17 standards provide the tools to develop more efficient and (relatively) simpler implementations of the aforementioned concepts. The objective of this library is to provide header-only implementations for the common tools used in simulation applications based off the most recent C++ standard (currently C++17).

STATUS (currently working features):

- N-dimensional data table with integrated look-up (interpolation)
- N-dimensional data table collection (map) for named-table look-up (and subsequent interpolation)
- N-dimensional matrices (see TODO)

TODO:

- README
- CODE COMMENTS
- Learn more about constexpr and adjust code as needed
- investigate reason for failure of 9-D interpolation test
  * tests will repeatedly fail for tolerances ~< 1E-12
  * floating-point rounding error ??
- integrators
- linear algebra (possibly leverage eigen or similar 3rd-party library)
  * possibly extend current matrix alias for static size definition via arrays (currently based on std::vector)
- tspi (time-space point information) logging/output
- generalized data structure for simulation object (??)
