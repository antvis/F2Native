
#include "graphics/shape/Marker.h"

void xg::shape::Marker::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    // point_ 为marker的中心
    if (symbol_ == "circle") {
        context.Arc(point_.x, point_.y, radius_, 0.0, 2 * M_PI, false);
    } else if (symbol_ == "square") {
        context.MoveTo(point_.x - radius_, point_.y - radius_);
        context.LineTo(point_.x + radius_, point_.y - radius_);
        context.LineTo(point_.x + radius_, point_.y + radius_);
        context.LineTo(point_.x - radius_, point_.y + radius_);
    } else if (symbol_ == "rect") {
        context.MoveTo(point_.x - size_.width / 2, point_.y - size_.height / 2);
        context.LineTo(point_.x + size_.width / 2, point_.y - size_.height / 2);
        context.LineTo(point_.x + size_.width / 2, point_.y + size_.height / 2);
        context.LineTo(point_.x - size_.width / 2, point_.y + size_.height / 2);
    }
    context.ClosePath();
}

BBox xg::shape::Marker::CalculateBox(canvas::CanvasContext &context) const {
    BBox bbox;
    if (symbol_ == "rect") {
        bbox.minX = point_.x - size_.width / 2;
        bbox.maxX = point_.x + size_.width / 2;
        bbox.minY = point_.y - size_.height / 2;
        bbox.maxY = point_.y + size_.height / 2;
    } else {
        bbox.minX = point_.x - radius_;
        bbox.maxX = point_.x + radius_;
        bbox.minY = point_.y - radius_;
        bbox.maxY = point_.y + radius_;
    }
    bbox.x = point_.x;
    bbox.y = point_.y;
    return bbox;
}