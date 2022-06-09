//
// Created by luna on 2022/5/27.
//

#ifndef ANDROID_PHONE_FINANCECHART_BRIDGERAILINGANDROID_H
#define ANDROID_PHONE_FINANCECHART_BRIDGERAILINGANDROID_H

#include <jni.h>
#include "JNIUtil.h"
#include "JavaRef.h"
#include "JavaChartBridgeCallBack.h"
#include <bridge/AbstractBridgeRailing.h>
#include <bridge/ChartBridge.h>

using namespace xg::jni;
using namespace xg::bridge;

namespace xg {
    namespace bridge {

        class BridgeRailingAndroid final : public AbstractBridgeRailing {
        public:
            /**
             * 传入F2BridgeRailing 对象
             */
            BridgeRailingAndroid(jobject nativeRailObject);

            ~BridgeRailingAndroid();

            /**
             * 播放动画
             * @param params 是一个jsonstring，需要解析成平台的map对象，读取其中delay和command两个key，从而延迟执行command
             */
            void PlayAnimation(const std::string &params) override;

            /**
             * 上屏，触发CanvasView系统刷新
             */
            void Swap() override;

            /**
             * 传入一个时区的名字，返回一个时区相对去GMT的偏移，单位是秒
             * @param timezoneName 时区的名字
             * @return 时区的偏移量
             */
            long GetTimezoneOffset(const std::string &timezoneName) override;

            /**
             * 传入一个string的值和时区，按传入的格式化进行格式
             * @param value 时间戳
             * @param timezoneName 时区的名字
             * @param timestampFormatter 格式化的方式 YYYY-MM
             * @return 格式化后的时间
             */
            std::string FormatTime(const std::string &value, const std::string &timezoneName,
                                   const std::string &timestampFormatter) override;
            /**
             *
             * @param methodName invokeMethod的名字
             * @param params 参数
             * @param nativeBridgeObject Android层F2ChartBridge对象，用于反射callback
             * @param chartBridge c++层ChartBridge对象
             */
            void InvokeJavaMethod(const std::string &methodName,
                                  const std::string &params,
                                  jobject nativeBridgeObject,
                                  ChartBridge *chartBridge);
            /**
             * 设置画布的上下文
             * @param context 画布
             */
            inline void SetCanvasContext(void *context) override { context_ = context; }
            inline void *GetCanvasContext() override { return context_; }

        private:
            void *context_;
            JNIEnv *env_;
            ScopedJavaGlobalRef<jobject> *nativeRailObject_ = nullptr;
            jmethodID playAnimation_ = nullptr;
            jmethodID swap_ = nullptr;
            jmethodID getTimezoneOffset_ = nullptr;
            jmethodID formatTime_ = nullptr;
            JavaChartBridgeCallBack *bridgeCallBack_ = nullptr;

        };
    }
}


#endif //ANDROID_PHONE_FINANCECHART_BRIDGERAILINGANDROID_H

