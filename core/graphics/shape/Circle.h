#ifndef XG_GRAPHICS_SHAPE_CIRCLE_H
#define XG_GRAPHICS_SHAPE_CIRCLE_H

#include "graphics/shape/Shape.h"

namespace xg {
namespace shape {

class Circle : public Shape {
  public:
    Circle(const util::Point &center, const float radius) : radius_(radius) {
        type_ = "circle";
        point_ = center;
    }

    Circle(const util::Point &center, const float radius, const std::string &fill) : radius_(radius) {
        type_ = "circle";
        point_ = center;

        SetFillColor(fill);
    }

    BBox CalculateBox(canvas::CanvasContext &context) const override;

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;

  public:
    float radius_;
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_CIRCLE_H */
