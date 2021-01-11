#include "GeomShapeBase.h"
#include "graphics/shape/Circle.h"

#ifndef XG_GRAPHICS_GEOM_SHAPE_POINT_H
#define XG_GRAPHICS_GEOM_SHAPE_POINT_H

namespace xg {
namespace geom {
namespace shape {
class Point : public GeomShapeBase {
  public:
    Point() {}
    void Draw(std::string shapeType,
              canvas::coord::AbstractCoord &coord,
              canvas::CanvasContext &context,
              const nlohmann::json &data,
              xg::shape::Group &container) override {
        std::string shape = "circle";
        if(data.contains("_shape")) {
            shape = data["_shape"];
        }

        util::Point center = {data["_x"], data["_y"]};
        const nlohmann::json &style = data["_style"];
        if(shape == "circle") {
            float radius = 1;
            if(data.contains("_size")) {
                radius = data["_size"].get<float>() * context.GetDevicePixelRatio();
            } else {
                radius = style["radius"].get<float>() * context.GetDevicePixelRatio();
            }
            std::string color = style["color"];
            auto circle = std::make_unique<xg::shape::Circle>(center, radius, color);
            container.AddElement(std::move(circle));
        }
    }
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_SHAPE_LINE_H
