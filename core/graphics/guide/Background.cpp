#include "graphics/guide/Background.h"
#include "graphics/XChart.h"
#include "graphics/shape/Polygon.h"

void xg::guide::Background::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];
    
    std::string shape = json::GetString(this->config_, "shape", "rect");
    if (shape == "polygon") {
        std::vector<util::Point> points;
        const auto &pointsJson = json::GetArray(this->config_, "points");
        for_each(pointsJson.begin(), pointsJson.end(), [&](auto &pointJson) -> void {
            util::Point point = this->GetPosition(chart, pointJson, xField, yField);
            points.push_back(point);
        });
        canvas::CanvasFillStrokeStyle colorStyle = util::ColorParser(this->config_, "fixedColor");
        auto p = xg::make_unique<xg::shape::Polygon>(1, points, colorStyle, colorStyle, false);
        xg::util::BBox bbox = p->GetBBox(context);
        canvas::CanvasFillStrokeStyle newColorStyle = util::ColorParser(this->config_, "fixedColor", &bbox);
        p->SetFillStyle(newColorStyle);
        // 此处简易实现，后续需要区分stroke和fill
        p->SetStorkStyle(newColorStyle);
        container->AddElement(std::move(p));
    } else if (shape == "rect") {
        util::Point leftBottom = this->GetPosition(chart, json::Get(this->config_, "leftBottom"), xField, yField);
        util::Point rightTop = this->GetPosition(chart, json::Get(this->config_, "rightTop"), xField, yField);
        
        std::string color = json::Get(this->config_, "color");
        util::Point p = {leftBottom.x, rightTop.y};
        
        auto rect = xg::make_unique<xg::shape::Rect>(p, util::Size{fabs(rightTop.x - leftBottom.x), fabs(rightTop.y - leftBottom.y)});
        float fillOpacity = json::GetNumber(config_, "fillOpacity", 1);
        rect->SetFillOpacity(fillOpacity);
        rect->SetFillColor(color);
        container->AddElement(std::move(rect));
    }
}
