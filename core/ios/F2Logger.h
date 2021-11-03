#ifndef XG_F2_NATIVE_F2LOGGER_H
#define XG_F2_NATIVE_F2LOGGER_H

#include <stdio.h>
#include <string>

namespace xg {
namespace ios {

void InnerLog(int level, const std::string &traceId, const char *fmt, ...);

}
}

#define F2_LOG_INFO 1
#define F2_LOG_WARN 2
#define F2_LOG_ERROR 3

#define F2_LOG_I(traceId, ...) ((void)xg::ios::InnerLog(F2_LOG_INFO, traceId, __VA_ARGS__))
#define F2_LOG_W(traceId, ...) ((void)xg::ios::InnerLog(F2_LOG_WARN, traceId, __VA_ARGS__))
#define F2_LOG_E(traceId, ...) ((void)xg::ios::InnerLog(F2_LOG_ERROR, traceId, __VA_ARGS__))

#endif /* XG_F2_NATIVE_F2LOGGER_H */
