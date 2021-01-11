
#ifndef XG_GRAPHICS_SHAPE_MARKER_H
#define XG_GRAPHICS_SHAPE_MARKER_H
#include "graphics/shape/Shape.h"

namespace xg {
namespace shape {

class Marker : public Shape {
  public:
    Marker(const util::Point &center, const float radius, const std::string &fill, const std::string &symbol)
        : radius_(radius), symbol_(symbol) {
        type_ = "marker";
        point_ = center;
        canFill_ = true;
        canStroke_ = true;
        fill_ = fill;
        stroke_ = fill;
    }

    BBox CalculateBox(canvas::CanvasContext &context) const override;

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;

  public:
    float radius_;
    string symbol_ = "circle";
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_MARKER_H */
