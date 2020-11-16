#include "graphics/shape/Circle.h"

void xg::shape::Circle::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    context.Arc(point_.x, point_.y, radius_, 0.0,2*M_PI);
    context.ClosePath();
}
