//
// Created by luna on 2022/5/27.
//

#ifndef CHART_NATIVE_CORE_JAVACHARTBRIDGECALLBACK_H
#define CHART_NATIVE_CORE_JAVACHARTBRIDGECALLBACK_H

#include <string>
#include <jni.h>
#include "JavaRef.h"

namespace xg {
    namespace bridge {
        class JavaChartBridgeCallBack {
        public:
            /**
             * nativeBridgeObject 传入Android层的F2ChartBridge对象
             */
            JavaChartBridgeCallBack(jobject nativeBridgeObject);

            ~JavaChartBridgeCallBack();

            /**
             * chartbridge callback的函数，反射调用Android层的函数
             * @param message callback的数据
             */
            void BridgeCallback(const std::string &message);

        private:
            /**
             * load class
             */
            bool InitClass(JNIEnv *env);

            JNIEnv *env_ = nullptr;
            jmethodID invokeCallbackMethod_ = nullptr;
            // 用于反射callback
            xg::jni::ScopedJavaGlobalRef<jobject> *nativeBridgeObject_ = nullptr;

        };
    }
}


#endif //CHART_NATIVE_CORE_JAVACHARTBRIDGECALLBACK_H
