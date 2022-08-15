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
#include "../util/Point.h"
#include "../util/json.h"
#include "../../reflection/reflection.h"

namespace xg {
namespace tooltip {

struct CrosshairsStyle {
    bool yCrosshair = true;
    string stroke = "#1890FF";
    float lineWidth = 1.f;
    string type = "dash";
    vector<float> dash = {4, 4};
    bool hidden = false;
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(CrosshairsStyle)
        FIELDS(FIELD(&CrosshairsStyle::yCrosshair),
               FIELD(&CrosshairsStyle::stroke),
               FIELD(&CrosshairsStyle::lineWidth),
               FIELD(&CrosshairsStyle::type),
               FIELD(&CrosshairsStyle::dash),
               FIELD(&CrosshairsStyle::hidden))
        CTORS(DEFAULT_CTOR(CrosshairsStyle))
    END_TYPE
#endif
};

struct RectCfg {
    float radius = 1.f;
    string fill = "#1890FF";
    vector<float> padding =  {3, 3};
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(RectCfg)
        FIELDS(FIELD(&RectCfg::radius),
               FIELD(&RectCfg::fill),
               FIELD(&RectCfg::padding))
        CTORS(DEFAULT_CTOR(RectCfg))
    END_TYPE
#endif
};

struct Tip {
    bool inner = false;
    float fontSize = 10;
    string fill = "#ffffff";
    string textAlign = "center";
    string textBaseline = "bottom";
    bool hidden = false;
    
    
    string ToJson() {
        std::stringstream ss;
        ss << "{";
        ss << "\"inner\":" << inner <<",";
        ss << "\"fontSize\":" << fontSize <<",";
        if(!fill.empty()) ss << "\"fill\":\"" <<fill <<"\",";
        if(!textAlign.empty())  ss << "\"textAlign\":\"" << textAlign <<"\",";
        if(!textBaseline.empty()) ss << "\"textBaseline\":\"" << textBaseline <<"\",";
        ss << "\"hidden\":" << hidden ;
        //todo xtip ytip
        ss << "}";
        return ss.str();
    }
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(Tip)
        FIELDS(FIELD(&Tip::inner),
               FIELD(&Tip::fontSize),
               FIELD(&Tip::fill),
               FIELD(&Tip::textAlign),
               FIELD(&Tip::textBaseline),
               FIELD(&Tip::hidden))
        CTORS(DEFAULT_CTOR(Tip))
    END_TYPE
#endif
};

struct ToolTipCfg {
    string onPressStart, onPress, onPressEnd;
    CrosshairsStyle crosshairsStyle;
    RectCfg background;
    Tip xTip, yTip;
    bool hidden = false;
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(ToolTipCfg)
        FIELDS(FIELD(&ToolTipCfg::onPressStart),
               FIELD(&ToolTipCfg::onPress),
               FIELD(&ToolTipCfg::onPressEnd),
               FIELD(&ToolTipCfg::crosshairsStyle),
               FIELD(&ToolTipCfg::background),
               FIELD(&ToolTipCfg::xTip),
               FIELD(&ToolTipCfg::yTip),
               FIELD(&ToolTipCfg::hidden))
        CTORS(DEFAULT_CTOR(ToolTipCfg))
    END_TYPE
#endif
};

struct ToolTipItem {
    double x, y;
    std::string color;
    std::string name;
    std::string value;
    std::string title;
    double touchX, touchY;
    Tip xTip, yTip;
    
    string ToJson() {
        std::stringstream ss;
        ss << "{";
        ss << "\"x\":" << x <<",";
        ss << "\"y\":" << y <<",";
        if(!color.empty()) ss << "\"color\":\"" <<color <<"\",";
        if(!name.empty())  ss << "\"name\":\"" << name <<"\",";
        if(!value.empty()) ss << "\"value\":\"" << value <<"\",";
        if(!title.empty()) ss << "\"title\":\"" << title <<"\",";
        ss << "\"touchX\":" << touchX <<",";
        ss << "\"touchY\":" << touchY <<",";
        ss << "\"xTip\":" << xTip.ToJson() <<",";
        ss << "\"yTip\":" << yTip.ToJson();
        ss << "}";
        return ss.str();
    }
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(ToolTipItem)
        FIELDS(FIELD(&ToolTipItem::x),
               FIELD(&ToolTipItem::y),
               FIELD(&ToolTipItem::color),
               FIELD(&ToolTipItem::name),
               FIELD(&ToolTipItem::value),
               FIELD(&ToolTipItem::title),
               FIELD(&ToolTipItem::touchX),
               FIELD(&ToolTipItem::touchY),
               FIELD(&ToolTipItem::xTip),
               FIELD(&ToolTipItem::yTip))
        CTORS(DEFAULT_CTOR(ToolTipItem))
    END_TYPE
#endif
};

struct ToolTipItemList {
    vector<ToolTipItem> tooltip;
    
