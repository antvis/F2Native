#ifndef XG_GRAPHICS_SHAPE_CIRCLE_H
#define XG_GRAPHICS_SHAPE_CIRCLE_H

#include "graphics/shape/Shape.h"

namespace xg {
namespace shape {

class Circle : public Shape {
    public:
    Circle(const util::Point &center, const float radius, std::string fill) : point_(center), radius_(radius) {
        type_ = "circle";
        canFill_ = true;
        canStroke_ = true;
        fill_ = fill;
        stroke_ = fill;
    }

    //    BBox CalculateBox(canvas::CanvasContext &context) const override;

    protected:
    void CreatePath(canvas::CanvasContext &context) const override;

    public:
    util::Point point_;
    float radius_;
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_CIRCLE_H */
