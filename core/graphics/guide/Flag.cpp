#include "Flag.h"
#include "../XChart.h"
#include "../shape/Circle.h"
#include "../shape/Group.h"
#include "../shape/Line.h"
#include "../shape/Rect.h"
#include "../shape/Text.h"
#include "../../utils/common.h"

using namespace xg;
using namespace xg::guide;

void Flag::Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) {
    const std::string &xField = chart.GetXScaleField();
    std::vector<std::string> yFields = chart.getYScaleFields();
    const std::string yField = yFields[0];

    util::Point position = this->GetPosition(chart, json::Get(this->config_, "position"), xField, yField);

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
    context.SetFont(CreateFontStyle(fontSize));
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
    //泳道是从上到下0开始，范围：0～4，超出此范围，取相应的最值
    pointInLaneIndex = std::min(pointInLaneIndex, (size_t) 4);
    pointInLaneIndex = std::max(pointInLaneIndex, (size_t) 0);
    std::vector<std::size_t> laneErgodic;
    for(std::size_t index = 1; index < swammingLaneCount; ++index) {
        std::size_t _index =
            static_cast<std::size_t>((pointInLaneIndex + (vertical ? 1 : -1) * index + swammingLaneCount) % swammingLaneCount);
        laneErgodic.push_back(_index);
        //        chart.GetLogTracer()->trace("Flag#PreDrawFlagContent ergodic: %lu", _index);
    }

    //水平防碰撞padding，避免水平的两个flag靠的太近
    int paddingX = 2;
    for(std::size_t index = 0; index < laneErgodic.size(); ++index) {
        std::size_t laneIndex = laneErgodic[index];
        util::Rect &laneRect = swammingLane[laneIndex];

        double rectX = horizon ? position.x : position.x - padding[0] - padding[2] - labelWidth;

        //flag起始绘制点
        double rectY = laneRect.y + (laneRect.height - labelHeight - padding[1] - padding[3]) / 2;

        //碰撞rect加上padding，然后去与真实的flag判断碰撞
        util::Rect dangerRect = {rectX - paddingX, rectY, padding[0] + padding[2] + labelWidth + paddingX, padding[1] + padding[3] + labelHeight};
        
        //真实的flag绘制大小
        contentRect_ =  {rectX, rectY, padding[0] + padding[2] + labelWidth, padding[1] + padding[3] + labelHeight};
        
        bool coincide = false;
        for(std::size_t j = 0; j < dangerRects.size(); ++j) {
            if(collide(dangerRect, dangerRects[j])) {
                coincide = true;
                break;
            }
        }

        if(coincide == false) {
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
    
    if(config_.contains("rounding")) {
        float roundings[4] = {0, 0, 0, 0};
        json::ParseRoundings(config_["rounding"], &roundings[0], context.GetDevicePixelRatio());
        rect->SetRoundings(roundings);
    }

    
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
