#include "Line.h"
#include "graphics/XChart.h"
#include "graphics/shape/Polyline.h"

using namespace xg;

void guide::Line::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];

    util::Point position = this->GetPosition(chart, json::Get(this->config_, "position"), xField, yField);

    std::string orientation = config_["orientation"];
    util::CanvasFillStrokeStyle colorStyle = util::ColorParser(config_["color"]);
    float lineWidth = config_["lineWidth"].get<float>() * context.GetDevicePixelRatio();

    util::Point xAxis = chart.GetCoord().GetXAxis();
    util::Point yAxis = chart.GetCoord().GetYAxis();

    auto lambda = [&](std::vector<util::Point> &points) -> void {
        auto l = xg::make_unique<xg::shape::Polyline>(lineWidth, points, false);

        if(config_.contains("dash")) {
            l->SetDashLine(json::ParseDashArray(config_["dash"], context.GetDevicePixelRatio()));
        }
        l->strokeStyle_ = colorStyle;
        container->AddElement(std::move(l));
    };

    if(orientation == "horizontal") {
        std::vector<util::Point> points;
        points.push_back(util::Point{xAxis.x, position.y});
        points.push_back(util::Point{xAxis.y, position.y});
        lambda(points);
    } else if(orientation == "vertical") {
        std::vector<util::Point> points;
        points.push_back(util::Point{position.x, yAxis.y});
        points.push_back(util::Point{position.x, yAxis.x});
        lambda(points);
    } else if(orientation == "crossed") {
        std::vector<util::Point> hPoints;
        hPoints.push_back(util::Point{xAxis.x, position.y});
        hPoints.push_back(util::Point{xAxis.y, position.y});
        lambda(hPoints);

        std::vector<util::Point> vPoints;
        vPoints.push_back(util::Point{position.x, yAxis.y});
        vPoints.push_back(util::Point{position.x, yAxis.x});
        lambda(vPoints);
    }
}
