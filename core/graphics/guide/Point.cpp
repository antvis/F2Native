//
// Created by luna on 2023/8/14.
//

#include "Point.h"
#include "graphics/XChart.h"
#include "graphics/global.h"
#include "graphics/shape/Circle.h"

using namespace xg;

void guide::Point::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context,
                          const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];

    util::Point position = this->GetPosition(chart,
                                             json::Get(this->config_, "position"),
                                             xField, yField);
    double size = json::GetNumber(config_, "size", 6.0) * context.GetDevicePixelRatio();
    double radius = size / 2.0;
    nlohmann::json margin = config_["margin"];
    std::array<float, 2> _margin = margin;
    position.x = position.x - _margin[0] * context.GetDevicePixelRatio();
    position.y = position.y - _margin[1] * context.GetDevicePixelRatio();

    std::string strokeColor = json::GetString(config_, "stroke", "");
    std::string fillColor = json::GetString(config_, "fill", "");
    float lineWidth =
            json::GetNumber(config_, "lineWidth", 0) * context.GetDevicePixelRatio();
    float fillOpacity = json::GetNumber(config_, "fillOpacity", 1);
    std::string shape = json::GetString(config_, "shape", "circle");

    if (shape == "rect") {
        util::Point start = util::Point{position.x - radius, position.y - radius};
        auto rect = xg::make_unique<xg::shape::Rect>(start, util::Size{size, size}, fillColor,
                                                     strokeColor, lineWidth);
        rect->SetFillOpacity(fillOpacity);
        bbox_ = rect->GetBBox(context);
        container->AddElement(std::move(rect));
    } else {
        util::Point center = util::Point{position.x, position.y};
        auto circle = std::make_unique<xg::shape::Circle>(center, radius, fillColor, strokeColor,
                                                          lineWidth);
        circle->SetFillOpacity(fillOpacity);
        bbox_ = circle->GetBBox(context);
        container->AddElement(std::move(circle));
    }
}
