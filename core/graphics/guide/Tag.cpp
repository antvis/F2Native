//
// Created by luna on 2023/8/14.
//

#include "Tag.h"
#include "graphics/XChart.h"
#include "graphics/shape/Group.h"
#include "graphics/shape/Image.h"
#include "graphics/shape/Polygon.h"
#include "graphics/shape/Rect.h"
#include "graphics/shape/Text.h"
#include <utils/common.h>

using namespace xg;
using namespace xg::guide;

void Tag::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context,
                 const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];

    util::Point position = this->GetPosition(chart, json::Get(this->config_, "position"), xField,
                                             yField);
    nlohmann::json margin = config_["margin"];
    std::array<float, 2> _margin = margin;
    position.x = position.x - _margin[0] * context.GetDevicePixelRatio();
    position.y = position.y - _margin[1] * context.GetDevicePixelRatio();

    const float devicePixelRatio = context.GetDevicePixelRatio();
    float paddingLeft = 0.f, paddingRight = 0.f, paddingTop = 0.f, paddingBottom = 0.f;
    const nlohmann::json &paddingCfg = config_["padding"];
    if (paddingCfg.is_number()) {
        paddingLeft = paddingRight = paddingTop = paddingBottom =
                config_["padding"].get<float>() * devicePixelRatio;
    } else if (paddingCfg.is_array() && paddingCfg.size() == 4) {
        paddingLeft = paddingCfg[0].get<float>() * devicePixelRatio;
        paddingTop = paddingCfg[1].get<float>() * devicePixelRatio;
        paddingRight = paddingCfg[2].get<float>() * devicePixelRatio;
        paddingBottom = paddingCfg[3].get<float>() * devicePixelRatio;
    }
    const float padding[] = {paddingLeft, paddingTop, paddingRight, paddingBottom};
    const nlohmann::json &iconConfig = json::GetObject(config_, "iconConfig");
    if (iconConfig != nullptr && !iconConfig.is_null()) {
        iconConfigInfo_.showIcon = json::GetBool(iconConfig, "showIcon", false);
        iconConfigInfo_.iconUrl = json::GetString(iconConfig, "iconUrl");
        if (iconConfigInfo_.showIcon && !iconConfigInfo_.iconUrl.empty()) {
            iconConfigInfo_.width =
                    json::GetNumber(iconConfig, "width", 12) * context.GetDevicePixelRatio();
            iconConfigInfo_.height =
                    json::GetNumber(config_, "height", 12) * context.GetDevicePixelRatio();
            const auto &margin = json::GetArray(iconConfig, "margin");
            if (margin.is_array() && margin.size() >= 4) {
                std::array<float, 4> _margin = margin;
                // [left、top、right、bottom]
                iconConfigInfo_.margin[0] = _margin[0] * context.GetDevicePixelRatio();
                iconConfigInfo_.margin[1] = _margin[1] * context.GetDevicePixelRatio();
                iconConfigInfo_.margin[2] = _margin[2] * context.GetDevicePixelRatio();
                iconConfigInfo_.margin[3] = _margin[3] * context.GetDevicePixelRatio();
            }
            iconConfigInfo_.containerWidth = iconConfigInfo_.width +
                                             iconConfigInfo_.margin[0] + iconConfigInfo_.margin[2];
            iconConfigInfo_.containerHeight = iconConfigInfo_.height +
                                              iconConfigInfo_.margin[1] + iconConfigInfo_.margin[3];
        } else {
            iconConfigInfo_.showIcon = false;
            iconConfigInfo_.iconUrl = "";
        }
    }
    fontSize_ = config_["textSize"].get<float>() * context.GetDevicePixelRatio();
    context.SetFont(CreateFontStyle(fontSize_));
    content_ = json::GetString(config_, "content", "");
    float labelWidth = context.MeasureTextWidth(content_);
    labelHeight_ = fontSize_;

    const nlohmann::json &subConfig = json::GetObject(config_, "subConfig");
    float subLabelWidth = 0 ;

    if (subConfig != nullptr && !subConfig.is_null()) {
        subConfigInfo_.content = json::GetString(subConfig, "content", "");
        if (!subConfigInfo_.content.empty()) {
            subConfigInfo_.textSize = json::GetNumber(subConfig, "textSize", 0) * context.GetDevicePixelRatio();
            if (subConfigInfo_.textSize == 0) {
                subConfigInfo_.textSize = fontSize_;
            }
            subConfigInfo_.textAlign = json::GetString(subConfig, "textAlign", "");
            subConfigInfo_.textColor = json::GetString(subConfig, "textColor", "");
            subConfigInfo_.marginTop = json::GetNumber(subConfig, "marginTop", 0) * context.GetDevicePixelRatio();
            context.SetFont(CreateFontStyle(subConfigInfo_.textSize));
            subLabelWidth = context.MeasureTextWidth(subConfigInfo_.content);
            subLabelHeight_ =  context.MeasureTextHeight(subConfigInfo_.content);;
        }
    }

    // 真实的Tag绘制大小
    contentRect_ = {position.x, position.y,
                    padding[0] + padding[2] + fmax(labelWidth , subLabelWidth) + iconConfigInfo_.containerWidth,
                    padding[1] + padding[3] + fmax(labelHeight_+ subConfigInfo_.marginTop + subLabelHeight_, iconConfigInfo_.containerHeight)};

    float sideSize = json::GetNumber(config_, "sideSize", 3) * context.GetDevicePixelRatio();
    // 理财tba引领版用了side字段，有bug，没有*density，后面不会推荐使用
    if (config_.contains("side")) {
        sideSize = json::GetNumber(config_, "side", 6);
    }

    std::string direct = GetDirect(chart, context, position, dangerRects, sideSize);
    fillOpacity_ = json::GetNumber(config_, "fillOpacity", 1);
    strokeColor_ = json::GetString(config_, "stroke", "");
    fillColor_ = json::GetString(config_, "fill", "");
    lineWidth_ = config_["lineWidth"].get<float>() * context.GetDevicePixelRatio();


    DrawTagPolygon(chart, container, context, position, direct, sideSize);
    DrawTagContent(chart, container, context, position, padding, direct);


    this->bbox_ = {static_cast<float>(contentRect_.x),
                   static_cast<float>(contentRect_.x + contentRect_.width),
                   static_cast<float>(contentRect_.y),
                   static_cast<float>(contentRect_.y + contentRect_.height),
                   static_cast<float>(contentRect_.width), static_cast<float>(contentRect_.height),
                   static_cast<float>(contentRect_.x), static_cast<float>(contentRect_.y)};
}


