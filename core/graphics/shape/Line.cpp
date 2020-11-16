#include "graphics/shape/Line.h"

xg::shape::Line::Line(const Point &pt1, const Point &pt2, const float lineWidth, const string &strokeColor) : Shape() {
    p1_ = pt1;
    p2_ = pt2;
    lineWidth_ = lineWidth;
    stroke_ = strokeColor;
    type_ = "line";
    canStroke_ = true;
}

void xg::shape::Line::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    context.MoveTo(p1_.x, p1_.y);
    context.LineTo(p2_.x, p2_.y);
}

BBox xg::shape::Line::CalculateBox(canvas::CanvasContext &context) const {
    return BBoxUtil::GetBBoxFromLine(p1_.x, p1_.y, p2_.x, p2_.y, lineWidth_);
}
