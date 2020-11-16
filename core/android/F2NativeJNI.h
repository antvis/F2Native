
#include <jni.h>
#include <string>

#include <GCanvas/core/src/support/Log.h>

#include "JNIUtil.h"

#ifndef XG_F2_NATIVE_ANDROID_JNI_H
#define XG_F2_NATIVE_ANDROID_JNI_H

#define CODE_SUCCESS 0
#define CODE_FAIL_UNKNOWN -1
#define CODE_INIT_EGL_FAIL 1
#define CODE_INIT_CANVAS_CONTEXT_FAIL 2
#define CODE_CREATE_CANVAS_2D_CONTEXT_FAIL 3
#define CODE_RESUME_CANVAS_FAIL 4

#define LOG_BUFFER_SIZE 2048

namespace xg {
namespace jni {

void InnerLog(int level, std::string traceId, const char *fmt, ...);

} // namespace jni
} // namespace xg

#ifndef F2_LOG_I
#define F2_LOG_I(traceId, ...) ((void)xg::jni::InnerLog(ANDROID_LOG_INFO, traceId, __VA_ARGS__))
#endif // F2_LOG_I

#ifndef F2_LOG_W
#define F2_LOG_W(traceId, ...) ((void)xg::jni::InnerLog(ANDROID_LOG_WARN, traceId, __VA_ARGS__))
#endif // F2_LOG_W

#ifndef F2_LOG_E
#define F2_LOG_E(traceId, ...) ((void)xg::jni::InnerLog(ANDROID_LOG_ERROR, traceId, __VA_ARGS__))
#endif // F2_LOG_E

#endif // XG_F2_NATIVE_ANDROID_JNI_H
