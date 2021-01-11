#include "graphics/shape/Polyline.h"
#include "graphics/util/Path.h"

xg::shape::Polyline::Polyline(const float lineWidth, const vector<Point> &points, const string &strokeColor, const string &fillColor, const bool smooth) {
    canFill_ = true;
    canStroke_ = true;
    type_ = "polyline";
    lineWidth_ = lineWidth;
    points_ = points;
    stroke_ = strokeColor;
    fill_ = fillColor;
    smooth_ = smooth;
}

void xg::shape::Polyline::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    if(points_.size()) {
        context.SetLineDash(this->dash_);
        context.MoveTo(points_[0].x + point_.x, points_[0].y + point_.y);
        std::size_t len = points_.size();
        if(smooth_ && len > 2) {
            array<Point, 2> constraint = {Point(0, 0), Point(1, 1)};
            BezierPath b_path = util::PathUtil::getSplinePath(points_, false, constraint);
            // TODO 增加计算曲线的 point 偏移量
            auto lineTo = [&](const BezierPoint &point) {
                context.BezierCurveTo(point.cp1.x, point.cp1.y, point.cp2.x, point.cp2.y, point.p.x, point.p.y);
            };
            std::for_each(b_path.begin(), b_path.end(), lineTo);
        } else {
            auto lineTo = [&](const Point &point) { context.LineTo(point.x + point_.x, point.y + point_.y); };
            std::for_each(points_.begin() + 1, points_.end(), lineTo);
        }
    }
}

BBox xg::shape::Polyline::CalculateBox(canvas::CanvasContext &context) const {
    if(smooth_) {
        // TODO 缺少计算
        return {.0f};
    } else {
        BBox bbox = BBoxUtil::GetBBoxFromPoints(points_, lineWidth_);
        bbox.minX += point_.x;
        bbox.maxX += point_.x;
        bbox.minY += point_.y;
        bbox.maxY += point_.y;
        bbox.x += point_.x;
        bbox.y += point_.y;
        return bbox;
    }
}
