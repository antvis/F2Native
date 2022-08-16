#ifndef XG_GRAPHICS_SHAPE_CIRCLE_H
#define XG_GRAPHICS_SHAPE_CIRCLE_H

#include "Shape.h"

namespace xg {
namespace shape {

class Circle : public Shape {
  public:
    Circle(const util::Point &center, const float radius) : radius_(radius) {
        type_ = "circle";
        point_ = center;
    }

    Circle(const util::Point &center, const float radius, const std::string &fill, const std::string &stroke, const float lineWidth) : radius_(radius) {
        type_ = "circle";
        point_ = center;
        lineWidth_ = lineWidth;

        SetFillColor(fill);
        SetStorkColor(stroke);
    }
    
    Circle(const util::Point &center, const float radius, const canvas::CanvasFillStrokeStyle &fill, const canvas::CanvasFillStrokeStyle &stroke, const float lineWidth) : radius_(radius) {
        type_ = "circle";
        point_ = center;
        lineWidth_ = lineWidth;

        SetFillStyle(fill);
        SetStorkStyle(stroke);
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
