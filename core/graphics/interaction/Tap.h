#ifndef XG_GRAPHICS_EVENT_INTERACTION_CLICK_H
#define XG_GRAPHICS_EVENT_INTERACTION_CLICK_H

#include "InteractionBase.h"
#include "../event/EventController.h"

namespace xg {
class XChart;
namespace interaction {

class Tap : public InteractionBase {
  public:
    Tap(XChart *chart);

    virtual ~Tap();

    const std::string onClick(event::Event &event);
  private:
    const std::string handleClick(const util::Point &point);
};
} // namespace interaction
} // namespace xg

#endif // XG_GRAPHICS_EVENT_INTERACTION_PINCH_H