void
Tag::DrawTagPolygon(XChart &chart, shape::Group *container, canvas::CanvasContext &context,
                    util::Point &position, const std::string direct, const float sideSize) {

    std::vector<util::Point> arrowPoints = GetArrowPoints(chart, context, position,
                                                          direct, sideSize);
    auto polygon = xg::make_unique<xg::shape::Polygon>(lineWidth_, arrowPoints, strokeColor_,
                                                       fillColor_, false);

    polygon->SetZIndex(-10);
    polygon->SetFillOpacity(fillOpacity_);
    container->AddElement(std::move(polygon));
}

void Tag::DrawTagContent(XChart &chart, shape::Group *container, canvas::CanvasContext &context,
                         util::Point &position, const float padding[], const std::string direct) {
    chart.GetLogTracer()->trace("Tag#DrawTagContent direct after: %s", direct.c_str());


    auto rect = xg::make_unique<shape::Rect>(util::Point{posX_, posY_},
                                             util::Size(contentRect_.width, contentRect_.height),
                                             fillColor_, strokeColor_, lineWidth_);
    rect->SetFillOpacity(fillOpacity_);
    rect->SetZIndex(-5);
    //[bl,br,tl,tr]
    if (config_.contains("radius")) {
        float radius = json::GetNumber(config_, "radius", 0) * context.GetDevicePixelRatio();
        float roundings[4] = {radius, radius, radius, radius};

        if (direct == "tl") {
            roundings[1] = 0;
        } else if (direct == "tr") {
            roundings[0] = 0;
        } else if (direct == "bl") {
            roundings[3] = 0;
        } else if (direct == "br") {
            roundings[2] = 0;
        }
        rect->SetRoundings(roundings);
    }

    container->AddElement(std::move(rect));


    if (iconConfigInfo_.showIcon && !iconConfigInfo_.iconUrl.empty()) {
        util::Point center = util::Point(
                posX_ + padding[0] + iconConfigInfo_.margin[0],
                posY_ + padding[1] + (contentRect_.height - padding[1] - padding[3]) / 2 -
                iconConfigInfo_.containerHeight / 2 + iconConfigInfo_.margin[1]);
        auto image = xg::make_unique<shape::Image>(iconConfigInfo_.iconUrl, center,
                                                   util::Size{iconConfigInfo_.width,
                                                              iconConfigInfo_.height});
        if (!subConfigInfo_.content.empty()) {
            image->SetPoint(util::Point(posX_ + padding[0] + iconConfigInfo_.margin[0],
                                        posY_ + padding[1] +
                                            (contentRect_.height - padding[1] - padding[3] - iconConfigInfo_.margin[1] - iconConfigInfo_.margin[3]) / 2 -
                                            iconConfigInfo_.height / 2));
        }

        // 上屏
        image->OnLoad([&]() {
            func::Command *cmd = func::CreateCommand([this]() -> void {});
            // 立刻上屏
            chart.RequestAnimationFrame(cmd, 0);
        });
        container->AddElement(std::move(image));
    }

    std::string textAlign = config_["textAlign"];
    std::string textBaseline = config_["textBaseline"];
    std::string textColor = config_["textColor"];
    // 整体高度-上下padding-标题高度-副标题高度-副标题margin = 间距。只有图片高度比两行标题高度大的时候才优值
    float diff = contentRect_.height -  padding[1] - padding[3] - labelHeight_ - subLabelHeight_ - subConfigInfo_.marginTop;
    auto text = xg::make_unique<shape::Text>(content_, util::Point(0, 0), fontSize_, "", textColor);
    // 算的是text left、bottom的坐标
    text->SetPoint(util::Point(
            posX_ + padding[0] + iconConfigInfo_.containerWidth,
            posY_ + padding[1] + diff/2 + labelHeight_ ));

    if (config_.contains("font")) {
        nlohmann::json font = json::GetObject(config_, "font");
        if (!font.is_null()) {
            std::string fontStyle = json::GetString(font, "fontStyle", "");
            std::string fontVariant = json::GetString(font, "fontVariant", "");
            std::string fontWeight = json::GetString(font, "fontWeight", "");
            std::string fontFamily = json::GetString(font, "fontFamily", "");
            text->SetTextFont(fontStyle, fontVariant, fontWeight, fontFamily);
        }
    }

    if (!subConfigInfo_.content.empty()) {
        // 算的是text left、bottom的坐标

        if (subConfigInfo_.textColor.empty()) {
            subConfigInfo_.textColor = textColor;
        }
        if (subConfigInfo_.textBaseline.empty()) {
            subConfigInfo_.textBaseline = textBaseline;
        }
        if (subConfigInfo_.textAlign.empty()) {
            subConfigInfo_.textAlign = textAlign;
        }
        auto subText = xg::make_unique<shape::Text>(subConfigInfo_.content, util::Point(0, 0), subConfigInfo_.textSize, "",
                                                    subConfigInfo_.textColor);
        subText->SetPoint(util::Point(posX_ + padding[0] + iconConfigInfo_.containerWidth,
                                   posY_ + contentRect_.height - padding[3] - diff/2 ));
        subText->SetTextAlign(subConfigInfo_.textAlign);
        subText->SetTextBaseline(subConfigInfo_.textBaseline);
        container->AddElement(std::move(subText));
    }

    text->SetTextAlign(textAlign);
    text->SetTextBaseline(textBaseline);
    container->AddElement(std::move(text));
}

