#ifndef XG_CHART_UTILS_TIME_H
#define XG_CHART_UTILS_TIME_H

#include <chrono>
#include <ctime>
#include <string>
#include <time.h>

#if defined(__APPLE__) || defined(__EMSCRIPTEN__)
#include <sys/time.h>
#elif defined(__ANDROID__) || defined(ANDROID)
#endif

using namespace std;

namespace xg {

static long long CurrentTimestampAtMM() {
    timeval tv;
    gettimeofday(&tv, NULL);
    long long now = (static_cast<long long>(tv.tv_sec) * 1000LL) + (static_cast<long long>(tv.tv_usec) / 1000);
    return now;
}

static string TimeStampToHHmm(long long tsMM, bool timeZone = false) {
    struct tm *p;
    time_t t;
    t = tsMM / 1000;
    if(timeZone) {
        p = gmtime(&t);
    } else {
        p = localtime(&t);
    }
    char s[100];
    strftime(s, 100, "%H:%M", p);
    return std::string(s);
}

static string TimeStampToFormat(long long tsMM, bool timeZone = false, const char *format = "%Y-%m-%d %H:%M:%S") {
    struct tm *p;
    time_t t;
    t = tsMM / 1000;
    if(timeZone) {
        p = gmtime(&t);
    } else {
        p = localtime(&t);
    }
    char s[100];
    strftime(s, 100, format, p);
    return std::string(s);
}

/// 字符串格式转化为秒级时间戳
/// 2020-01-01 14:00:00 --> 1577858400
static time_t DateParserAtSecond(std::string date, std::string format = "%Y-%m-%d %H:%M:%S") {
    tm tm_;
    time_t t_;
    char buf[128] = {0};

    strcpy(buf, date.data());
    strptime(buf, format.data(), &tm_); //将字符串转换为tm时间
    tm_.tm_isdst = -1;
    t_ = mktime(&tm_); //将tm时间转换为秒时间
    return t_;
}

/// 字符串格式转化为 tm 结构
/// 2020-01-01 14:00:00 --> std::tm
static tm DateParserAtTM(std::string date, std::string format = "%Y-%m-%d %H:%M:%S") {
    tm tm_;
    time_t t_;
    char buf[128] = {0};

    strcpy(buf, date.data());
    strptime(buf, format.data(), &tm_); //将字符串转换为tm时间
    tm_.tm_isdst = -1;
    return std::move(tm_);
}

static tm DateParserTimeStamp(long long timestamp, bool timeZone = false) {
    time_t t = timestamp / 1000;

    std::tm rt = {0};
    if(timeZone) {
        gmtime_r(&t, &rt);
    } else {
        localtime_r(&t, &rt);
    }
    return std::move(rt);
}

} // namespace xg
#endif // XG_CHART_UTILS_TIME_H
