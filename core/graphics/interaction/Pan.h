#ifndef XG_GRAPHICS_EVENT_INTERACTION_PAN_H
#define XG_GRAPHICS_EVENT_INTERACTION_PAN_H

#include "InteractionBase.h"
#include "../event/EventController.h"

namespace xg {
class XChart;
namespace interaction {

class Pan : public InteractionBase {
  public:
    Pan(XChart *chart);

    bool OnPanStart(event::Event &event);

    bool OnPan(event::Event &event);

    bool onPanEnd(event::Event &event);
};
} // namespace interaction
} // namespace xg

#endif // XG_GRAPHICS_EVENT_INTERACTION_PAN_H