//   'tl' | 'tc' | 'tr'
//   'cl' | 'cc' | 'cr'
//   'bl' | 'bc' | 'br'
// 框体相对于箭头的方向，'tl'代表框体在箭头左上方，'br'代表框体在箭头右下方，'tc'代表框体在箭头正上方，以此类推
std::string Tag::GetDirect(XChart &chart, canvas::CanvasContext &context,
                           const util::Point &position, const std::vector<util::Rect> &dangerRects,
                           const float sideSize) {
    std::string direct = json::GetString(config_, "direct", "tl");
    chart.GetLogTracer()->trace("Tag#GetArrowPoints direct before: %s", direct.c_str());
    std::string newDirect = direct.size() < 2 ? "tl" : direct;

    float x = position.x;
    float y = position.y;

    chart.GetLogTracer()->trace("Tag#GetArrowPoints direct: x %lf ,y %lf", x, y);

    char vertical = newDirect.at(0);
    char horizontal = newDirect.at(1);
    float guideHeight = contentRect_.height;
    float guideWidth = contentRect_.width;
    float canvasWidth = chart.GetWidth();
    float canvasHeight = chart.GetHeight();
    
    bool autoAdjust = json::GetBool(config_, "autoAdjust", true);

    if (autoAdjust) {
        // adjust for vertical direction
        if (vertical == 't' && y - sideSize - guideHeight < 0) {
            vertical = 'b';
        } else if (vertical == 'b' && y + sideSize + guideHeight > canvasHeight) {
            vertical = 't';
        }
        // adjust for horizontal direction
        float diff = vertical == 'c' ? sideSize : 0;
        if (horizontal == 'l' && x - diff - guideWidth < 0) {
            horizontal = 'r';
        } else if (horizontal == 'r' && x + diff + guideWidth > canvasWidth) {
            horizontal = 'l';
        } else if (horizontal == 'c') {
            if (guideWidth / 2 + x + diff > canvasWidth) {
                horizontal = 'l';
            } else if (x - guideWidth / 2 - diff < 0) {
                horizontal = 'r';
            }
        }
        newDirect.clear();
        newDirect.push_back(vertical);
        newDirect.push_back(horizontal);
    }
    return std::move(newDirect);
}

