#ifndef XG_GRAPHICS_TOOLTIP_CONTROLLER_H
#define XG_GRAPHICS_TOOLTIP_CONTROLLER_H

#include "ToolTip.h"
#include "../event/EventController.h"
#include "../shape/Group.h"
#include "../scale/Scale.h"

namespace xg {
class XChart;
namespace tooltip {

typedef std::function<void()> ToolTipMarkerItemsCallback;
class ToolTipController {
  public:
    ToolTipController(XChart *chart);
    void Init(const ToolTipCfg &config);

    ~ToolTipController();

    void AddMonitor(ToolTipMarkerItemsCallback callback) { actionListeners_.push_back(callback); }

    void Clear() { toolTip_ = nullptr;}
  protected:
    void OnRender();
    void OnClearInner();

  private:
    bool OnPressStart(event::Event &event);
    bool OnPress(event::Event &event);
    bool OnPressEnd(event::Event &event);

    bool ShowToolTip(const util::Point &point);

    bool HideToolTip();

    std::string InvertYTip(const util::Point &p, xg::scale::AbstractScale &yScale);

  private:
    XChart *chart_;

    std::unique_ptr<ToolTip> toolTip_;
    ToolTipCfg config_;
    long long lastShowTimeStamp_ = 0;
    std::vector<ToolTipMarkerItemsCallback> actionListeners_{};
    shape::Group *container_ = nullptr;
};
} // namespace tooltip
} // namespace xg

#endif // XG_GRAPHICS_TOOLTIP_CONTROLLER_H
