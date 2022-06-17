#include "Circle.h"

void xg::shape::Circle::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    context.Arc(point_.x, point_.y, radius_, 0.0, 2 * M_PI);
    context.ClosePath();
}

BBox xg::shape::Circle::CalculateBox(canvas::CanvasContext &context) const {
    BBox bbox;
    bbox.minX = point_.x - radius_;
    bbox.maxX = point_.x + radius_;
    bbox.minY = point_.y - radius_;
    bbox.maxY = point_.y + radius_;
    bbox.x = point_.x;
    bbox.y = point_.y;
    return bbox;
}
