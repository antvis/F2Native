#include "android/F2NativeJNI.h"
#include "android/JNIUtil.h"
#include "graphics/func/Func.h"
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

    nlohmann::json Execute(nlohmann::json t = nlohmann::json()) override {
        JNIEnv *env = GetJniEnvSafe();
        if(env == nullptr || handle_ == nullptr || handle_->isNull()) {
            return nlohmann::json();
        }

        if(nExecuteMethod_ == nullptr) {
            jclass functionClass = env->GetObjectClass(handle_->obj());
            nExecuteMethod_ = env->GetMethodID(functionClass, "nExecute", "(Ljava/lang/String;)Ljava/lang/String;");
        }

        // double check nExecuteMethod_.
        if(nExecuteMethod_ == nullptr || jni::HasException(env)) {
            F2_LOG_E("JavaF2Function", "Get nExecute Method failed.  functionId: %s", functionId.data());
            jni::ClearException(env);
            return nlohmann::json();
        }

        std::string param;
        if(t.is_string()) {
            param = t;
        } else {
            param = t.dump();
        }
        jstring jparam = StringToJString(env, param);
        jstring jrst = (jstring)env->CallObjectMethod(handle_->obj(), nExecuteMethod_, jparam);

        if(jni::HasException(env)) {
            F2_LOG_E("JavaF2Function", "execute nExecute Method failed.  functionId: %s", functionId.data());
            jni::ClearException(env);
            return nlohmann::json();
        }

        std::string result = JavaStringToString(env, jrst);

        nlohmann::json rst = nlohmann::json::parse(result, nullptr, false);
        if(!rst.is_object() && !rst.is_array()) {
            rst = result;
        }
        jni::ClearException(env);
        return rst;
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