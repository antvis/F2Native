#include "Background.h"
#include "../XChart.h"

void xg::guide::Background::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    const std::vector<std::string> &yFields = chart.getYScaleFields();
    const std::string &yField = yFields[0];

    util::Point leftBottom = this->GetPosition(chart, config_.leftBottom, xField, yField);
    util::Point rightTop = this->GetPosition(chart, config_.rightTop, xField, yField);

    util::Point p = {leftBottom.x, rightTop.y};

    auto rect = xg::make_unique<xg::shape::Rect>(p, util::Size{fabs(rightTop.x - leftBottom.x), fabs(rightTop.y - leftBottom.y)});
    rect->SetFillColor(config_.color);
    container->AddElement(std::move(rect));
}
