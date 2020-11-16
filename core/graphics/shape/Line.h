#ifndef XG_GRAPHICS_SHAPE_LINE_H
#define XG_GRAPHICS_SHAPE_LINE_H

#include "graphics/shape/Shape.h"

namespace xg {
namespace shape {

class Line : public Shape {
    public:
    Line(const util::Point &, const util::Point &, const float lineWidth, const string &strokeColor);

    BBox CalculateBox(canvas::CanvasContext &context) const override;

    protected:
    void CreatePath(canvas::CanvasContext &context) const override;

    public:
    util::Point p1_, p2_;
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_LINE_H */
