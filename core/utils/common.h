#ifndef XG_UTIL_COMMON
#define XG_UTIL_COMMON

#include <float.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

#ifdef DEBUG
#define XG_ASSERT assert
#else
#define XG_ASSERT
#endif

#define XG_EPS DBL_EPSILON

namespace xg {

template <typename T, typename... Args> std::unique_ptr<T> make_unique(Args &&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

bool IsZero(double val) noexcept;
bool IsEqualDeviation (double v1, double v2, double deviation) noexcept;
bool IsEqual(double v1, double v2) noexcept;
} // namespace xg

#endif /* XG_UTIL_COMMON */
