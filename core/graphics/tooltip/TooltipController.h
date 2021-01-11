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
    nlohmann::json config_;
    long lastShowTimeStamp_ = 0;
    std::vector<ToolTipMarkerItemsCallback> actionListeners_{};
    shape::Group *container_ = nullptr;
};
} // namespace tooltip
} // namespace xg

#endif // XG_GRAPHICS_TOOLTIP_CONTROLLER_H
