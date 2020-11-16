#ifndef XG_GRAPHICS_GEOM_LINE_H
#define XG_GRAPHICS_GEOM_LINE_H

#include "graphics/geom/Geom.h"

namespace xg {
namespace geom {

class Line : public AbstractGeom {
  public:
    Line(utils::Tracer *tracer) : AbstractGeom(tracer) {
        type_ = "line";
        sortable_ = true;
    }
};
} // namespace geom
} // namespace xg

#endif /* XG_GRAPHICS_GEOM_LINE_H */
