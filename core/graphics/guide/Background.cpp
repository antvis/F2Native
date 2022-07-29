#include "Background.h"
#include "../XChart.h"

void xg::guide::from_json(const nlohmann::json &j, BackgroundCfg &b) {
    if (!j.is_object()) {
        return;
    }
    BackgroundCfg d;
    b.color = j.value("color", d.color);
    b.top = j.value("top", d.top);
    auto &leftBottom = json::GetArray(j, "leftBottom");
    auto &rightTop = json::GetArray(j, "rightTop");
    if (leftBottom.size() >= 2) {
        if (leftBottom[0].is_number()) {
            b.leftBottom[0] = leftBottom[0].dump();
        } else if(leftBottom[0].is_string()) {
            b.leftBottom[0] = leftBottom[0];
        }
        
        if (leftBottom[1].is_number()) {
            b.leftBottom[1] = leftBottom[1].dump();
        } else if(leftBottom[0].is_string()) {
            b.leftBottom[1] = leftBottom[1];
        }
    }
    
    if (rightTop.size() >= 2) {
        if (rightTop[0].is_number()) {
            b.rightTop[0] = rightTop[0].dump();
        } else if(rightTop[0].is_string()) {
            b.rightTop[0] = rightTop[0];
        }
        
        if (rightTop[1].is_number()) {
            b.rightTop[1] = rightTop[1].dump();
        } else if(rightTop[0].is_string()) {
            b.rightTop[1] = rightTop[1];
        }
    }
}

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
