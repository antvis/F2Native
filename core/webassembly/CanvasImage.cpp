//
//  CanvasImage.cpp
//  F2
//
//  Created by weiqing.twq on 2022/1/17.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#ifdef __EMSCRIPTEN__

#include "graphics/canvas/CanvasImage.h"

using namespace xg;
using namespace xg::canvas;

CanvasImage::~CanvasImage() {
    if(image_) {
        image_ = nullptr;
    }
}

void CanvasImage::OnLoad(std::function<void()> finished) {
    //通过JNI方法调用Java下载
}

float CanvasImage::GetWidth() { return 0; }

float CanvasImage::GetHeight() { return 0; }

#endif