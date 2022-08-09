#include <math.h>
#include "Rect.h"
#include "../../utils/common.h"

BBox xg::shape::Rect::CalculateBox(canvas::CanvasContext &context) const {
    return {static_cast<float>(point_.x), static_cast<float>(point_.x + size_.width), static_cast<float>(point_.y),
            static_cast<float>(point_.y + size_.height)};
}

void xg::shape::Rect::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    if(radius_ <= XG_EPS) {
        if(this->HasRounding()) {
            float height = static_cast<float>(size_.height);//height < 0 柱朝上
            float width = static_cast<float>(size_.width);
            float x = static_cast<float>(point_.x);
            float y = static_cast<float>(point_.y);
            context.MoveTo(x + roundings_[2], y);
            context.LineTo(x + width - roundings_[3], y);
            if (height < 0) {
                context.QuadraticCurveTo(x + width, y, x + width, y - roundings_[3]);
                context.LineTo(x + width, y + height + roundings_[1]);
            }else {
                context.QuadraticCurveTo(x + width, y, x + width, y + roundings_[3]);
                context.LineTo(x + width, y + height - roundings_[1]);
            }
            context.QuadraticCurveTo(x + width, y + height, x + width - roundings_[1], y + height);
            
            context.LineTo(x + roundings_[0], y + height);
            if (height < 0) {
                context.QuadraticCurveTo(x, y + height, x, y + height + roundings_[0]);
                context.LineTo(x, y - roundings_[2]);
            }else {
                context.QuadraticCurveTo(x, y + height, x, y + height - roundings_[0]);
                context.LineTo(x, y + roundings_[2]);
            }
            context.QuadraticCurveTo(x, y, x + roundings_[2], y);
        } else {
            context.Rect(point_.x, point_.y, size_.width, size_.height);
        }
        context.ClosePath();
    } else {
        double unitX = std::cos(startAngle_);
        double unitY = std::sin(startAngle_);
        context.SetLineDash(this->dash_);
        context.MoveTo(unitX * radius0_ + point_.x, unitY * radius0_ + point_.y);
        context.LineTo(unitX * radius_ + point_.x, unitY * radius_ + point_.y);
        if(fabs(endAngle_ - startAngle_) > 0.0001 || (fabs(startAngle_) <= XG_EPS && fabs(endAngle_) <= XG_EPS)) {
            context.Arc(point_.x, point_.y, radius_, startAngle_, endAngle_, false);
            context.LineTo(std::cos(endAngle_) * radius0_ + point_.x, std::sin(endAngle_) * radius0_ + point_.y);
            if(radius0_ >= XG_EPS) {
                context.Arc(point_.x, point_.y, radius0_, endAngle_, startAngle_, true);
            }
        }
    }
}

void xg::shape::Rect::UpdateAttribute(std::string attrName, double val) {
    xg::shape::Shape::UpdateAttribute(attrName, val);
    if(attrName == "width") {
        size_.width = val;
    } else if(attrName == "height") {
        size_.height = val;
    } else if(attrName == "endAngle") {
        this->endAngle_ = val;
    }
}

void xg::shape::Rect::SetRoundings(std::array<float, 4> roundings) {
    float height = static_cast<float>(size_.height);//height < 0 柱朝上
    float width = static_cast<float>(size_.width);
    float minWidth = max(roundings[0] + roundings[1], roundings[2] + roundings[3]);
    float minHeight = max(roundings[0] + roundings[3], roundings[1] + roundings[2]);
    float scale = fabs(min(width / minWidth, height / minHeight));
    roundings_ = roundings;
    if (scale < 1) {
        roundings_[0] *= scale;
        roundings_[1] *= scale;
        roundings_[2] *= scale;
        roundings_[3] *= scale;
    }
}

void xg::shape::Rect::SetRoundings(const std::vector<float> &roundings) {
    float height = static_cast<float>(size_.height);//height < 0 柱朝上
    float width = static_cast<float>(size_.width);
    float minWidth = max(roundings[0] + roundings[1], roundings[2] + roundings[3]);
    float minHeight = max(roundings[0] + roundings[3], roundings[1] + roundings[2]);
    float scale = fabs(min(width / minWidth, height / minHeight));
    scale = min(scale, 1.f);
    roundings_[0] *= scale;
    roundings_[1] *= scale;
    roundings_[2] *= scale;
    roundings_[3] *= scale;
}
