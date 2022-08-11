#ifndef XG_GRAPHICS_ANIMATE_GEOM_H
#define XG_GRAPHICS_ANIMATE_GEOM_H

#include "TimeLine.h"
#include "../canvas/Coord.h"
#include "../shape/Group.h"
#include "../../reflection/reflection.h"

namespace xg {
class XChart;
namespace animate {

namespace action {

void GroupScaleIn(animate::TimeLine *timeLine,
                         shape::Group *container,
                         nlohmann::json &cfg,
                         canvas::coord::AbstractCoord *coord,
                         util::Point zeroY,
                         std::string type);

void GroupScaleInX(animate::TimeLine *timeLine, shape::Group *container, nlohmann::json &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY);

void GroupScaleInY(animate::TimeLine *timeLine, shape::Group *container, nlohmann::json &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY);

void GroupScaleInXY(animate::TimeLine *timeLine, shape::Group *container, nlohmann::json &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY);

void GroupWaveIn(animate::TimeLine *timeLine, shape::Group *container, nlohmann::json &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY);

void DoGroupAnimate(std::string animateType,
                           animate::TimeLine *timeLine,
                           shape::Group *container,
                           nlohmann::json &cfg,
                           canvas::coord::AbstractCoord *coord,
                           util::Point zeroY);

} // namespace action

util::Matrix GetScaledShapeMatrix(shape::Shape *shape, util::Vector2D *v, std::string direct);

void DoAnimation(TimeLine *timeLine_, shape::Shape *shape, const AnimateState &endState, nlohmann::json &cfg, std::function<void()> onEnd);


struct AnimateCfg {
    bool enable = false;
};
extern void from_json(const nlohmann::json& j, AnimateCfg& p);

class GeomAnimate {
  public:
    GeomAnimate(XChart *chart_);

    ~GeomAnimate();

    static std::string GetGeomAnimateDefaultCfg(std::string geomType, canvas::coord::AbstractCoord *coord);

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
