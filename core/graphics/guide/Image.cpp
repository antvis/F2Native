//
//  Image.cpp
//  F2
//
//  Created by weiqing.twq on 2021/9/1.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#include "Image.h"
#include "../XChart.h"
#include "../global.h"
#include "../shape/Image.h"

using namespace xg;

void guide::from_json(const nlohmann::json &j, ImageCfg &i) {
    if (!j.is_object()) {
        return;
    }
    ImageCfg d;
    i.margin = j.value("margin", d.margin);
    i.width = j.value("width", d.width);
    i.height = j.value("height", d.height);
    i.src = j.value("src", d.src);
    i.top = j.value("top", d.top);
    auto &position = json::GetArray(j, "position");
    if (position.size() >= 2) {
        if (position[0].is_number()) {
            i.position[0] = position[0].dump();
        } else if(position[0].is_string()) {
            i.position[0] = position[0];
        }
        
        if (position[1].is_number()) {
            i.position[1] = position[1].dump();
        } else if(position[0].is_string()) {
            i.position[1] = position[1];
        }
    }
}

void guide::Image::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    if (config_.src.empty()) {
        return;
    }
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string &yField = yFields[0];

    util::Point position = this->GetPosition(chart, config_.position, xField, yField);
    
    double width = config_.width * context.GetDevicePixelRatio();
    double height = config_.height * context.GetDevicePixelRatio();
    
    position.x = position.x - config_.margin[0] * context.GetDevicePixelRatio();
    position.y = position.y - config_.margin[1] * context.GetDevicePixelRatio();
    auto image = xg::make_unique<shape::Image>(config_.src, position, util::Size {width, height});
    
    //上屏
    image->OnLoad([&] () {
        func::Command *cmd = func::CreateCommand([this]() -> void {});
        //立刻上屏
        chart.RequestAnimationFrame(cmd, 0);
    });
    container->AddElement(std::move(image));

}
