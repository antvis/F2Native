#include "InteractionBase.h"
#include "graphics/event/EventController.h"

#ifndef XG_GRAPHICS_EVENT_INTERACTION_PAN_H
#define XG_GRAPHICS_EVENT_INTERACTION_PAN_H

namespace xg {
class XChart;
namespace interaction {

class Pan : public InteractionBase {
  public:
    Pan(XChart *chart);

    void OnPanStart(event::Event &event);

    void OnPan(event::Event &event);

    // void onPanEnd(event::Event &event);
};
} // namespace interaction
} // namespace xg

#endif // XG_GRAPHICS_EVENT_INTERACTION_PAN_H
