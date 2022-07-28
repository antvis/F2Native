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
    }

    float GetDefaultWidthRatio(XChart &chart);

    unordered_map<string, double> CreateShapePointsCfg(XChart &chart, XData &item, size_t index); // {x, y, y0, size}
    vector<double> CreateShapePoints(XChart &chart, XData &item, size_t index);

    void BeforeMapping(XChart &chart, XDataGroup &dataArray) override;

    vector<util::Point> getRectPoints(const unordered_map<string, double> &cfg, const vector<double> &y);

    virtual void Draw(XChart &chart, const XDataArray &groupData, std::size_t start, std::size_t end) const override;

    Interval &Tag(const std::string &json = "{}");

  private:
    nlohmann::json tagConfig_;
};
} // namespace geom
} // namespace xg

#endif /* XG_GRAPHICS_GEOM_INTERVAL_H */
