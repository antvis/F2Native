
#include "graphics/canvas/CanvasContext.h"
#include "graphics/canvas/Coord.h"
#include "graphics/shape/Group.h"
#include "graphics/shape/Line.h"
#include "graphics/shape/Rect.h"
#include "graphics/shape/Text.h"
#include "graphics/util/Point.h"
#include <nlohmann/json.hpp>

#ifndef XG_GRAPHICS_TOOLTIP_H
#define XG_GRAPHICS_TOOLTIP_H

namespace xg {
namespace tooltip {
class ToolTip {
  public:
    ToolTip(nlohmann::json cfg) : config_(cfg) {}
    ~ToolTip() { this->Clear(); }

    void SetPosition(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, util::Point point, nlohmann::json &tooltipItems) {
        if(tooltipItems.size() == 1) {
            this->_ShowCrosshairs(coord, canvasContext, point, true);
            this->SetYTipContent(coord, canvasContext, tooltipItems[0]["value"], point);
        } else if(tooltipItems.size() > 1) {
            this->_ShowCrosshairs(coord, canvasContext, point, false);
        }
    }

    void SetXTipContent(canvas::CanvasContext &canvasContext, std::string content, util::Point point) {
        nlohmann::json &backCfg = config_["background"];
        std::string backColor = backCfg["fill"];
        nlohmann::json &padding = backCfg["padding"];
        double radius = backCfg["radius"].get<double>() * canvasContext.GetDevicePixelRatio();
        double paddingH = padding[0].get<double>() * canvasContext.GetDevicePixelRatio();
        double paddingV = padding[1].get<double>() * canvasContext.GetDevicePixelRatio();

        nlohmann::json &xTipCfg = config_["xTip"];
        double fontSize = xTipCfg["fontSize"];
        fontSize *= canvasContext.GetDevicePixelRatio();
        float width = canvasContext.MeasureTextWidth(content);

        std::unique_ptr<shape::Rect> backRect =
            std::make_unique<shape::Rect>(util::Point{point.x - width / 2 - paddingH, point.y},
                                          util::Size{width + paddingH * 2, fontSize + paddingV * 2}, backColor);
        //        if(radius > 0) {
        //            backRect->radius_ = radius;
        //        }
        container_->AddElement(std::move(backRect));

        std::string color = xTipCfg["fill"];
        std::string textAlign = xTipCfg["textAlign"];
        std::string textBaseline = xTipCfg["textBaseline"];
        std::unique_ptr<shape::Text> xTip =
            std::make_unique<shape::Text>(content, util::Point{point.x, point.y + fontSize + paddingV}, fontSize, color, color);
        xTip->SetTextAlign(textAlign);
        xTip->SetTextBaseline(textBaseline);
        container_->AddElement(std::move(xTip));
    }

    void SetYTipContent(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, std::string content, util::Point point) {
        nlohmann::json &backCfg = config_["background"];
        std::string backColor = backCfg["fill"];
        nlohmann::json &padding = backCfg["padding"];
        double paddingH = padding[0].get<double>() * canvasContext.GetDevicePixelRatio();
        double paddingV = padding[1].get<double>() * canvasContext.GetDevicePixelRatio();

        nlohmann::json &yTipCfg = config_["yTip"];
        double fontSize = yTipCfg["fontSize"];
        fontSize *= canvasContext.GetDevicePixelRatio();
        float width = canvasContext.MeasureTextWidth(content);

        std::unique_ptr<shape::Rect> backRect =
            std::make_unique<shape::Rect>(util::Point{coord.GetXAxis().x - width - paddingH * 2, point.y - fontSize / 2 - paddingV},
                                          util::Size{width + paddingH * 2, fontSize + paddingV * 2}, backColor);
        container_->AddElement(std::move(backRect));

        std::string color = yTipCfg["fill"];
        std::string textAlign = yTipCfg["textAlign"];
        std::string textBaseline = yTipCfg["textBaseline"];
        std::unique_ptr<shape::Text> yTip =
            std::make_unique<shape::Text>(content, util::Point{coord.GetXAxis().x - width / 2 - paddingH, point.y + fontSize / 2},
                                          fontSize, color, color);
        yTip->SetTextAlign(textAlign);
        yTip->SetTextBaseline(textBaseline);
        container_->AddElement(std::move(yTip));
    }

    void Clear() { container_->Clear(); }

    void Show(canvas::CanvasContext &canvasContext) {
        container_->Show();
        container_->Draw(canvasContext);
    }

    void Hide() { container_->Hide(); }

  private:
    void _ShowCrosshairs(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, util::Point &point, bool showY) {
        if(config_["showCrosshairs"] == false) {
            return;
        }

        std::string lineColor = config_["crosshairsStyle"]["stroke"];
        double lineWidth = config_["crosshairsStyle"]["lineWidth"];

        lineWidth *= canvasContext.GetDevicePixelRatio();

        std::unique_ptr<shape::Line> xLine = std::make_unique<shape::Line>(util::Point{point.x, coord.GetYAxis().x},
                                                                           util::Point{point.x, coord.GetYAxis().y}, lineWidth, lineColor);
        container_->AddElement(std::move(xLine));

        if(showY) {
            std::unique_ptr<shape::Line> yLine =
                std::make_unique<shape::Line>(util::Point{coord.GetXAxis().x, point.y},
                                              util::Point{coord.GetXAxis().y, point.y}, lineWidth, lineColor);
            container_->AddElement(std::move(yLine));
        }
    }

  private:
    std::unique_ptr<shape::Group> container_ = std::make_unique<shape::Group>();
    nlohmann::json config_;
};
} // namespace tooltip
} // namespace xg

#endif // XG_GRAPHICS_TOOLTIP_H