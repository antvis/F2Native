#include <string>

#if defined(ANDROID)
#include "android/F2NativeJNI.h"
#endif

#if defined(__APPLE__)
#include "ios/F2Logger.h"
#endif

#ifndef X_CHART_UTILS_TRACER_H
#define X_CHART_UTILS_TRACER_H
namespace utils {
class Tracer {
  public:
    Tracer(const std::string &name) : name_(name) {}

    void trace(const char *fmt, ...);

  private:
    std::string name_;
};
} // namespace utils

#endif // X_CHART_UTILS_TRACER_H
