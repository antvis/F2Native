
#include "Geom.h"

#ifndef XG_GRAPHICS_GEOM_POINT_H
#define XG_GRAPHICS_GEOM_POINT_H

namespace xg {
class XChart;

namespace geom {
class Point : public AbstractGeom {
  public:
    Point(Group *_container, utils::Tracer *tracer) : AbstractGeom(_container, tracer) {
        type_ = "point";
        sortable_ = true;
        this->styleConfig_ = {{"radius", 3}, {"color", "#1890FF"}};
    }

    void BeforeMapping(XChart &chart, nlohmann::json &dataArray) override;

    void Draw(XChart &chart, const nlohmann::json &groupData) const override;
};
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_POINT_H