    string ToJson() {
        std::stringstream ss;
        ss << "{" << "\"tooltip\":";
        
        for (auto &item : tooltip) {
            ss << "[" << item.ToJson() << "],";
        }
        ss.seekp(-1, ss.cur);
        ss << "}";
        return ss.str();
    }
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(ToolTipItemList)
        FIELDS(FIELD(&ToolTipItemList::tooltip))
        CTORS(DEFAULT_CTOR(ToolTipItemList))
    END_TYPE
#endif
};

class ToolTip {
  public:
    ToolTip(shape::Group *container, ToolTipCfg &cfg) : container_(container), config_(cfg) {}
    ~ToolTip() { container_ = nullptr; }

    void SetPosition(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, util::Point point, const ToolTipItem &tooltipItems) {
        this->_ShowCrosshairs(coord, canvasContext, point);
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

    void SetXTipContent(canvas::CanvasContext &canvasContext, const ToolTipItem &tooltipItems, const util::Point &point, const util::Point &xAxis) {
        if(config_.xTip.hidden) {
            return;
        }
        const std::string &content = tooltipItems.title;
        auto &backCfg = config_.background;
        const std::string &backColor = backCfg.fill;
        auto &padding = backCfg.padding;
        double radius = backCfg.radius* canvasContext.GetDevicePixelRatio();
        double paddingH = padding[0] * canvasContext.GetDevicePixelRatio();
        double paddingV = padding[1] * canvasContext.GetDevicePixelRatio();

        const auto &xTipCfg = tooltipItems.xTip;
        const std::string &color = xTipCfg.fill;
        const std::string &textAlign = xTipCfg.textAlign;
        const std::string &textBaseline = xTipCfg.textBaseline;
        double fontSize = xTipCfg.fontSize *  canvasContext.GetDevicePixelRatio();
        bool inner = xTipCfg.inner;
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
                        const ToolTipItem &tooltipItems,
                        const util::Point &point) {
        if(config_.yTip.hidden) {
            return;
        }
        const std::string &content = tooltipItems.value;
        auto &backCfg = config_.background;
        const std::string &backColor = backCfg.fill;
        auto &padding = backCfg.padding;
        double paddingH = padding[0] * canvasContext.GetDevicePixelRatio();
        double paddingV = padding[1] * canvasContext.GetDevicePixelRatio();

        const auto &yTipCfg = tooltipItems.yTip;
        const std::string &color = yTipCfg.fill;
        const std::string &textAlign = yTipCfg.textAlign;
        const std::string &textBaseline = yTipCfg.textBaseline;
        double fontSize = yTipCfg.fontSize * canvasContext.GetDevicePixelRatio();
        bool inner = yTipCfg.inner;
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
    void _ShowCrosshairs(canvas::coord::AbstractCoord &coord, canvas::CanvasContext &canvasContext, util::Point &point) {
        if (config_.crosshairsStyle.hidden) {
            return;
        }
        
        const auto &crosshairsStyle = config_.crosshairsStyle;
        const std::string &lineColor = crosshairsStyle.stroke;
        double lineWidth = crosshairsStyle.lineWidth;
        const std::string &type = crosshairsStyle.type;
        bool showY = crosshairsStyle.yCrosshair;

        lineWidth *= canvasContext.GetDevicePixelRatio();

        vector<xg::util::Point> xPoints;
        xPoints.push_back(util::Point{point.x, coord.GetYAxis().x});
        xPoints.push_back(util::Point{point.x, coord.GetYAxis().y});

        auto xLine = xg::make_unique<shape::Polyline>(lineWidth, xPoints, lineColor, "", false);
        auto dash = json::ScaleDash(crosshairsStyle.dash, canvasContext.GetDevicePixelRatio());
        if(type == "dash") {
            xLine->SetDashLine(dash);
        }

        container_->AddElement(std::move(xLine));

        if(showY) {
            vector<xg::util::Point> yPoints;
            yPoints.push_back(util::Point{coord.GetXAxis().x, point.y});
            yPoints.push_back(util::Point{coord.GetXAxis().y, point.y});

            auto yLine = xg::make_unique<shape::Polyline>(lineWidth, yPoints, lineColor, "", false);

            if(type == "dash") {
                yLine->SetDashLine(dash);
            }
            container_->AddElement(std::move(yLine));
        }
    }

  private:
    ToolTipCfg config_;
    shape::Group *container_ = nullptr;
};
} // namespace tooltip
} // namespace xg

#endif // XG_GRAPHICS_TOOLTIP_H
