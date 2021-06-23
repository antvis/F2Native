#include "Animator.h"

#ifndef XG_GRAPHICS_ANIMATE_TIMELINE_H
#define XG_GRAPHICS_ANIMATE_TIMELINE_H

namespace xg {
class XChart;
namespace animate {
class TimeLine {
  public:
    TimeLine(XChart *chart) : chart_(chart) {}

    void Play();

    void Stop();

    void PushAnim(AnimInfo &&animInfo);

    ~TimeLine();

  protected:
    void OnUpdate();

  private:
    void frameLoop();

  private:
    XChart *chart_;
    bool playing_ = false;
    std::vector<AnimInfo> animInfos_;
    long long time_;
};
} // namespace animate
} // namespace xg

#endif // XG_GRAPHICS_ANIMATE_TIMELINE_H