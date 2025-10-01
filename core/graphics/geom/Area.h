#ifndef XG_GRAPHICS_GEOM_AREA_H
#define XG_GRAPHICS_GEOM_AREA_H

#include "Geom.h"

namespace xg {
class XChart;
namespace geom {

class Area : public AbstractGeom {
  public:
    Area(Group *_container, utils::Tracer *tracer) : AbstractGeom(_container, tracer) {
        type_ = "area";
        shapeType_ = "area";
        sortable_ = true;
        generatePoints_ = true;
    }

    void BeforeMapping(XChart &chart, nlohmann::json &dataArray) override;

  private:
    nlohmann::json CreateShapePointsCfg(XChart &chart, nlohmann::json &data);

    nlohmann::json GetAreaPoints(XChart &chart, nlohmann::json &data, nlohmann::json &cfg);
};
} // namespace geom
} // namespace xg

#endif /* XG_GRAPHICS_GEOM_AREA_H */
