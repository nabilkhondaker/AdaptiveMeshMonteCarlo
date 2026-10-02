// Dual number implementation is header-only.
// This translation unit exists to satisfy the CMake source list.
#include "ammc/automatic_differentiation/dual.hpp"
namespace ammc::ad {
// Explicit instantiation for double if needed by some compilers.
template struct Dual<double>;
}
