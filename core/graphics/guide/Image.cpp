//
//  Image.cpp
//  F2
//
//  Created by weiqing.twq on 2021/9/1.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#include "Image.h"
#include "graphics/XChart.h"
#include "graphics/global.h"
#include "graphics/shape/Image.h"

using namespace xg;

void guide::Image::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string &yField = yFields[0];

    util::Point position = this->GetPosition(chart, json::GetArray(this->config_, "position"), xField, yField);
    const nlohmann::json &margin = config_["margin"];
    std::array<float, 2> _margin = margin;
    
    double width = json::GetNumber(config_, "width", NAN) * context.GetDevicePixelRatio();
    double height = json::GetNumber(config_, "height", NAN) * context.GetDevicePixelRatio();
    
    const std::string &src = json::GetString(config_, "src");
    if (src.empty()) {
        return;
    }

    position.x = position.x - _margin[0] * context.GetDevicePixelRatio();
    position.y = position.y - _margin[1] * context.GetDevicePixelRatio();
    auto image = xg::make_unique<shape::Image>(src, position, util::Size {width, height});
    
    //上屏
    image->OnLoad([&] () {
        func::Command *cmd = func::CreateCommand([this]() -> void {});
        //立刻上屏
        chart.RequestAnimationFrame(cmd, 0);
    });
    container->AddElement(std::move(image));

}
