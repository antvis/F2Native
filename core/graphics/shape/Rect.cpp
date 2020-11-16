#include "graphics/shape/Rect.h"
#include "utils/common.h"
#include <math.h>

BBox xg::shape::Rect::CalculateBox(canvas::CanvasContext &context) const {
    return {static_cast<float>(point_.x), static_cast<float>(point_.x + size_.width), static_cast<float>(point_.y),
            static_cast<float>(point_.y + size_.width)};
}

void xg::shape::Rect::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    if(radius_ <= XG_EPS) {
        context.Rect(point_.x, point_.y, size_.width, size_.height);
        context.ClosePath();
    } else {
        double unitX = std::cos(startAngle_);
        double unitY = std::sin(startAngle_);
        context.MoveTo(unitX * radius0_ + point_.x, unitY * radius0_ + point_.y);
        context.LineTo(unitX * radius_ + point_.x, unitY * radius_ + point_.y);
        if(fabs(endAngle_ - startAngle_) > 0.0001 || (fabs(startAngle_) <= XG_EPS && fabs(endAngle_) <= XG_EPS)) {
            context.Arc(point_.x, point_.y, radius_, startAngle_, endAngle_, false);
            context.LineTo(std::cos(endAngle_) * radius0_ + point_.x, std::sin(endAngle_) * radius0_ + point_.y);
            if(radius0_ >= XG_EPS) {
                context.Arc(point_.x, point_.y, radius0_, endAngle_, startAngle_, true);
            }
        }
        context.ClosePath();
    }
}
