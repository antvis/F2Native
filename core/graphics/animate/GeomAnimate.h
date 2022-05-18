#ifndef XG_GRAPHICS_ANIMATE_GEOM_H
#define XG_GRAPHICS_ANIMATE_GEOM_H

#include "TimeLine.h"
#include "../canvas/Coord.h"
#include "../shape/Group.h"

namespace xg {
class XChart;
namespace animate {

namespace action {

static void GroupScaleIn(animate::TimeLine *timeLine,
                         shape::Group *container,
                         nlohmann::json &cfg,
                         canvas::coord::AbstractCoord *coord,
                         util::Point zeroY,
                         std::string type);

static void
GroupScaleInX(animate::TimeLine *timeLine, shape::Group *container, nlohmann::json &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY) {
    GroupScaleIn(timeLine, container, cfg, coord, zeroY, "x");
}

static void
GroupScaleInY(animate::TimeLine *timeLine, shape::Group *container, nlohmann::json &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY) {
    GroupScaleIn(timeLine, container, cfg, coord, zeroY, "y");
}

static void
GroupScaleInXY(animate::TimeLine *timeLine, shape::Group *container, nlohmann::json &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY) {
    GroupScaleIn(timeLine, container, cfg, coord, zeroY, "xy");
}

static void
GroupWaveIn(animate::TimeLine *timeLine, shape::Group *container, nlohmann::json &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY);

static void DoGroupAnimate(std::string animateType,
                           animate::TimeLine *timeLine,
                           shape::Group *container,
                           nlohmann::json &cfg,
                           canvas::coord::AbstractCoord *coord,
                           util::Point zeroY);

} // namespace action

static util::Matrix GetScaledShapeMatrix(shape::Shape *shape, util::Vector2D *v, std::string direct);

static void
DoAnimation(TimeLine *timeLine_, shape::Shape *shape, const AnimateState &endState, nlohmann::json &cfg, std::function<void()> onEnd);

class GeomAnimate {
  public:
    GeomAnimate(XChart *chart_);

    ~GeomAnimate();

    static std::string GetGeomAnimateDefaultCfg(std::string geomType, canvas::coord::AbstractCoord *coord);

    void OnBeforeCanvasDraw();

    void Clear();
  private:
    TimeLine *timeLine_ = nullptr;
    XChart *chart_ = nullptr;
    bool isUpdate_ = false;
};
} // namespace animate
} // namespace xg

#endif // XG_GRAPHICS_ANIMATE_GEOM_H
