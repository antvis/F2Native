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
              const StyleCfg &style) override {
        std::string shape = "circle";
        if(!data._shape.empty()) {
            shape = data._shape;
        }

        util::Point center = {data._x, data._y};

        std::string colorStyle = GLOBAL_COLORS[0];
        if(!data._color.empty()) {
            colorStyle = data._color;
        } else if(!style.fill.empty()){
            colorStyle = style.fill;
        }
        
        std::string strokeColor = "";
        if(!style.stroke.empty()){
            strokeColor = style.stroke;
        }
        
        float radius = 3;
        if(!std::isnan(data._size)) {
            radius = data._size;
        }else if(!std::isnan(style.size)){
            radius = style.size;
        }

        if(shape == "circle") {           
            radius *= context.GetDevicePixelRatio();

            auto circle = std::make_unique<xg::shape::Circle>(center, radius, colorStyle, strokeColor, style.lineWidth);
            container.AddElement(std::move(circle));
        }
    }
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_SHAPE_LINE_H
