#ifndef XG_GRAPHICS_GEOM_POINT_H
#define XG_GRAPHICS_GEOM_POINT_H

#include "Geom.h"

namespace xg {
class XChart;

namespace geom {
class Point : public AbstractGeom {
  public:
    Point(Group *_container, utils::Tracer *tracer) : AbstractGeom(_container, tracer) {
        type_ = "point";
        sortable_ = true;
//        this->styleConfig_ = {};
    }

    void Draw(XChart &chart, const XDataArray &groupData, std::size_t start, std::size_t end) const override;
};
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_POINT_H
