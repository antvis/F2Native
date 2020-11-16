
#ifndef XG_GRAPHICS_SHAPE_MARKER_H
#define XG_GRAPHICS_SHAPE_MARKER_H
#include "graphics/shape/Shape.h"

namespace xg {
namespace shape {

class Marker : public Shape {
  public:
    Marker(const util::Point &center, const float radius, const std::string &fill,const std::string &symbol) : point_(center), radius_(radius), symbol_(symbol){
        type_ = "marker";
        canFill_ = true;
        canStroke_ = true;
        fill_ = fill;
        stroke_ = fill;
    }

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;

  public:
    util::Point point_;
    float radius_;
    string symbol_ = "circle";
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_MARKER_H */

