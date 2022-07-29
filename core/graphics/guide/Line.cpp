#include "Line.h"
#include "../XChart.h"
#include "../shape/Polyline.h"

using namespace xg;

void guide::from_json(const nlohmann::json &j, LineCfg &c) {
    if (!j.is_object()) {
        return;
    }
    LineCfg d;
    c.color = j.value("color", d.color);
    c.lineWidth = j.value("lineWidth", d.lineWidth);
    c.orientation = j.value("orientation", d.orientation);
    c.dash = j.value("dash", d.dash);
    c.top = j.value("top", d.top);
    auto &position = json::GetArray(j, "position");
    if (position.size() >= 2) {
        if (position[0].is_number()) {
            c.position[0] = position[0].dump();
        } else if(position[0].is_string()) {
            c.position[0] = position[0];
        }
        
        if (position[1].is_number()) {
            c.position[1] = position[1].dump();
        } else if(position[0].is_string()) {
            c.position[1] = position[1];
        }
    }
}

void guide::Line::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    const std::vector<std::string> &yFields = chart.getYScaleFields();
    const std::string &yField = yFields[0];

    util::Point position = this->GetPosition(chart, config_.position, xField, yField);
    float lineWidth = config_.lineWidth * context.GetDevicePixelRatio();
    auto lambda = [&](const std::vector<util::Point> &points) -> void {
        auto l = xg::make_unique<xg::shape::Polyline>(lineWidth, points, false);
        auto dash = json::ScaleDash(config_.dash, context.GetDevicePixelRatio());
        l->SetDashLine(dash);
        l->SetStorkColor(config_.color);
        container->AddElement(std::move(l));
    };

    util::Point xAxis = chart.GetCoord().GetXAxis();
    util::Point yAxis = chart.GetCoord().GetYAxis();
    if(config_.orientation == "horizontal") {
        lambda(std::vector<util::Point>{util::Point{xAxis.x, position.y}, util::Point{xAxis.y, position.y}});
    } else if(config_.orientation == "vertical") {
        lambda(std::vector<util::Point> {util::Point{position.x, yAxis.y}, util::Point{position.x, yAxis.x}});
    } else if(config_.orientation == "crossed") {
        lambda(std::vector<util::Point> {util::Point{xAxis.x, position.y}, util::Point{xAxis.y, position.y}});
        lambda(std::vector<util::Point> {util::Point{position.x, yAxis.y}, util::Point{position.x, yAxis.x}});
    }
}
