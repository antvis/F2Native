//
//  CanvasImage.cpp
//  F2
//
//  Created by weiqing.twq on 2022/1/17.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#if defined(ANDROID)

#include "graphics/canvas/CanvasImage.h"
#include <jni.h>
#include "JNIUtil.h"

using namespace xg;
using namespace xg::canvas;
using namespace xg::jni;


CanvasImage::~CanvasImage() {
    this->image_ = nullptr;
    this->finished_ = nullptr;
}

void CanvasImage::OnLoad(std::function<void()> finished) {
    this->finished_ = finished;
    JNIEnv *env_ = GetJniEnvSafe();
    ScopedJavaLocalRef<jstring> urlStr_ = StringToJavaString(env_, this->url_);
    jclass clzz = env_->FindClass("com/antgroup/antv/f2/F2ImageLoadUtil");
    jmethodID methodID = env_->GetStaticMethodID(clzz, "loadImage", "(Ljava/lang/String;J)V");
    // C调用java
    long canvasImageHandler = reinterpret_cast<jlong>(this);
    env_->CallStaticVoidMethod(clzz, methodID, urlStr_.obj(), canvasImageHandler);
}

void CanvasImage::OnLoadFinish(void *image, bool isSuccess) {
    if (isSuccess) {
        this->image_ = image;
        if (this->finished_) {
            this->finished_();
        }
    }
}

float CanvasImage::GetWidth() {
    if (this->image_ != nullptr) {
        JNIEnv *env_ = GetJniEnvSafe();
        jclass clzz = env_->FindClass("android/graphics/Bitmap");
        jmethodID getWidth_ = env_->GetMethodID(clzz, "getWidth", "()I");
        env_->CallIntMethod((jobject) this->image_, getWidth_);

    }
    return 0;
}

float CanvasImage::GetHeight() {
    if (this->image_ != nullptr) {
        JNIEnv *env_ = GetJniEnvSafe();
        jclass clzz = env_->FindClass("android/graphics/Bitmap");
        jmethodID getHeight_ = env_->GetMethodID(clzz, "getHeight", "()I");
        env_->CallIntMethod((jobject) this->image_, getHeight_);
    }
    return 0;
}

#endif //ANDROID
