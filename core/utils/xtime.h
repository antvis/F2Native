#ifndef XG_CHART_UTILS_TIME_H
#define XG_CHART_UTILS_TIME_H

#include <ctime>
#include <string>
#include <time.h>

#if defined(__APPLE__)
#include <sys/time.h>
#elif defined(__ANDROID__) || defined(ANDROID)
#endif

namespace xg {

static long CurrentTimestampAtMM() {
    timeval tv;
    gettimeofday(&tv, NULL);
    long now = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return now;
}

static std::string TimeStampToHHmm(long long tsMM) {
    struct tm *p;
    time_t t;
    t = tsMM / 1000;
    p = localtime(&t);
    char s[100];
    strftime(s, 100, "%H:%M", p);
    return std::string(s);
}
} // namespace xg
#endif // XG_CHART_UTILS_TIME_H
