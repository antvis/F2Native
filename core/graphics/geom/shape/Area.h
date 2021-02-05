#ifndef GRAPHICS_GEOM_SHAPE_AREA_H
#define GRAPHICS_GEOM_SHAPE_AREA_H

#include "graphics/geom/shape/GeomShapeBase.h"
#include "graphics/global.h"
#include "graphics/shape/Area.h"
#include <vector>

using namespace xg::util;

namespace xg {
namespace geom {
namespace shape {

class Area : public GeomShapeBase {
  public:
    Area() {}

    void Draw(std::string shapeType,
              canvas::coord::AbstractCoord &coord,
              canvas::CanvasContext &context,
              const nlohmann::json &data,
              std::size_t start,
              std::size_t end,
              xg::shape::Group &container) override {
        vector<xg::util::Point> topPoints;
        vector<xg::util::Point> bottomPoints;

        for(std::size_t i = start; i <= end; i++) {
            const nlohmann::json &item = data[i];
            const nlohmann::json &points = item["_points"];
            util::Point bottom{points[0]["x"], points[0]["y"]};
            util::Point top{points[1]["x"], points[1]["y"]};
            top = coord.ConvertPoint(top);
            bottom = coord.ConvertPoint(bottom);

            topPoints.emplace_back(top);
            bottomPoints.emplace_back(bottom);
        }
        std::reverse(bottomPoints.begin(), bottomPoints.end());

        std::size_t size = end - start + 1;
        if(size > 0 && data[start].contains("_shape")) {
            shapeType = data[start]["_shape"];
        }
        bool smooth = shapeType == "smooth";
        auto l = xg::make_unique<xg::shape::Area>(topPoints, bottomPoints, smooth);

        if(size <= 0) {
            l->fillStyle_ = util::CanvasFillStrokeStyle(GLOBAL_COLORS[0]);
        } else {
            l->fillStyle_ = util::ColorParser(data[start], "_color");
        }
        container.AddElement(std::move(l));
    }
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif
