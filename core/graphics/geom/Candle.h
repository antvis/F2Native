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
        this->styleConfig_ = {
                {"radius", 4},
                {"lineWidth", 1},                            // lineWidth
                {"fill", {"#0E9976","#999999", "#E62C3B"}}, // or {"strock", {"#0E9976", "#999999", "#E62C3B"}}, colors: [down, equal, up]
        };
    }

    nlohmann::json CreateShapePointsCfg(XChart &chart, nlohmann::json &item, size_t index);

    void BeforeMapping(XChart &chart, nlohmann::json &dataArray) override;

    void Draw(XChart &chart, const nlohmann::json &groupData, std::size_t start, std::size_t end) const override;
protected:
    void ProcessScale(XChart &chart) override;
  private:
    nlohmann::json getRectPoints(nlohmann::json &cfg);
    nlohmann::json getLinePoints(nlohmann::json &cfg);
};
} // namespace geom
} // namespace xg

#endif /* XG_GRAPHICS_GEOM_CANDLE_H */
