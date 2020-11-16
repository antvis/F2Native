
#include "graphics/shape/Marker.h"


void xg::shape::Marker::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    if (symbol_ == "circle") {
        context.Arc(point_.x, point_.y, radius_, 0.0,2*M_PI,false);
    }
    if (symbol_ == "square") {
        context.MoveTo(point_.x - radius_, point_.y - radius_);
        context.LineTo(point_.x + radius_, point_.y - radius_);
        context.LineTo(point_.x + radius_, point_.y + radius_);
        context.LineTo(point_.x - radius_, point_.y + radius_);
    }
    context.ClosePath();
}
