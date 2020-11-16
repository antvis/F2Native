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

    util::Point coordCenter = chart.GetCoord().GetCenter();
    bool horizon = true;
    if(position.x <= coordCenter.x) {
        // 在左边， 旗子向右飘
        horizon = true;
    } else {
        // 在右边， 旗子向左飘
        horizon = false;
    }

    bool vertical = true;
    if(position.y <= coordCenter.y) {
        // 在上面，旗子向下飘
        vertical = true;
    } else {
        // 在下面，旗子向上飘
        vertical = false;
    }

    float devicePixelRatio = context.GetDevicePixelRatio();

    std::string color = config_["color"];
    std::string textColor = config_["textColor"];
    float fontSize = config_["textSize"].get<float>() * devicePixelRatio;
    const float radius = 2 * devicePixelRatio;
    std::string content = config_["content"];

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

    float lineWidth = config_["lineWidth"].get<float>() * devicePixelRatio;
    // lineWidth *= chart.GetRatio();

    std::string backgroundColor = config_["backgroundColor"];

    auto circle = xg::make_unique<shape::Circle>(position, radius, color);
    circle->SetZIndex(-10);
    container->AddElement(std::move(circle));

    auto text = xg::make_unique<shape::Text>(content, util::Point(0, 0), fontSize, "", textColor);

    float textWidth = text->GetTextWidth(context);                   // context.MeasureTextWidth(content);
    float textHeight = text->GetTextHeight() + 1 * chart.GetRatio(); // fontSize + 1 * chart.GetRatio();

    float rectHeight = textHeight + paddingTop + paddingBottom;
    float rectWidth = textWidth + paddingLeft + paddingRight;

    float lineHeight = GetLineHeight(chart, container, position, rectWidth, rectHeight, horizon, vertical, devicePixelRatio,
                                     coordCenter, dangerRects);

    util::Point endPoint = util::Point(position.x, vertical ? position.y + lineHeight : position.y - lineHeight);

    // 不能超出合法绘制区
    endPoint.y = fmax(rectHeight, fmin(endPoint.y, chart.GetCoord().GetCenter().y * 2));

    auto line = xg::make_unique<shape::Line>(position, endPoint, lineWidth, color);
    line->SetZIndex(-10);
    container->AddElement(std::move(line));

    auto rect = xg::make_unique<shape::Rect>(util::Point(horizon ? endPoint.x : endPoint.x - rectWidth, endPoint.y - rectHeight),
                                             util::Size(rectWidth, rectHeight), backgroundColor, color, lineWidth);
    rect->SetZIndex(-5);

    text->SetPoint(util::Point(rect->point_.x + rectWidth / 2, rect->point_.y + rectHeight - paddingBottom));
    text->SetTextAlign("center");
    //#ifdef ANDROID
    //    text->pt_.y += 1 * chart.GetRatio();
    //#endif

    container->AddElement(std::move(rect));
    container->AddElement(std::move(text));

    if(horizon) {
        bbox_.minX = endPoint.x;
        bbox_.maxX = endPoint.x + rectWidth;
    } else {
        bbox_.minX = endPoint.x - rectWidth;
        bbox_.maxX = endPoint.x;
    }
    bbox_.minY = endPoint.y - rectHeight;
    bbox_.maxY = endPoint.y;

    bbox_.x = bbox_.minX;
    bbox_.y = bbox_.minY;
    bbox_.width = bbox_.maxX - bbox_.minX;
    bbox_.height = bbox_.maxY - bbox_.minY;

    //    auto circle2 = xg::make_unique<shape::Circle>(util::Point(bbox_.x, bbox_.y), radius, "#000");
    //    container->AddElement(std::move(circle2));
    //
    //    auto rect2 = xg::make_unique<shape::Rect>(util::Point(bbox_.x, bbox_.y), util::Size(bbox_.width, bbox_.height),
    //                                              std::vector<float>(), "#000", 1.0); container->AddElement(std::move(rect2));
}

float Flag::GetLineHeight(XChart &chart,
                          shape::Group *container,
                          util::Point &position,
                          float width,
                          float height,
                          bool horizon,
                          bool vertical,
                          float ratio,
                          util::Point &coordCenter,
                          const std::vector<util::Rect> &dangerRects) {
    //    util::Rect
    int lineHeight = 30 * ratio;

    if(dangerRects.empty()) {
        return lineHeight; // + height - 5 * ratio;
    }

    util::Rect target(horizon ? position.x : position.x - width, vertical ? position.y + lineHeight : position.y - lineHeight, width, height);
    target.y -= height;
    //            auto rect = xg::make_unique<shape::Rect>(
    //                    util::Point(target.x, target.y),
    //                    util::Size(target.width, target.height),
    //                    std::vector<float>(), "#DC143C");

    long index = dangerRects.size() - 1;

    while(index >= 0) {
        const util::Rect &dangerRect = dangerRects.at(index);

        while(fabs(target.x - dangerRect.x) <= dangerRect.width && fabs(target.y - dangerRect.y) <= dangerRect.height) {
            // 有碰撞
            if(vertical) {
                if(target.y + target.height > dangerRect.y && position.y < target.y) {
                    double y = target.y - fabs(target.y + target.height - dangerRect.y) - 5;
                    if(y <= position.y) {
                        // 跑点上面去了
                        break;
                    }
                    target.y = y;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        index--;
    }

    index = dangerRects.size() - 1;
    while(index >= 0) {
        const util::Rect &dangerRect = dangerRects.at(index);

        while(fabs(target.x - dangerRect.x) <= dangerRect.width && fabs(target.y - dangerRect.y) <= dangerRect.height) {
            // 有碰撞
            if(vertical) {
                // 往下, y 增加
                target.y += dangerRect.height;
            } else {
                target.y -= dangerRect.height;
            }
            // chart.GetLogTracer()->trace("GetLineHeight", " 碰撞：target x:%lf, y: %lf vertical:%s ", position.x, position.y,
            //                             vertical ? "true" : "false");

            // 有碰撞， index 重来
            index = dangerRects.size();
        }
        index--;
    }

    //            rect->point_ = util::Point(target.x, target.y);
    //            container->AddElement(std::move(rect));

    if(vertical) {
        // chart.GetLogTracer()->trace("GetLineHeight", " result height:  %lf", fabs(target.y - position.y) + height);
        return fabs(target.y - position.y) + height;
    } else {
        // chart.GetLogTracer()->trace("GetLineHeight", " result height:  %lf", fabs(target.y - position.y) - height);
        return fabs(target.y - position.y) - height;
    }
}
