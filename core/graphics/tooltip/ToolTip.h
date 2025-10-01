#ifndef XG_GRAPHICS_TOOLTIP_H
#define XG_GRAPHICS_TOOLTIP_H

#include "../canvas/CanvasContext.h"
#include "../canvas/Coord.h"
#include "../global.h"
#include "../shape/Group.h"
#include "../shape/Line.h"
#include "../shape/Polyline.h"
#include "../shape/Rect.h"
#include "../shape/Text.h"
#include "../shape/Circle.h"
#include "../util/Point.h"
#include "../util/json.h"
#include "../../nlohmann/json.hpp"

namespace xg {
namespace tooltip {
class ToolTip {
  public:
    ToolTip(shape::Group *container, nlohmann::json cfg) : container_(container), config_(cfg) {}
    ~ToolTip() { container_ = nullptr; }

    void SetPosition(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, util::Point point, const nlohmann::json &tooltipItems, const std::string &position) {
        this->_ShowCrosshairs(coord, canvasContext, point, true);
        this->_ShowSnap(coord, canvasContext, point, tooltipItems);
        this->SetYTipContent(coord, canvasContext, tooltipItems[0], point, position);
    }

    float GetXTipRectOffsetX(const std::string &textAlign, float fontWidth) {
        if(textAlign == "start") {
            return fontWidth / 2;
        } else if(textAlign == "end") {
            return -fontWidth / 2;
        } else {
            return 0;
        }
    }

    float GetXTipRectOffsetY(const std::string &textBaseline, float fontSize) {
        if(textBaseline == "top") {
            return -fontSize;
        } else if(textBaseline == "middle") {
            return -fontSize / 2;
        } else {
            return 0;
        }
    }

    void SetXTipContent(canvas::CanvasContext &canvasContext,
                        const nlohmann::json &tooltipItems, \
                        const util::Point &point,
                        const util::Point &xAxis,
                        const std::string &position) {
        if(!config_["xTip"].is_object() || (config_.contains("showXTip") && json::GetBool(config_, "showXTip", true) == false)) {
            return;
        }
        const std::string &content = tooltipItems["title"];
        nlohmann::json &backCfg = config_["background"];
        const std::string &backColor = backCfg["fill"];
        nlohmann::json &padding = backCfg["padding"];
//        double radius = backCfg["radius"].get<double>() * canvasContext.GetDevicePixelRatio();
        double paddingH = padding[0].get<double>() * canvasContext.GetDevicePixelRatio();
        double paddingV = padding[1].get<double>() * canvasContext.GetDevicePixelRatio();

        const nlohmann::json &xTipCfg = tooltipItems["xTip"];
        const std::string &color = xTipCfg["fill"];
        const std::string &textAlign = xTipCfg["textAlign"];
        const std::string &textBaseline = xTipCfg["textBaseline"];
        double fontSize = xTipCfg["fontSize"];
        bool inner = xTipCfg["inner"];
        fontSize *= canvasContext.GetDevicePixelRatio();
        // 先创建一个text对象
        std::unique_ptr<shape::Text> xTip = std::make_unique<shape::Text>(content, util::Point{0, 0}, fontSize, color, color);
        float width = xTip->GetTextWidth(canvasContext);
        float height =  fmax(xTip->GetTextHeight(),fontSize);

        float rectY = inner ? point.y - height - paddingV * 2 : point.y;
        float textY = inner ? point.y - paddingV: point.y + height + paddingV;
        float rectOffsetX = GetXTipRectOffsetX(textAlign, width);
        float rectOffsetY = GetXTipRectOffsetY(textBaseline, fontSize);

        //左右边界限制
        float left = point.x - width / 2 - paddingH + rectOffsetX;
        float right = point.x + width / 2 + paddingH + rectOffsetX;
        float limitX = fmax(left, xAxis.x);
        if (right > xAxis.y) {
            limitX = xAxis.y - width - paddingH * 2;
        }
        
        std::unique_ptr<shape::Rect> backRect =
            std::make_unique<shape::Rect>(util::Point{limitX, rectY + rectOffsetY },
                                          util::Size{width + paddingH * 2, height + paddingV * 2}, backColor);
        if(backCfg.contains("radius")) {
            float radius = json::GetNumber(backCfg, "radius", 0) * canvasContext.GetDevicePixelRatio();
            float roundings[4] = {radius, radius, radius, radius};
            backRect->SetRoundings(roundings);
        }
        container_->AddElement(std::move(backRect));

        limitX +=  width / 2 + paddingH - rectOffsetX;
        xTip->SetPoint(util::Point{limitX, textY});
        xTip->SetTextAlign(textAlign);
        xTip->SetTextBaseline(textBaseline);
        if (xTipCfg.contains("font")){
            nlohmann::json font = json::GetObject(xTipCfg, "font");
            if (!font.is_null()){
                std::string fontStyle = json::GetString(font, "fontStyle", "");
                std::string fontVariant = json::GetString(font, "fontVariant", "");
                std::string fontWeight = json::GetString(font, "fontWeight", "");
                std::string fontFamily = json::GetString(font, "fontFamily", "");
                xTip->SetTextFont(fontStyle, fontVariant, fontWeight, fontFamily);
            }
        }
        container_->AddElement(std::move(xTip));
    }

