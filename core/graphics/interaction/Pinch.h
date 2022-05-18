#ifndef XG_GRAPHICS_EVENT_INTERACTION_PINCH_H
#define XG_GRAPHICS_EVENT_INTERACTION_PINCH_H

#include "InteractionBase.h"
#include "../event/EventController.h"

namespace xg {
class XChart;
namespace interaction {

class Pinch : public InteractionBase {
  public:
    Pinch(XChart *chart);

    bool OnPinchStart(event::Event &event);

    bool OnPinch(event::Event &event);

    bool onPinchEnd(event::Event &event);
};
} // namespace interaction
} // namespace xg

#endif // XG_GRAPHICS_EVENT_INTERACTION_PINCH_H
