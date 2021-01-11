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
    context.MoveTo(p1_.x + this->point_.x, p1_.y + this->point_.y);
    context.LineTo(p2_.x + this->point_.x, p2_.y + this->point_.y);
}

BBox xg::shape::Line::CalculateBox(canvas::CanvasContext &context) const {
    return BBoxUtil::GetBBoxFromLine(p1_.x + this->point_.x, p1_.y + this->point_.y, p2_.x + this->point_.x,
                                     p2_.y + this->point_.y, lineWidth_);
}
