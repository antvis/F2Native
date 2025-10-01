#include <float.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

#ifndef XG_UTIL_COMMON
#define XG_UTIL_COMMON

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

static bool IsZero(double val) noexcept { return (fabs(val) < XG_EPS); }
static bool IsEqualDeviation (double v1, double v2, double deviation) noexcept { return fabs(v1 - v2) < deviation; }
static bool IsEqual(double v1, double v2) noexcept { return IsEqualDeviation(v1, v2, XG_EPS); }

static std::string CreateFontStyle(float fontSize = 12.f,
                                const std::string &fontStyle = "normal",
                                const std::string &fontVariant = "normal",
                                const std::string &fontWeight = "normal",
                                const std::string &fontFamily = "sans-serif")  {
    return fontStyle + " " + fontVariant + " " + fontWeight + " " + std::to_string(fontSize) + "px " + fontFamily;
}

} // namespace xg

#endif /* XG_UTIL_COMMON */
