#include <jni.h>
#include <string>
#include <map>
#include <iostream>
#include <vector>
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

ScopedJavaLocalRef<jfloatArray> VectorToJFloatArray(JNIEnv *env, const std::vector<float> &params);

ScopedJavaLocalRef<jintArray> VectorToJIntArray(JNIEnv *env, const std::vector<int> &params);

bool HasException(JNIEnv *env);

bool ClearException(JNIEnv *env);

std::string JavaStringToString(JNIEnv *env, jstring string);

jstring StringToJString(JNIEnv *env, const std::string &u8_string);

std::map<int, int> JavaMapIntIntToMap(JNIEnv *env, jobject jmap);

std::map<std::string,int> JavaMapStringIntToMap(JNIEnv *env, jobject jmap);

void native_clog(int level, const char *tag, const char *msg);

jstring jniStringFromUTF8(JNIEnv * env, const std::string & str);

// std::string GetJavaExceptionInfo(JNIEnv *env, jthrowable java_throwable);
} // namespace jni
} // namespace xg

#endif // XG_F2_NATIVE_ANDROID_JNI_UTILS_H
