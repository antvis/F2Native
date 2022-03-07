
#include "graphics/canvas/CanvasContext.h"
#include "graphics/canvas/Coord.h"
#include "graphics/global.h"
#include "graphics/shape/Group.h"
#include "graphics/shape/Line.h"
#include "graphics/shape/Polyline.h"
#include "graphics/shape/Rect.h"
#include "graphics/shape/Text.h"
#include "graphics/util/Point.h"
#include "graphics/util/json.h"
#include <nlohmann/json.hpp>

#ifndef XG_GRAPHICS_TOOLTIP_H
#define XG_GRAPHICS_TOOLTIP_H

namespace xg {
namespace tooltip {
class ToolTip {
  public:
    ToolTip(shape::Group *container, nlohmann::json cfg) : container_(container), config_(cfg) {}
    ~ToolTip() { container_ = nullptr; }

    void SetPosition(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, util::Point point, const nlohmann::json &tooltipItems) {
        this->_ShowCrosshairs(coord, canvasContext, point, true);
        this->SetYTipContent(coord, canvasContext, tooltipItems, point);
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

    void SetXTipContent(canvas::CanvasContext &canvasContext, const nlohmann::json &tooltipItems, const util::Point &point, const util::Point &xAxis) {
        if(!config_["xTip"].is_object()) {
            return;
        }
        const std::string &content = tooltipItems["title"];
        nlohmann::json &backCfg = config_["background"];
        const std::string &backColor = backCfg["fill"];
        nlohmann::json &padding = backCfg["padding"];
        double radius = backCfg["radius"].get<double>() * canvasContext.GetDevicePixelRatio();
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
        float rectY = inner ? point.y - fontSize - paddingV * 2 : point.y;
        float textY = inner ? point.y - paddingV: point.y + fontSize + paddingV;
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
            std::make_unique<shape::Rect>(util::Point{limitX, rectY + rectOffsetY},
                                          util::Size{width + paddingH * 2, fontSize + paddingV * 2}, backColor);
        container_->AddElement(std::move(backRect));

        limitX +=  width / 2 + paddingH - rectOffsetX;
        xTip->SetPoint(util::Point{limitX, textY});
        xTip->SetTextAlign(textAlign);
        xTip->SetTextBaseline(textBaseline);
        container_->AddElement(std::move(xTip));
    }

    void SetYTipContent(canvas::coord::AbstractCoord &coord,
                        canvas::CanvasContext &canvasContext,
                        const nlohmann::json &tooltipItems,
                        const util::Point &point) {
        if(!config_["yTip"].is_object()) {
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
        float rectX = inner ? coord.GetXAxis().x : coord.GetXAxis().x - width - paddingH * 2;
        float textX = inner ? coord.GetXAxis().x + width / 2 + paddingH : coord.GetXAxis().x - width / 2 - paddingH;
        float rectOffsetX = GetXTipRectOffsetX(textAlign, width);
        float rectOffsetY = GetXTipRectOffsetY(textBaseline, fontSize);

        std::unique_ptr<shape::Rect> backRect =
            std::make_unique<shape::Rect>(util::Point{rectX + rectOffsetX, point.y - fontSize / 2 - paddingV + rectOffsetY},
                                          util::Size{width + paddingH * 2, fontSize + paddingV * 2}, backColor);
        container_->AddElement(std::move(backRect));

        yTip->SetPoint(util::Point{textX, point.y + fontSize / 2});
        yTip->SetTextAlign(textAlign);
        yTip->SetTextBaseline(textBaseline);
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
    void _ShowCrosshairs(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, util::Point &point, bool showY) {
        if(config_["showCrosshairs"] == false) {
            return;
        }

        const nlohmann::json &crosshairsStyle = config_["crosshairsStyle"];
        const std::string &lineColor = crosshairsStyle["stroke"];
        double lineWidth = crosshairsStyle["lineWidth"];
        const std::string &type = crosshairsStyle["type"];

        lineWidth *= canvasContext.GetDevicePixelRatio();

        vector<xg::util::Point> xPoints;
        xPoints.push_back(util::Point{point.x, coord.GetYAxis().x});
        xPoints.push_back(util::Point{point.x, coord.GetYAxis().y});

        auto xLine = xg::make_unique<shape::Polyline>(lineWidth, xPoints, lineColor, "", false);
        if(type == "dash") {
            xLine->SetDashLine(json::ParseDashArray(crosshairsStyle["dash"], canvasContext.GetDevicePixelRatio()));
        }

        container_->AddElement(std::move(xLine));

        if(showY) {
            vector<xg::util::Point> yPoints;
            yPoints.push_back(util::Point{coord.GetXAxis().x, point.y});
            yPoints.push_back(util::Point{coord.GetXAxis().y, point.y});

            auto yLine = xg::make_unique<shape::Polyline>(lineWidth, yPoints, lineColor, "", false);

            if(type == "dash") {
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
