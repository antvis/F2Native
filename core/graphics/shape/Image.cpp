//
//  Image.cpp
//  F2
//
//  Created by weiqing.twq on 2021/9/1.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#include "graphics/shape/Image.h"

using namespace xg;

void shape::Image::OnLoad(std::function<void()> finished) {
    callback_ = finished;
}

BBox shape::Image::CalculateBox(canvas::CanvasContext &context) const {
    return {
        .x = static_cast<float>(point_.x),
        .y = static_cast<float>(point_.y),
        .width = static_cast<float>(size_.width),
        .height = static_cast<float>(size_.height),
        .minX = static_cast<float>(point_.x),
        .maxX= static_cast<float>(point_.x + size_.width),
        .minY = static_cast<float>(point_.y),
        .maxY = static_cast<float>(point_.y + size_.height)};
}

void xg::shape::Image::DrawInner(canvas::CanvasContext &context) const {
    // size无效不绘制
    if (std::isnan(size_.width) || std::isnan(size_.height)) {
        return;
    }
    image_->OnLoad([&](void) {
        if (!std::isnan(size_.width) && !std::isnan(size_.height)) {
            context.DrawImage(image_.get(), point_.x, point_.y, size_.width, size_.height);
        }

        if (callback_) {
            callback_();
        }
    });
}
