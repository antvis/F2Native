#ifndef XG_GRAPHICS_GEOM_INTERVAL_H
#define XG_GRAPHICS_GEOM_INTERVAL_H

#include "graphics/geom/Geom.h"
#include "graphics/util/json.h"

namespace xg {
namespace geom {

class Interval : public AbstractGeom {
  public:
    Interval(Group *_container, utils::Tracer *tracer) : AbstractGeom(_container, tracer) {
        type_ = "interval";
        generatePoints_ = true;
        this->styleConfig_ = {{"lineWidth", 1}, {"widthRatio", 0.5}};
    }

    float GetDefaultWidthRatio(XChart &chart);

    nlohmann::json CreateShapePointsCfg(XChart &chart, nlohmann::json &item, size_t index); // {x, y, y0, size}

    void BeforeMapping(XChart &chart, nlohmann::json &dataArray) override;

    nlohmann::json getRectPoints(nlohmann::json &cfg);

    virtual void Draw(XChart &chart, const nlohmann::json &groupData, std::size_t start, std::size_t end) const override;

    Interval &Tag(const std::string &json = "{}");

  private:
    nlohmann::json tagConfig_;
};
} // namespace geom
} // namespace xg

#endif /* XG_GRAPHICS_GEOM_INTERVAL_H */
