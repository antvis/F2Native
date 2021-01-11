#include <jni.h>
#include <string>

#include "JavaRef.h"

#ifndef XG_F2_NATIVE_ANDROID_JNI_UTILS_H
#define XG_F2_NATIVE_ANDROID_JNI_UTILS_H

namespace xg {
namespace jni {
void InitJavaVM(JavaVM *vm);

JavaVM *GetJVM();

JNIEnv *GetJniEnv();

JNIEnv *GetJniEnvSafe();

JNIEnv *AttachCurrentThread();

void DetachFromVM();

ScopedJavaLocalRef<jstring> StringToJavaString(JNIEnv *env, const std::string &str);

bool HasException(JNIEnv *env);

bool ClearException(JNIEnv *env);

std::string JavaStringToString(JNIEnv *env, jstring string);

jstring StringToJString(JNIEnv *env, const std::string &u8_string);

void native_clog(int level, const char *tag, const char *msg);

// std::string GetJavaExceptionInfo(JNIEnv *env, jthrowable java_throwable);
} // namespace jni
} // namespace xg

#endif // XG_F2_NATIVE_ANDROID_JNI_UTILS_H
