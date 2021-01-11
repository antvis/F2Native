#include "graphics/guide/Flag.h"
#include "graphics/XChart.h"
#include "graphics/shape/Circle.h"
#include "graphics/shape/Group.h"
#include "graphics/shape/Line.h"
#include "graphics/shape/Rect.h"
#include "graphics/shape/Text.h"
#include <utils/common.h>

using namespace xg;
using namespace xg::guide;

void Flag::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];

    util::Point position = this->GetPosition(chart, xField, yField);

    const float devicePixelRatio = context.GetDevicePixelRatio();
    float paddingLeft = 0.f, paddingRight = 0.f, paddingTop = 0.f, paddingBottom = 0.f;
    const nlohmann::json &paddingCfg = config_["padding"];
    if(paddingCfg.is_number()) {
        paddingLeft = paddingRight = paddingTop = paddingBottom = config_["padding"].get<float>() * devicePixelRatio;
    } else if(paddingCfg.is_array() && paddingCfg.size() == 4) {
        paddingLeft = paddingCfg[0].get<float>() * devicePixelRatio;
        paddingTop = paddingCfg[1].get<float>() * devicePixelRatio;
        paddingRight = paddingCfg[2].get<float>() * devicePixelRatio;
        paddingBottom = paddingCfg[3].get<float>() * devicePixelRatio;
    }

    const float padding[] = {paddingLeft, paddingTop, paddingRight, paddingBottom};

    PreDrawFlagContent(chart, container, context, position, padding, dangerRects);
    DrawFlagCircleAndLine(chart, container, context, position);
    DrawFragContent(chart, container, context, position, padding);

    this->bbox_ = {static_cast<float>(contentRect_.x),     static_cast<float>(contentRect_.x + contentRect_.width),
                   static_cast<float>(contentRect_.y),     static_cast<float>(contentRect_.y + contentRect_.height),
                   static_cast<float>(contentRect_.width), static_cast<float>(contentRect_.height),
                   static_cast<float>(contentRect_.x),     static_cast<float>(contentRect_.y)};
}

void Flag::PreDrawFlagContent(XChart &chart,
                              shape::Group *container,
                              canvas::CanvasContext &context,
                              const util::Point &position,
                              const float padding[],
                              const std::vector<util::Rect> &dangerRects) {
    const util::Point coordCenter = chart.GetCoord().GetCenter();

    bool horizon = position.x < coordCenter.x ? true : false;  // true 点在左边，旗子向右飘
    bool vertical = position.y < coordCenter.y ? true : false; // true 点在上面， 旗子向下

    float fontSize = config_["textSize"].get<float>() * context.GetDevicePixelRatio();
    std::string content = config_["content"];

    float labelWidth = context.MeasureTextWidth(content);
    const float labelHeight = fontSize + 1.;

    util::Point coordStart{chart.GetCoord().GetXAxis().x, chart.GetCoord().GetYAxis().y};

    //泳道计算
    const std::size_t swammingLaneCount = 5;
    double laneHeight = chart.GetCoord().GetHeight() / swammingLaneCount;

    //    chart.GetLogTracer()->trace("Flag#PreDrawFlagContent coordHeight: %lf, laneHeight: %lu", chart.GetCoord().GetHeight(), laneHeight);

    std::vector<util::Rect> swammingLane;
    for(std::size_t index = 0; index < swammingLaneCount; ++index) {
        swammingLane.push_back({coordStart.x, coordStart.y + laneHeight * index, chart.GetCoord().GetWidth(), laneHeight});
    }

    //分配泳道
    std::size_t pointInLaneIndex = static_cast<std::size_t>(floor(position.y - coordStart.y) / laneHeight);
    std::vector<std::size_t> laneErgodic;
    for(std::size_t index = 1; index < swammingLaneCount; ++index) {
        std::size_t _index =
            static_cast<std::size_t>((pointInLaneIndex + (vertical ? 1 : -1) * index + swammingLaneCount) % swammingLaneCount);
        laneErgodic.push_back(_index);
        //        chart.GetLogTracer()->trace("Flag#PreDrawFlagContent ergodic: %lu", _index);
    }

    for(std::size_t index = 0; index < laneErgodic.size(); ++index) {
        std::size_t laneIndex = laneErgodic[index];
        util::Rect &laneRect = swammingLane[laneIndex];

        double rectX = horizon ? position.x : position.x - padding[0] - padding[2] - labelWidth;
        double rectY = laneRect.y + laneRect.height / 2 - labelHeight - padding[1] - padding[3];

        this->contentRect_ = {rectX, rectY, padding[0] + padding[2] + labelWidth, padding[1] + padding[3] + labelHeight};

        bool coincide = false;
        for(std::size_t j = 0; j < dangerRects.size(); ++j) {
            if(collide(contentRect_, dangerRects[j])) {
                coincide = true;
                break;
            }
        }

        if(coincide == false) {
            contentRect_.y = laneRect.y + laneRect.height / 2 - contentRect_.height / 2;
            break;
        }
    }
}

void Flag::DrawFlagCircleAndLine(XChart &chart, shape::Group *container, canvas::CanvasContext &context, util::Point &position) {

    const float radius = config_["radius"].get<float>() * context.GetDevicePixelRatio();
    std::string color = config_["color"];

    auto circle = xg::make_unique<shape::Circle>(position, radius, color);
    circle->SetZIndex(-10);
    container->AddElement(std::move(circle));

    util::Point endPoint{position.x, contentRect_.y};
    float lineWidth = config_["lineWidth"].get<float>() * context.GetDevicePixelRatio();

    auto line = xg::make_unique<shape::Line>(position, endPoint, lineWidth, color);
    line->SetZIndex(-10);
    container->AddElement(std::move(line));

    //    chart.GetLogTracer()->trace("Flag#DrawFlagCircleAndLine lineHeight: %lf", endPoint.y - position.y);
}

void Flag::DrawFragContent(XChart &chart, shape::Group *container, canvas::CanvasContext &context, util::Point &position, const float padding[]) {

    std::string backgroundColor = config_["backgroundColor"];
    std::string color = config_["color"];
    float lineWidth = config_["lineWidth"].get<float>() * context.GetDevicePixelRatio();

    auto rect = xg::make_unique<shape::Rect>(util::Point{contentRect_.x, contentRect_.y},
                                             util::Size(contentRect_.width, contentRect_.height), backgroundColor, color, lineWidth);
    rect->SetZIndex(-5);
    container->AddElement(std::move(rect));

    std::string content = config_["content"];
    float fontSize = config_["textSize"].get<float>() * context.GetDevicePixelRatio();
    std::string textAlign = config_["textAlign"];
    std::string textBaseline = config_["textBaseline"];
    std::string textColor = config_["textColor"];

    auto text = xg::make_unique<shape::Text>(content, util::Point(0, 0), fontSize, "", textColor);

    text->SetPoint(util::Point(contentRect_.x + padding[0], contentRect_.y + contentRect_.height - padding[3] - 1));
    text->SetTextAlign(textAlign);
    text->SetTextBaseline(textBaseline);
    container->AddElement(std::move(text));
}
