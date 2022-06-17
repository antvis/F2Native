//
// Created by luna on 2022/5/27.
//


#include "BridgeRailingAndroid.h"
#include "BitmapCanvasContext.h"
#include "JavaChartBridgeCallBack.h"
#include <graphics/XChart.h>
#include <bridge/AbstractBridgeRailing.h>
#include <bridge/ChartBridge.h>


using namespace xg;
using namespace xg::bridge;

BridgeRailingAndroid::BridgeRailingAndroid(jobject nativeRailObject) {
    env_ = GetJniEnvSafe();
    nativeRailObject_ = new xg::jni::ScopedJavaGlobalRef<jobject>(env_, nativeRailObject);

    jclass contextCls = env_->GetObjectClass(nativeRailObject_->obj());

    if (env_->ExceptionCheck()) { return; }
    playAnimation_ = env_->GetMethodID(contextCls, "playAnimation", "(Ljava/lang/String;)V");
    F2ASSERT(playAnimation_, "playAnimation_ method is null");

    if (env_->ExceptionCheck()) { return; }
    swap_ = env_->GetMethodID(contextCls, "swap", "()V");
    F2ASSERT(swap_, "swap_ method is null");

    if (env_->ExceptionCheck()) { return; }
    getTimezoneOffset_ = env_->GetMethodID(contextCls, "getTimezoneOffset",
                                           "(Ljava/lang/String;)J");
    F2ASSERT(getTimezoneOffset_, "getTimezoneOffset_ method is null");

    if (env_->ExceptionCheck()) { return; }
    formatTime_ = env_->GetMethodID(contextCls, "formatTime",
                                    "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    F2ASSERT(formatTime_, "formatTime_ method is null");
}

BridgeRailingAndroid::~BridgeRailingAndroid() {
    XG_RELEASE_POINTER(nativeRailObject_);
    XG_RELEASE_POINTER(bridgeCallBack_);
}

void BridgeRailingAndroid::PlayAnimation(const std::string &params) {
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, nativeRailObject_->obj(), playAnimation_,
                         jni::StringToJavaString(env_, params).obj());
}

void BridgeRailingAndroid::Swap() {
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethodNoArg(env_, nativeRailObject_->obj(), swap_);
}

long BridgeRailingAndroid::GetTimezoneOffset(const std::string &timezoneName) {
    env_ = GetJniEnvSafe();
    F2SafeCallLongMethod(env_, nativeRailObject_->obj(), getTimezoneOffset_,
                         jni::StringToJavaString(env_, timezoneName).obj());

}

std::string
BridgeRailingAndroid::FormatTime(const std::string &value, const std::string &timezoneName,
                                 const std::string &formatter) {
    env_ = GetJniEnvSafe();
    ScopedJavaLocalRef<jstring> value_ = jni::StringToJavaString(env_, value);
    ScopedJavaLocalRef<jstring> timezoneName_ = jni::StringToJavaString(env_, timezoneName);
    ScopedJavaLocalRef<jstring> formatter_ = jni::StringToJavaString(env_, formatter);

    F2SafeCallStringMethod(env_, nativeRailObject_->obj(), formatTime_,
                           value_.obj(), timezoneName_.obj(), formatter_.obj());
}


void BridgeRailingAndroid::InvokeJavaMethod(const std::string &methodName,
                                            const std::string &params,
                                            jobject nativeBridgeObject,
                                            ChartBridge *chartBridge) {
    if(chartBridge == nullptr || nativeBridgeObject == nullptr){
        return;
    }
    XG_RELEASE_POINTER(bridgeCallBack_);
    bridgeCallBack_ = new JavaChartBridgeCallBack(nativeBridgeObject);
    chartBridge->InvokeMethod(
            methodName, params,
            XG_MEMBER_OWNER_CALLBACK_1(JavaChartBridgeCallBack::BridgeCallback, bridgeCallBack_));
}
