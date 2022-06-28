#ifndef XG_GRAPHICS_GEOM_INTERVAL_H
#define XG_GRAPHICS_GEOM_INTERVAL_H

#include "Geom.h"
#include "../util/json.h"

namespace xg {
namespace geom {

class Interval : public AbstractGeom {
  public:
    Interval(Group *_container, utils::Tracer *tracer) : AbstractGeom(_container, tracer) {
        type_ = "interval";
        generatePoints_ = true;
        nlohmann::json defaultCfg ={{"lineWidth", 1}, {"stroke", "#ffffff"}, {"widthRatio", 0.5}};
        if(this->styleConfig_.is_object()) {
            this->styleConfig_.merge_patch(defaultCfg);
        }
    }

    float GetDefaultWidthRatio(XChart &chart);

    nlohmann::json CreateShapePointsCfg(XChart &chart, nlohmann::json &item, size_t index); // {x, y, y0, size}

    void BeforeMapping(XChart &chart, XDataGroup &dataArray) override;

    nlohmann::json getRectPoints(nlohmann::json &cfg);

    virtual void Draw(XChart &chart, const XDataArray &groupData, std::size_t start, std::size_t end) const override;

    Interval &Tag(const std::string &json = "{}");

  private:
    nlohmann::json tagConfig_;
};
} // namespace geom
} // namespace xg

#endif /* XG_GRAPHICS_GEOM_INTERVAL_H */
