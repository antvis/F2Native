#ifndef XG_GRAPHICS_GEOM_PATH_H
#define XG_GRAPHICS_GEOM_PATH_H

#include "graphics/geom/Geom.h"

namespace xg {
namespace geom {

class Path : public AbstractGeom {
  public:
    Path(Group *_container, utils::Tracer *tracer) : AbstractGeom(_container, tracer) {
        type_ = "path";
        shapeType_ = "line";
    }
};
} // namespace geom
} // namespace xg

#endif /* XG_GRAPHICS_GEOM_PATH_H */
