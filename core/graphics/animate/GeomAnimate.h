#ifndef XG_GRAPHICS_ANIMATE_GEOM_H
#define XG_GRAPHICS_ANIMATE_GEOM_H

#include "TimeLine.h"
#include "../canvas/Coord.h"
#include "../shape/Group.h"
#include "../../reflection/reflection.h"

namespace xg {
class XChart;
namespace animate {

struct AnimateCfg {
    bool hidden = true;
    string animate;
    string erasing = "linear";
    long delay = 16;
    long duration = 450;
};

namespace action {

void GroupScaleIn(animate::TimeLine *timeLine,
                         shape::Group *container,
                         const AnimateCfg &cfg,
                         canvas::coord::AbstractCoord *coord,
                         util::Point zeroY,
                         std::string type);

void GroupScaleInX(animate::TimeLine *timeLine, shape::Group *container, const AnimateCfg &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY);

void GroupScaleInY(animate::TimeLine *timeLine, shape::Group *container, const AnimateCfg &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY);

void GroupScaleInXY(animate::TimeLine *timeLine, shape::Group *container, const AnimateCfg &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY);

void GroupWaveIn(animate::TimeLine *timeLine, shape::Group *container, const AnimateCfg &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY);

void DoGroupAnimate(std::string animateType,
                           animate::TimeLine *timeLine,
                           shape::Group *container,
                           const AnimateCfg &cfg,
                           canvas::coord::AbstractCoord *coord,
                           util::Point zeroY);

} // namespace action

util::Matrix GetScaledShapeMatrix(shape::Shape *shape, util::Vector2D *v, std::string direct);

void DoAnimation(TimeLine *timeLine_, shape::Shape *shape, const AnimateState &endState, const animate::AnimateCfg &cfg, std::function<void()> onEnd);


class GeomAnimate {
  public:
    GeomAnimate(XChart *chart_);

    ~GeomAnimate();

    static std::string GetGeomAnimateDefaultCfg(const std::string &geomType, canvas::coord::AbstractCoord *coord);

    void OnBeforeCanvasDraw();

    void Clear();
    
    inline void SetAnimateConfig(const AnimateCfg &cfg) {
        animateCfg_ = cfg;
    }
  private:
    TimeLine *timeLine_ = nullptr;
    XChart *chart_ = nullptr;
    bool isUpdate_ = false;
    AnimateCfg animateCfg_;
};
} // namespace animate
} // namespace xg

#endif // XG_GRAPHICS_ANIMATE_GEOM_H
