//
// Created by luna on 2022/5/27.
//
#include <string>
#include <functional>
#include <jni.h>

#include "JavaChartBridgeCallBack.h"
#include "bridge/ChartBridge.h"
#include "BridgeRailingAndroid.h"

using namespace xg;
using namespace xg::bridge;

JavaChartBridgeCallBack::JavaChartBridgeCallBack(jobject nativeBridgeObject) {
    env_ = GetJniEnvSafe();
    nativeBridgeObject_ = new xg::jni::ScopedJavaGlobalRef<jobject>(env_, nativeBridgeObject);
}

JavaChartBridgeCallBack::~JavaChartBridgeCallBack() {
    XG_RELEASE_POINTER(nativeBridgeObject_);
}

void
JavaChartBridgeCallBack::BridgeCallback(const std::string &result) {
    F2_LOG_I("#F2ChartBridge", "#BridgeCallback %s ", result.data());
    if (!InitClass(env_)) {
        F2_LOG_I("#F2ChartBridge", "#BridgeCallback %s ", "fail init");
        return;
    }
    if (nativeBridgeObject_ == nullptr || nativeBridgeObject_->isNull() || invokeCallbackMethod_ == nullptr) {
        F2_LOG_I("#F2ChartBridge", "#BridgeCallback %s ", "fail null");
        return;
    }

    F2SafeCallVoidMethod(env_, nativeBridgeObject_->obj(), invokeCallbackMethod_,
                         jni::StringToJavaString(env_, result).obj());

}

bool JavaChartBridgeCallBack::InitClass(JNIEnv *env) {
    if (invokeCallbackMethod_ != nullptr) {
        return true;
    }
    F2_LOG_I("#F2ChartBridge", " %s ", "InitClass");
    if (env == nullptr || nativeBridgeObject_ == nullptr) {
        return false;
    }
    jclass bridgeCls = env->GetObjectClass(nativeBridgeObject_->obj());
    invokeCallbackMethod_ = env->GetMethodID(bridgeCls, "nInvokeCallback",
                                             "(Ljava/lang/String;)V");
    if (HasException(env)) {
        return false;
    }
    return true;
}
