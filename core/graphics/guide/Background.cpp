#include "Background.h"
#include "../XChart.h"

void xg::guide::Background::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];

    util::Point leftBottom = this->GetPosition(chart, json::Get(this->config_, "leftBottom"), xField, yField);
    util::Point rightTop = this->GetPosition(chart, json::Get(this->config_, "rightTop"), xField, yField);

    std::string color = json::Get(this->config_, "color");
    util::Point p = {leftBottom.x, rightTop.y};

    auto rect = xg::make_unique<xg::shape::Rect>(p, util::Size{fabs(rightTop.x - leftBottom.x), fabs(rightTop.y - leftBottom.y)});
    rect->SetFillColor(color);
    container->AddElement(std::move(rect));
}
