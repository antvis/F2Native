#include "graphics/guide/Text.h"
#include "graphics/XChart.h"
#include "graphics/global.h"
#include "graphics/shape/Text.h"
#include <utils/common.h>

using namespace xg;

void guide::Text::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];

    util::Point position = this->GetPosition(chart, json::Get(this->config_, "position"), xField, yField);

    std::string textColor = json::GetString(config_, "textColor");
    std::string content = json::GetString(config_, "content");
    std::string textAlign = json::GetString(config_, "textAlign");
    std::string textBaseLine = json::GetString(config_, "textBaseline");

    float textSize = config_["textSize"];
    textSize *= context.GetDevicePixelRatio();

    nlohmann::json margin = config_["margin"];

    std::array<float, 2> _margin = margin;
    position.x = position.x - _margin[0] * context.GetDevicePixelRatio();
    position.y = position.y - _margin[1] * context.GetDevicePixelRatio();

    auto text = xg::make_unique<shape::Text>(content, position, textSize, "", textColor);

    if (config_.contains("font")){
        nlohmann::json font = json::GetObject(config_, "font");
        if (!font.is_null()){
            std::string fontStyle = json::GetString(font, "fontStyle", "");
            std::string fontVariant = json::GetString(font, "fontVariant", "");
            std::string fontWeight = json::GetString(font, "fontWeight", "");
            std::string fontFamily = json::GetString(font, "fontFamily", "");
            text->SetTextFont(fontStyle, fontVariant, fontWeight, fontFamily);
        }
    }

    float fillOpacity = json::GetNumber(config_, "fillOpacity", 1);
    text->SetFillOpacity(fillOpacity);
    text->SetTextAlign(textAlign);
    text->SetTextBaseline(textBaseLine);
    bbox_ = text->GetBBox(context);
    container->AddElement(std::move(text));
}
