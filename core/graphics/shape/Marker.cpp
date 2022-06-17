#include "Marker.h"

void xg::shape::Marker::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    if(symbol_ == "circle") {
        context.Arc(point_.x, point_.y, radius_, 0.0, 2 * M_PI, false);
    }
    if(symbol_ == "square") {
        context.MoveTo(point_.x - radius_, point_.y - radius_);
        context.LineTo(point_.x + radius_, point_.y - radius_);
        context.LineTo(point_.x + radius_, point_.y + radius_);
        context.LineTo(point_.x - radius_, point_.y + radius_);
    }
    context.ClosePath();
}

BBox xg::shape::Marker::CalculateBox(canvas::CanvasContext &context) const {
    BBox bbox;
    bbox.minX = point_.x - radius_;
    bbox.maxX = point_.x + radius_;
    bbox.minY = point_.y - radius_;
    bbox.maxY = point_.y + radius_;
    bbox.x = point_.x;
    bbox.y = point_.y;
    return bbox;
}
