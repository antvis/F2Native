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

    Circle(const util::Point &center, const float radius, const std::string &fillColor) : radius_(radius) {
        type_ = "circle";
        point_ = center;

        SetFillColor(fillColor);
    }

    Circle(const util::Point &center, const float radius, const std::string &fillColor, const std::string &strokeColor, float lineWidth): radius_(radius)  {
        type_ = "circle";
        point_ = center;

        SetFillStrokeStyle(strokeColor, fillColor);
        lineWidth_ = lineWidth;
    }

    BBox CalculateBox(canvas::CanvasContext &context) const override;

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;

  public:
    float radius_;
    std::vector<float> dash_;
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_CIRCLE_H */