std::vector<util::Point> Tag::GetArrowPoints(XChart &chart,
                                             canvas::CanvasContext &context,
                                             const util::Point &position,
                                             const std::string direct, const float sideSize) {
    chart.GetLogTracer()->trace("Tag#GetArrowPoints direct: %s", direct.c_str());

    std::vector<util::Point> arrowPoints;
    float guideHeight = contentRect_.height;
    float guideWidth = contentRect_.width;

    posX_ = contentRect_.x;
    posY_ = contentRect_.y;
    chart.GetLogTracer()->trace("Tag#GetArrowPoints direct: w %lf ,h %lf, x %lf ,y %lf",
                                guideWidth, guideHeight, posX_, posY_);
    if (direct == "tl") {
        arrowPoints = {
                {posX_,        posY_ - sideSize - 1},
                {posX_,        posY_},
                {posX_ - sideSize, posY_ - sideSize - 1},
        };
        posX_ -= guideWidth;
        posY_ = posY_ - guideHeight - sideSize;
    } else if (direct == "cl") {
        arrowPoints = {
                {posX_ - sideSize - 1, posY_ - sideSize},
                {posX_ - sideSize - 1, posY_ + sideSize},
                {posX_,            posY_},
        };
        posX_ = posX_ - guideWidth - sideSize;
        posY_ -= guideHeight / 2;
    } else if (direct == "bl") {
        arrowPoints = {
                {posX_,        posY_},
                {posX_,        posY_ + sideSize + 1},
                {posX_ - sideSize, posY_ + sideSize + 1},
        };
        posX_ = posX_ - guideWidth;
        posY_ += sideSize;
    } else if (direct == "bc") { // 有问题
        arrowPoints = {
                {posX_,        posY_},
                {posX_ - sideSize, posY_ + sideSize + 1},
                {posX_ + sideSize, posY_ + sideSize + 1},
        };
        posX_ = posX_ - guideWidth / 2;
        posY_ = posY_ + sideSize;
    } else if (direct == "br") {
        arrowPoints = {
                {posX_,        posY_},
                {posX_,        posY_ + sideSize + 1},
                {posX_ + sideSize, posY_ + sideSize + 1},
        };
        posY_ += sideSize;
    } else if (direct == "cr") {
        arrowPoints = {
                {posX_,        posY_},
                {posX_ + sideSize, posY_ - sideSize},
                {posX_ + sideSize, posY_ + sideSize},
        };
        posX_ += sideSize;
        posY_ -= guideHeight / 2;
    } else if (direct == "tr") {
        arrowPoints = {
                {posX_,        posY_},
                {posX_,        posY_ - sideSize - 1},
                {posX_ + sideSize, posY_ - sideSize - 1},
        };
        posY_ = posY_ - guideHeight - sideSize;
    } else if (direct == "tc") {
        arrowPoints = {
                {posX_,        posY_},
                {posX_ - sideSize, posY_ - sideSize - 1},
                {posX_ + sideSize, posY_ - sideSize - 1},
        };
        posX_ -= guideWidth / 2;
        posY_ = posY_ - guideHeight - sideSize;
    }
    return arrowPoints;
}