    void SetYTipContent(canvas::coord::AbstractCoord &coord,
                        canvas::CanvasContext &canvasContext,
                        const nlohmann::json &tooltipItems,
                        const util::Point &point,
                        const std::string &position) {
        if(!config_["yTip"].is_object() || (config_.contains("showYTip") && json::GetBool(config_, "showYTip", true) == false)) {
            return;
        }
        const std::string &content = tooltipItems["value"];
        nlohmann::json &backCfg = config_["background"];
        const std::string &backColor = backCfg["fill"];
        nlohmann::json &padding = backCfg["padding"];
        double paddingH = padding[0].get<double>() * canvasContext.GetDevicePixelRatio();
        double paddingV = padding[1].get<double>() * canvasContext.GetDevicePixelRatio();

        const nlohmann::json &yTipCfg = tooltipItems["yTip"];
        const std::string &color = yTipCfg["fill"];
        const std::string &textAlign = yTipCfg["textAlign"];
        const std::string &textBaseline = yTipCfg["textBaseline"];
        double fontSize = yTipCfg["fontSize"];
        bool inner = yTipCfg["inner"];
        fontSize *= canvasContext.GetDevicePixelRatio();
        std::unique_ptr<shape::Text> yTip =
                std::make_unique<shape::Text>(content, util::Point{0, 0}, fontSize, color, color);
        float width = yTip->GetTextWidth(canvasContext);
        if (position == "right") {
            float rectX = inner ? coord.GetXAxis().y - width - paddingH * 2 : coord.GetXAxis().y;
            float textX = inner ? coord.GetXAxis().y - width / 2 - paddingH : coord.GetXAxis().y + width / 2 + paddingH;
            float rectOffsetX = GetXTipRectOffsetX(textAlign, width);
            float rectOffsetY = GetXTipRectOffsetY(textBaseline, fontSize);
            
            std::unique_ptr<shape::Rect> backRect =
            std::make_unique<shape::Rect>(util::Point{rectX + rectOffsetX, point.y - fontSize / 2 - paddingV + rectOffsetY},
                                          util::Size{width + paddingH * 2, fontSize + paddingV * 2}, backColor);
            if(backCfg.contains("radius")) {
                float radius = json::GetNumber(backCfg, "radius", 0) * canvasContext.GetDevicePixelRatio();
                float roundings[4] = {radius, radius, radius, radius};
                backRect->SetRoundings(roundings);
            }
            container_->AddElement(std::move(backRect));
            
            yTip->SetPoint(util::Point{textX, point.y + fontSize / 2});
        } else {
            float rectX = inner ? coord.GetXAxis().x : coord.GetXAxis().x - width - paddingH * 2;
            float textX = inner ? coord.GetXAxis().x + width / 2 + paddingH : coord.GetXAxis().x - width / 2 - paddingH;
            float rectOffsetX = GetXTipRectOffsetX(textAlign, width);
            float rectOffsetY = GetXTipRectOffsetY(textBaseline, fontSize);
            
            std::unique_ptr<shape::Rect> backRect =
            std::make_unique<shape::Rect>(util::Point{rectX + rectOffsetX, point.y - fontSize / 2 - paddingV + rectOffsetY},
                                          util::Size{width + paddingH * 2, fontSize + paddingV * 2}, backColor);
            if(backCfg.contains("radius")) {
                float radius = json::GetNumber(backCfg, "radius", 0) * canvasContext.GetDevicePixelRatio();
                float roundings[4] = {radius, radius, radius, radius};
                backRect->SetRoundings(roundings);
            }
            container_->AddElement(std::move(backRect));
            
            yTip->SetPoint(util::Point{textX, point.y + fontSize / 2});
        }
        yTip->SetTextAlign(textAlign);
        yTip->SetTextBaseline(textBaseline);
        if (yTipCfg.contains("font")){
            nlohmann::json font = json::GetObject(yTipCfg, "font");
            if (!font.is_null()){
                std::string fontStyle = json::GetString(font, "fontStyle", "");
                std::string fontVariant = json::GetString(font, "fontVariant", "");
                std::string fontWeight = json::GetString(font, "fontWeight", "");
                std::string fontFamily = json::GetString(font, "fontFamily", "");
                yTip->SetTextFont(fontStyle, fontVariant, fontWeight, fontFamily);
            }
        }
        container_->AddElement(std::move(yTip));
    }

