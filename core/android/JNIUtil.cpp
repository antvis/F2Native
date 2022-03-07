#include "JNIUtil.h"

#include <android/log.h>
#include <assert.h>
#include <string>
#include <unistd.h>

#include "JavaRef.h"

namespace xg {
namespace jni {

static JavaVM *g_jvm = nullptr;

#define ASSERT_NO_EXCEPTION() assert(env->ExceptionCheck() == JNI_FALSE);

void InitJavaVM(JavaVM *vm) {
    assert(g_jvm == nullptr);
    g_jvm = vm;
}

JavaVM *GetJVM() { return g_jvm; }

JNIEnv *AttachCurrentThread() {
    JNIEnv *env = nullptr;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_4;
    args.name = nullptr;
    args.group = nullptr;
    args.name = std::to_string(gettid()).data();

    jint ret = g_jvm->AttachCurrentThread(&env, &args);
    // LOGI("AttachCurrentThread:name=%s,ret=%d", (args.name == nullptr ? "[null]" : args.name), ret);
    assert(JNI_OK == ret);
    return env;
}

/**
 * Get a JNIEnv* valid for this thread, regardless of whether
 * we're on a native thread or a Java thread.
 * If the calling thread is not currently attached to the JVM
 * it will be attached, and then automatically detached when the
 * thread is destroyed.
 */
JNIEnv *GetJniEnv() {
    JNIEnv *env = nullptr;
    // We still call GetEnv first to detect if the thread already
    // is attached. This is done to avoid setting up a DetachCurrentThread
    // call on a Java thread.
    // g_vm is a global.
    auto get_env_result = g_jvm->GetEnv((void **)&env, JNI_VERSION_1_4);
    if(get_env_result == JNI_EDETACHED) {
        //            if (g_vm->AttachCurrentThread(&env, NULL) == JNI_OK) {
        //                DeferThreadDetach(env);
        //            } else {
        //                // Failed to attach thread. Throw an exception if you want to.
        //            }
        return nullptr;
    } else if(get_env_result == JNI_EVERSION) {
        return nullptr;
        // Unsupported JNI version. Throw an exception if you want to.
    }
    return env;
}

JNIEnv *GetJniEnvSafe() {
    JNIEnv *env = GetJniEnv();
    if(env == nullptr) {
        env = AttachCurrentThread();
    }
    if(env == nullptr) {
        return nullptr;
    }
    return env;
}

void DetachFromVM() {
    if(g_jvm) {
        g_jvm->DetachCurrentThread();
    }
}

std::string JavaStringToString(JNIEnv *env, jstring str) {
    if(env == nullptr || str == nullptr) {
        return "";
    }

    std::string u8_string;
    const char *rawString = env->GetStringUTFChars(str, 0);
    if(rawString == nullptr) {
        u8_string = "";
    } else {
        u8_string = rawString;
    }
    env->ReleaseStringUTFChars(str, rawString);
    ASSERT_NO_EXCEPTION();
    return u8_string;
}

bool HasException(JNIEnv *env) { return env->ExceptionCheck() != JNI_FALSE; }

bool ClearException(JNIEnv *env) {
    if(!HasException(env))
        return false;
    env->ExceptionDescribe();
    env->ExceptionClear();
    return true;
}

static int getutf8Length(char header) {
    int len = 0;
    char mask = (char)0x80;
    while(header & mask) {
        header <<= 1;
        len++;
    }
    return len == 0 ? 1 : len;
}

static int oneUtf8Convert2Utf16(const std::string &utf8Str, int start, char *output) {
    char b1, b2, b3;
    int utf8Len = getutf8Length(utf8Str[start]);
    // assert(utf8Len >= 1 && utf8Len <= 3);
    if(utf8Len == 1) {
        *output = utf8Str[start];
    } else if(utf8Len == 2) {
        b1 = utf8Str[start];
        b2 = utf8Str[start + 1];
        if((b2 & 0xE0) != 0x80)
            return utf8Len;
        *output = (b1 << 6) + (b2 & 0x3F);
        *(output + 1) = (b1 >> 2) & 0x07;
    } else if(utf8Len == 3) {
        b1 = utf8Str[start];
        b2 = utf8Str[start + 1];
        b3 = utf8Str[start + 2];
        if(((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80))
            return utf8Len;
        *output = (b2 << 6) + (b3 & 0x3F);
        *(output + 1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
    }
    return utf8Len;
}

static std::u16string utf8Couver2Utf16(const std::string &utf8str) {
    std::u16string ret;
    for(auto i = 0; i < utf8str.size();) {
        char *output = NULL;
        output = (char *)malloc(sizeof(char) * 2);
        memset(output, 0x00, 2);
        int size = oneUtf8Convert2Utf16(utf8str, i, output);
        ret.append((char16_t *)output, 1);
        free(output);
        i += size;
        output = NULL;
    }
    return ret;
}

jstring StringToJString(JNIEnv *env, const std::string &u8_string) {
    std::u16string u16_string = utf8Couver2Utf16(u8_string);
    auto result = env->NewString(reinterpret_cast<const jchar *>(u16_string.data()), u16_string.length());
    return result;
}

ScopedJavaLocalRef<jstring> StringToJavaString(JNIEnv *env, const std::string &data) {
    if(data.empty()) { // 空数据guard
        auto result = ScopedJavaLocalRef<jstring>(env, env->NewStringUTF(""));
        ASSERT_NO_EXCEPTION();
        return result;
    } else {
        auto result = ScopedJavaLocalRef<jstring>(env, env->NewStringUTF(data.data()));
        ASSERT_NO_EXCEPTION();
        return result;
    }
}

ScopedJavaLocalRef<jfloatArray> VectorToJFloatArray(JNIEnv *env, const std::vector<float> &params) {
    auto jArray = ScopedJavaLocalRef<jfloatArray>(env, env->NewFloatArray(params.size()));
    env->SetFloatArrayRegion(jArray.obj(), 0, params.size(), params.data());
    return jArray;
}

ScopedJavaLocalRef<jintArray> VectorToJIntArray(JNIEnv *env, const std::vector<int> &params) {
    auto jArray = ScopedJavaLocalRef<jintArray>(env, env->NewIntArray(params.size()));
    env->SetIntArrayRegion(jArray.obj(), 0, params.size(), params.data());
    return jArray;
}

void native_clog(int level, const char *tag, const char *msg) { __android_log_write(level, tag, msg); }

/*
std::string GetJavaExceptionInfo(JNIEnv *env, jthrowable java_throwable)
{
    ScopedJavaLocalRef <jclass> throwable_clazz(
            env, env->FindClass("java/lang/Throwable"));
    jmethodID throwable_printstacktrace = env->GetMethodID(
            throwable_clazz.obj(), "printStackTrace", "(Ljava/io/PrintStream;)V");

    // Create an instance of ByteArrayOutputStream.
    ScopedJavaLocalRef <jclass> bytearray_output_stream_clazz(
            env, env->FindClass("java/io/ByteArrayOutputStream"));
    jmethodID bytearray_output_stream_constructor =
            env->GetMethodID(bytearray_output_stream_clazz.obj(), "<init>", "()V");
    jmethodID bytearray_output_stream_tostring = env->GetMethodID(
            bytearray_output_stream_clazz.obj(), "toString", "()Ljava/lang/String;");
    ScopedJavaLocalRef <jobject> bytearray_output_stream(
            env, env->NewObject(bytearray_output_stream_clazz.obj(),
                                bytearray_output_stream_constructor));

    // Create an instance of PrintStream.
    ScopedJavaLocalRef <jclass> printstream_clazz(
            env, env->FindClass("java/io/PrintStream"));
    jmethodID printstream_constructor = env->GetMethodID(
            printstream_clazz.obj(), "<init>", "(Ljava/io/OutputStream;)V");
    ScopedJavaLocalRef <jobject> printstream(
            env, env->NewObject(printstream_clazz.obj(), printstream_constructor,
                                bytearray_output_stream.obj()));

    // Call Throwable.printStackTrace(PrintStream)
    env->CallVoidMethod(java_throwable, throwable_printstacktrace,
                        printstream.obj());

    // Call ByteArrayOutputStream.toString()
    ScopedJavaLocalRef <jstring> exception_string(
            env,
            static_cast<jstring>(env->CallObjectMethod(
                    bytearray_output_stream.obj(), bytearray_output_stream_tostring)));
    if (ClearException(env))
    {
        return "Java OOM'd in exception handling, check logcat";
    }

    return JavaStringToString(env, exception_string.obj());
}
 */
} // namespace jni

} // namespace xg