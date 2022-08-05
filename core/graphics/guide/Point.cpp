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

//void guide::from_json(const nlohmann::json &j, PointCfg &c) {
//    if (!j.is_object()) {
//        return;
//    }
//    PointCfg d;
//    c.size = j.value("size", d.size);
//    c.shape = j.value("shape", d.shape);
//    c.fill = j.value("fill", d.fill);
//    c.stroke = j.value("stroke", d.stroke);
//    c.lineWidth = j.value("lineWidth", d.lineWidth);
//    c.top = j.value("top", d.top);
//    c.margin = j.value("margin", d.margin);
//    auto &position = json::GetArray(j, "position");
//    if (position.size() >= 2) {
//        if (position[0].is_number()) {
//            c.position[0] = position[0].dump();
//        } else if(position[0].is_string()) {
//            c.position[0] = position[0];
//        }
//        
//        if (position[1].is_number()) {
//            c.position[1] = position[1].dump();
//        } else if(position[0].is_string()) {
//            c.position[1] = position[1];
//        }
//    }
//}

void guide::Point::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string &yField = yFields[0];

    util::Point position = this->GetPosition(chart, config_.position, xField, yField);
    float offsetX = config_.margin[0];
    float offsetY = config_.margin[1];
    double size = config_.size * context.GetDevicePixelRatio();
    const std::string &color = config_.fill;
    const std::string &stroke = config_.stroke;
    const float lineWidth = config_.lineWidth;
    
    position.x = position.x + offsetX * context.GetDevicePixelRatio();
    position.y = position.y + offsetY * context.GetDevicePixelRatio();
    
    auto circle = xg::make_unique<shape::Circle>(position, size, color, stroke, lineWidth);
    bbox_ = circle->GetBBox(context);
    container->AddElement(std::move(circle));
}