    void Show(canvas::CanvasContext &canvasContext, utils::Tracer *tracer) {
        if(tracer != nullptr) {
            tracer->trace("tooltip#Show container: %lu ", container_->children_.size());
        }
        container_->Show();
        container_->Draw(canvasContext);
    }
    
    void Hide() { container_->Hide(); }
    
private:
    void _ShowSnap(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, util::Point &point, const nlohmann::json &tooltipItems) {
        if (!config_["snap"].is_boolean() || config_["snap"] == false) {
            return;
        }
        
        std::for_each(tooltipItems.begin(), tooltipItems.end(), [&](const nlohmann::json& tooltipItem) -> void {
            util::Point snapPoint;
            snapPoint.x = json::GetNumber(tooltipItem, "x");
            snapPoint.y = json::GetNumber(tooltipItem, "y");
            const nlohmann::json &style = json::GetObject(tooltipItem, "tooltipMarkerStyle", {});
            const std::string fillColor = json::GetString(style, "fill", "#ffffff");
            const std::string strokeColor = json::GetString(style, "stroke", json::GetString(tooltipItem, "color"));
            const double radius = json::GetNumber(style, "radius", 3) * canvasContext.GetDevicePixelRatio();
            const double lineWidth = json::GetNumber(style, "lineWidth", 1) * canvasContext.GetDevicePixelRatio();
            auto circle = std::make_unique<shape::Circle>(snapPoint, radius, fillColor, strokeColor, lineWidth);
            container_->AddElement(std::move(circle));
        });
    }
    void _ShowCrosshairs(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, util::Point &point, bool showY) {
        if (!config_["showCrosshairs"].is_boolean() || config_["showCrosshairs"] == false) {
            return;
        }
        
        const std::string &crosshairsType = json::GetString(config_, "crosshairsType", "xy");
        bool hasX = false;
        bool hasY = false;
        if (crosshairsType == "xy") {
            hasX = true;
            hasY = true;
        } else if (crosshairsType == "x") {
            hasX = true;
        } else if (crosshairsType == "y") {
            hasY = true;
        }
        
        if (!hasX && !hasY) {
            return;
        }
        
        const nlohmann::json &crosshairsStyle = config_["crosshairsStyle"];
        const std::string &lineColor = crosshairsStyle["stroke"];
        double lineWidth = crosshairsStyle["lineWidth"];
        lineWidth *= canvasContext.GetDevicePixelRatio();
        const std::string &type = crosshairsStyle["type"];

        if (hasX) {
            vector<xg::util::Point> xPoints;
            xPoints.push_back(util::Point{point.x, coord.GetYAxis().x});
            xPoints.push_back(util::Point{point.x, coord.GetYAxis().y});
            
            auto xLine = xg::make_unique<shape::Polyline>(lineWidth, xPoints, lineColor, "", false);
            if (type == "dash") {
                xLine->SetDashLine(json::ParseDashArray(crosshairsStyle["dash"], canvasContext.GetDevicePixelRatio()));
            }
            container_->AddElement(std::move(xLine));
        }

        if (hasY && showY) {
            vector<xg::util::Point> yPoints;
            yPoints.push_back(util::Point{coord.GetXAxis().x, point.y});
            yPoints.push_back(util::Point{coord.GetXAxis().y, point.y});

            auto yLine = xg::make_unique<shape::Polyline>(lineWidth, yPoints, lineColor, "", false);

            if (type == "dash") {
                yLine->SetDashLine(json::ParseDashArray(crosshairsStyle["dash"], canvasContext.GetDevicePixelRatio()));
            }
            container_->AddElement(std::move(yLine));
        }
    }

  private:
    nlohmann::json config_;
    shape::Group *container_ = nullptr;
};
} // namespace tooltip
} // namespace xg

#endif // XG_GRAPHICS_TOOLTIP_H
