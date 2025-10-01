#ifndef XG_GRAPHICS_TOOLTIP_CONTROLLER_H
#define XG_GRAPHICS_TOOLTIP_CONTROLLER_H

#include "ToolTip.h"
#include "../event/EventController.h"
#include "../shape/Group.h"
#include "../scale/Scale.h"

namespace xg {
class XChart;
namespace tooltip {

typedef std::function<void(nlohmann::json &data)> ToolTipMarkerItemsCallback;
class ToolTipController {
  public:
    ToolTipController(XChart *chart);
    void Init(const nlohmann::json &config = {});
    void AddConfig(const nlohmann::json &config = {});

    ~ToolTipController();

    void AddMonitor(ToolTipMarkerItemsCallback callback) { actionListeners_.push_back(callback); }

    void Clear() { toolTip_ = nullptr; toolTips_.clear(); configs_.clear(); }
    bool isTooltips() { return configs_.size() > 0; }
    std::vector<nlohmann::json> &GetConfigs() { return configs_; }
  protected:
    void OnRender();
    void OnClearInner();

  private:
    bool OnPressStart(event::Event &event);
    bool OnPress(event::Event &event);
    bool OnPressEnd(event::Event &event);

    bool ShowToolTip(const util::Point &point);
    bool ShowToolTips(const util::Point &point);

    bool HideToolTip();

    std::string InvertYTip(const util::Point &p, xg::scale::AbstractScale &yScale);

  private:
    XChart *chart_;
    
    std::vector<nlohmann::json> configs_ = {};
    std::vector<std::unique_ptr<ToolTip>> toolTips_;
    std::unique_ptr<ToolTip> toolTip_;
    nlohmann::json config_ = {{"alwaysShow", false},
                              {"showTitle", false},
                              {"showCrosshairs", false},
                              {"tooltipMarkerStyle", {}},
                              {"crosshairsType", "xy"},
                              {"crosshairsStyle", {{"stroke", "#1890FF"}, {"lineWidth", 1}, {"type", "dash"}, {"dash", {4, 4}}}},
                              {"showTooltipMarker", true},
                              {"background", {{"radius", 1}, {"fill", "#1890FF"}, {"padding", {3, 3}}}},
                              {"xPositionType", "record"},
                              {"yPositionType", "coord"},
                              {"showXTip", true},
                              {"showYTip", true},
                              {"xTip", {{"inner", false}, {"fontSize", 10}, {"fill", "#ffffff"}, {"textAlign", "center"}, {"textBaseline", "bottom"}}},
                              {"yTip", {{"inner", false}, {"fontSize", 10}, {"fill", "#ffffff"}, {"textAlign", "center"}, {"textBaseline", "bottom"}}},
                              {"titleStyle", {{"fontSize", 12}, {"fill", "#ffffff"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
                              {"nameStyle", {{"fontSize", 12}, {"fill", "#ffffffa6"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
                              {"valueStyle", {{"fontSize", 12}, {"fill", "#ffffff"}, {"textAlign", "start"}, {"textBaseline", "middle"}}},
                              {"showItemMarker", true},
                              {"itemMarkerStyle", {{"radius", 3}, {"symbol", "circle"}, {"lineWidth", 1}, {"stroke", "#ffffff"}}},
                              {"layout", "horizontal"},
                              {"onPress", false},
                              {"snap", false},
                              {"hidden", false}};
    long long lastShowTimeStamp_ = 0;
    std::vector<ToolTipMarkerItemsCallback> actionListeners_{};
    shape::Group *container_ = nullptr;
};
} // namespace tooltip
} // namespace xg

#endif // XG_GRAPHICS_TOOLTIP_CONTROLLER_H
