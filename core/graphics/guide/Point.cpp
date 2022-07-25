//
//  Circle.cpp
//  F2
//
//  Created by weiqing.twq on 2021/9/9.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#include "Point.h"
#include "../XChart.h"
#include "../global.h"
#include "../shape/Circle.h"

using namespace xg;

void guide::Point::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string &yField = yFields[0];

    util::Point position = this->GetPosition(chart, json::GetArray(this->config_, "position"), xField, yField);
    double offsetX = json::GetNumber(config_, "offsetX");
    double offsetY = json::GetNumber(config_, "offsetY");
    double size = json::GetNumber(config_, "size") * context.GetDevicePixelRatio();
    const std::string &color = json::GetString(config_, "fill");
    const std::string &stroke = json::GetString(config_, "stroke");
    const float lineWidth = json::GetNumber(config_, "lineWidth");
    
    position.x = position.x + offsetX * context.GetDevicePixelRatio();
    position.y = position.y + offsetY * context.GetDevicePixelRatio();
    
    auto circle = xg::make_unique<shape::Circle>(position, size, color, stroke, lineWidth);
    bbox_ = circle->GetBBox(context);
    container->AddElement(std::move(circle));
}
