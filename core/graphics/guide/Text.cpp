#include "Text.h"
#include "../XChart.h"
#include "../global.h"
#include "../shape/Text.h"
#include "../../utils/common.h"

using namespace xg;

void guide::from_json(const nlohmann::json &j, TextCfg &c) {
    if (!j.is_object()) {
        return;
    }
    TextCfg d;
    c.content = j.value("content", d.content);
    c.textColor = j.value("textColor", d.textColor);
    c.textAlign = j.value("textAlign", d.textAlign);
    c.textBaseline = j.value("textBaseline", d.textBaseline);
    c.textSize = j.value("textSize", d.textSize);
    c.margin = j.value("margin", d.margin);
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

void guide::Text::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];

    util::Point position = this->GetPosition(chart, config_.position, xField, yField);

    float textSize = config_.textSize * context.GetDevicePixelRatio();


    position.x = position.x - config_.margin[0] * context.GetDevicePixelRatio();
    position.y = position.y - config_.margin[1] * context.GetDevicePixelRatio();

    auto text = xg::make_unique<shape::Text>(config_.content, position, textSize, "", config_.textColor);
    text->SetTextAlign(config_.textAlign);
    text->SetTextBaseline(config_.textBaseline);
    bbox_ = text->GetBBox(context);
    container->AddElement(std::move(text));
}
