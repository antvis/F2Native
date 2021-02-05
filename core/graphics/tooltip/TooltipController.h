#include "ToolTip.h"
#include "graphics/event/EventController.h"
#include "graphics/shape/Group.h"
#include <nlohmann/json.hpp>

#ifndef XG_GRAPHICS_TOOLTIP_CONTROLLER_H
#define XG_GRAPHICS_TOOLTIP_CONTROLLER_H

namespace xg {
class XChart;
namespace tooltip {

typedef std::function<void(nlohmann::json &data)> ToolTipMarkerItemsCallback;
class ToolTipController {
  public:
    ToolTipController(XChart *chart);
    void Init(const nlohmann::json &config = {});

    ~ToolTipController();

    void AddMonitor(ToolTipMarkerItemsCallback callback) { actionListeners_.push_back(callback); }

  protected:
    void OnRender();
    void OnClearInner();

  private:
    bool OnPressStart(event::Event &event);
    bool OnPress(event::Event &event);
    bool OnPressEnd(event::Event &event);

    bool ShowToolTip(const util::Point &point);

    bool HideToolTip();

    std::string InvertYTip(const util::Point &p);

  private:
    XChart *chart_;

    std::unique_ptr<ToolTip> toolTip_;
    nlohmann::json config_ = {{"alwaysShow", false},
                              {"showTitle", false},
                              {"showCrosshairs", false},
                              {"crosshairsStyle", {{"stroke", "#1890FF"}, {"lineWidth", 1}, {"type", "dash"}, {"dash", {4, 4}}}},
                              {"showTooltipMarker", true},
                              {"background", {{"radius", 1}, {"fill", "#1890FF"}, {"padding", {3, 3}}}},
                              {"xTip", {{"fontSize", 10}, {"fill", "#ffffff"}, {"textAlign", "center"}, {"textBaseline", "bottom"}}},
                              {"yTip", {{"fontSize", 10}, {"fill", "#ffffff"}, {"textAlign", "center"}, {"textBaseline", "bottom"}}},
                              {"titleStyle", {{"fontSize", 12}, {"fill", "#ffffff"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
                              {"nameStyle", {{"fontSize", 12}, {"fill", "#ffffffa6"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
                              {"valueStyle", {{"fontSize", 12}, {"fill", "#ffffff"}, {"textAlign", "start"}, {"textBaseline", "middle"}}},
                              {"showItemMarker", true},
                              {"itemMarkerStyle", {{"radius", 3}, {"symbol", "circle"}, {"lineWidth", 1}, {"stroke", "#ffffff"}}},
                              {"layout", "horizontal"},
                              {"onPress", false},
                              {"snap", false},
                              {"hidden", false}};
    long lastShowTimeStamp_ = 0;
    std::vector<ToolTipMarkerItemsCallback> actionListeners_{};
    shape::Group *container_ = nullptr;
};
} // namespace tooltip
} // namespace xg

#endif // XG_GRAPHICS_TOOLTIP_CONTROLLER_H
