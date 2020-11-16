#ifndef XG_GRAPHICS_EVENT_INTERACTION_BASE_H
#define XG_GRAPHICS_EVENT_INTERACTION_BASE_H

namespace xg {
class XChart;
namespace interaction {
class InteractionBase {
  public:
    InteractionBase(XChart *chart) : chart_(chart) {}

    virtual ~InteractionBase() { chart_ = nullptr; }

  protected:
    XChart *chart_ = nullptr;
};
} // namespace interaction
} // namespace xg

#endif // XG_GRAPHICS_EVENT_INTERACTION_BASE_H
