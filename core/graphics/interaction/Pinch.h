#include "InteractionBase.h"
#include "graphics/event/EventController.h"

#ifndef XG_GRAPHICS_EVENT_INTERACTION_PINCH_H
#define XG_GRAPHICS_EVENT_INTERACTION_PINCH_H

namespace xg {
class XChart;
namespace interaction {

class Pinch : public InteractionBase {
  public:
    Pinch(XChart *chart);

    void OnPinchStart(event::Event &event);

    void OnPinch(event::Event &event);

    void onPinchEnd(event::Event &event);
};
} // namespace interaction
} // namespace xg

#endif // XG_GRAPHICS_EVENT_INTERACTION_PINCH_H
