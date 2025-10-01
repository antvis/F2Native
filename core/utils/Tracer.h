#include <string>

#if defined(ANDROID)
#include "android/F2NativeJNI.h"
#endif

#if defined(__APPLE__)
#include "../ios/F2Logger.h"
#endif

#ifndef X_CHART_UTILS_TRACER_H
#define X_CHART_UTILS_TRACER_H
namespace utils {
class Tracer {
  public:
    Tracer(const std::string &name, bool log = true) : name_(name), log_(log) {}

    void trace(const char *fmt, ...);

  private:
    std::string name_;
    bool log_;
};
} // namespace utils

#endif // X_CHART_UTILS_TRACER_H
