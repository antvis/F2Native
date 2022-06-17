#include "android/F2NativeJNI.h"
#include "android/JNIUtil.h"
#include "graphics/func/Func.h"
#include "graphics/util/json.h"
#include <jni.h>
#include <nlohmann/json.hpp>

#ifndef XG_F2_NATIVE_ANDROID_JNI_RUNNABLE_H
#define XG_F2_NATIVE_ANDROID_JNI_RUNNABLE_H

namespace xg {
namespace jni {

static jmethodID nExecuteMethod_ = nullptr;

class JavaF2Function : public func::F2Function {
  public:
    JavaF2Function(ScopedJavaGlobalRef<jobject> *_handle) : func::F2Function(), handle_(_handle) {}

    const std::string Execute(const std::string &functionId, const std::string &param) override {
        JNIEnv *env = GetJniEnvSafe();
        if(nExecuteMethod_ == nullptr) {
            jclass functionClass = env->GetObjectClass(handle_->obj());
            nExecuteMethod_ = env->GetMethodID(functionClass, "nExecute", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        }

        // double check nExecuteMethod_.
        if(nExecuteMethod_ == nullptr || jni::HasException(env)) {
            jni::ClearException(env);
            F2_LOG_E("JavaF2Function", "Get nExecute Method failed.  functionId: %s", functionId.data());
            return "{}";
        }

        jstring jfunctionId = StringToJString(env, functionId);
        jstring jparam = StringToJString(env, param);
        jstring jrst = (jstring)env->CallObjectMethod(handle_->obj(), nExecuteMethod_, jfunctionId, jparam);

        if(jni::HasException(env)) {
            jni::ClearException(env);
            F2_LOG_E("JavaF2Function", "execute nExecute Method failed.  functionId: %s", functionId.data());
            return "{}";
        }

        std::string result = JavaStringToString(env, jrst);
        jni::ClearException(env);
        return result;
    }

    ~JavaF2Function() override {
        handle_->Reset();
        delete handle_;
        handle_ = nullptr;
    }

  private:
    ScopedJavaGlobalRef<jobject> *handle_ = nullptr;
};
} // namespace jni
} // namespace xg

#endif // XG_F2_NATIVE_ANDROID_JNI_RUNNABLE_H