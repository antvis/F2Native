#ifndef XG_GRAPHICS_GEOM_SHAPE_POINT_H
#define XG_GRAPHICS_GEOM_SHAPE_POINT_H

#include "GeomShapeBase.h"
#include "../../func/Func.h"
#include "../../shape/Circle.h"

namespace xg {
namespace geom {
namespace shape {
class Point : public GeomShapeBase {
  public:
    Point() {}
    void Draw(std::string shapeType,
              canvas::coord::AbstractCoord &coord,
              canvas::CanvasContext &context,
              const XData &data,
              std::size_t start,
              std::size_t end,
              xg::shape::Group &container,
              bool connectNulls) override {
        std::string shape = "circle";
        if(!data._shape.empty()) {
            shape = data._shape;
        }

        util::Point center = {data._x, data._y};
        nlohmann::json style = data._style;

        std::string colorStyle = GLOBAL_COLORS[0];
        if(style.contains("color")) {
            colorStyle = style["color"];
        } else if(!data._color.empty()) {
            colorStyle = data._color;
        }

        if(shape == "circle") {
            float radius = 3;
            if(style.contains("size")) {
                radius = style["size"].get<float>();
            } else if(!std::isnan(data._size)) {
                radius = data._size;
            }
            radius *= context.GetDevicePixelRatio();

            auto circle = std::make_unique<xg::shape::Circle>(center, radius);
            // TODO 兼容 stroke 模式 & linear or radial 模式
            circle->SetFillColor(colorStyle);
            container.AddElement(std::move(circle));
        }
    }
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_SHAPE_LINE_H
