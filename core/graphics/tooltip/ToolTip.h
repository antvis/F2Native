
#include "graphics/canvas/CanvasContext.h"
#include "graphics/canvas/Coord.h"
#include "graphics/global.h"
#include "graphics/shape/Group.h"
#include "graphics/shape/Line.h"
#include "graphics/shape/Polyline.h"
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
    ToolTip(shape::Group *container, nlohmann::json cfg) : container_(container), config_(cfg) {}
    ~ToolTip() { container_ = nullptr; }

    void SetPosition(canvas::coord::AbstractCoord &coord,
                     canvas::CanvasContext &canvasContext,
                     util::Point point,
                     nlohmann::json &tooltipItems,
                     const std::string &yTip) {
        this->_ShowCrosshairs(coord, canvasContext, point, true);
        this->SetYTipContent(coord, canvasContext, yTip, point);
    }

    void SetXTipContent(canvas::CanvasContext &canvasContext, const std::string &content, util::Point point) {
        if(!config_["xTip"].is_object()) {
            return;
        }
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
        if(!config_["yTip"].is_object()) {
            return;
        }
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

        nlohmann::json &crosshairsStyle = config_["crosshairsStyle"];
        std::string lineColor = crosshairsStyle["stroke"];
        double lineWidth = crosshairsStyle["lineWidth"];
        std::string type = crosshairsStyle["type"];

        lineWidth *= canvasContext.GetDevicePixelRatio();

        vector<xg::util::Point> xPoints;
        xPoints.push_back(util::Point{point.x, coord.GetYAxis().x});
        xPoints.push_back(util::Point{point.x, coord.GetYAxis().y});

        auto xLine = xg::make_unique<shape::Polyline>(lineWidth, xPoints, lineColor, "", false);
        if(type == "dash") {
            xLine->SetDashLine(xg::GLOBAL_LINE_DASH);
        }

        container_->AddElement(std::move(xLine));

        if(showY && config_["yTip"].is_object()) {
            vector<xg::util::Point> yPoints;
            yPoints.push_back(util::Point{coord.GetXAxis().x, point.y});
            yPoints.push_back(util::Point{coord.GetXAxis().y, point.y});

            auto yLine = xg::make_unique<shape::Polyline>(lineWidth, yPoints, lineColor, "", false);

            if(type == "dash") {
                yLine->SetDashLine(xg::GLOBAL_LINE_DASH);
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
