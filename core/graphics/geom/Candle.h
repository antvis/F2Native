#ifndef XG_GRAPHICS_GEOM_CANDLE_H
#define XG_GRAPHICS_GEOM_CANDLE_H

#include "Geom.h"
#include "../util/json.h"

namespace xg {
namespace geom {

class Candle : public AbstractGeom {
  public:
    Candle(Group *_container, utils::Tracer *tracer) : AbstractGeom(_container, tracer) {
        type_ = "candle";
        generatePoints_ = true;
    }

    unordered_map<string, double> CreateShapePointsCfg(XChart &chart, XData &item, size_t index);

    void BeforeMapping(XChart &chart, XDataGroup &dataArray) override;

    void Draw(XChart &chart, const XDataArray &groupData, std::size_t start, std::size_t end) const override;

  private:
    array<util::Point, 4> getRectPoints(unordered_map<string, double> &cfg);
    array<double, 2> getLinePoints(unordered_map<string, double> &cfg);
};
} // namespace geom
} // namespace xg

#endif /* XG_GRAPHICS_GEOM_CANDLE_H */
