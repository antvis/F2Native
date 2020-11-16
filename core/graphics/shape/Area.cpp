#include "graphics/shape/Area.h"
#include "graphics/util/Path.h"

xg::shape::Area::Area(const vector<util::Point> &topPoints, const vector<util::Point> &bottomPoints, const string &fillColor, const bool smooth) {
    canStroke_ = false;
    canFill_ = true;
    fill_ = fillColor;
    type_ = "area";
    fillOpacity_ = 0.1;
    smooth_ = smooth;

    topPoints_ = topPoints;
    bottomPoints_ = bottomPoints;
    mergePoints_.reserve(topPoints_.size() + bottomPoints_.size());
    mergePoints_.insert(mergePoints_.end(), topPoints_.begin(), topPoints_.end());
    mergePoints_.insert(mergePoints_.end(), bottomPoints_.begin(), bottomPoints_.end());
}

void xg::shape::Area::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    if(mergePoints_.size()) {
        context.MoveTo(mergePoints_[0].x, mergePoints_[0].y);
        std::size_t len = mergePoints_.size();
        if(smooth_ && len > 2) {
            array<Point, 2> constraint = {Point(0, 0), Point(1, 1)};
            BezierPath b_path = util::PathUtil::getSplinePath(mergePoints_, false, constraint);
            auto lineTo = [&](const BezierPoint &point) {
                context.BezierCurveTo(point.cp1.x, point.cp1.y, point.cp2.x, point.cp2.y, point.p.x, point.p.y);
            };
            std::for_each(b_path.begin(), b_path.end(), lineTo);
        } else {
            auto lineTo = [&](const Point &point) { context.LineTo(point.x, point.y); };
            std::for_each(mergePoints_.begin() + 1, mergePoints_.end(), lineTo);
        }
        context.ClosePath();
    }
}

BBox xg::shape::Area::CalculateBox(canvas::CanvasContext &context) const {
    return BBoxUtil::GetBBoxFromPoints(mergePoints_, .0f);
}
