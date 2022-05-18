#include "Tracer.h"
#include <stdarg.h>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

void utils::Tracer::trace(const char *fmt, ...) {
    char buffer[4096] = {0};

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1, fmt, args);
    va_end(args);

#ifdef ANDROID
    F2_LOG_I(this->name_.data(), "%s", buffer);
#elif defined(TARGET_OS_IPHONE)
    F2_LOG_I(this->name_.data(), "%s", buffer);
#else
    printf("%s %s\n", this->name_.data(), buffer);
#endif
}
