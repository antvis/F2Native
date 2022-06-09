//
//  Polygon.cpp
//  F2
//
//  Created by weiqing.twq on 2021/11/12.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#include "Polygon.h"
#include "../util/Path.h"

using namespace xg;
using namespace std;

void xg::shape::Polygon::CreatePath(canvas::CanvasContext &context) const {
    context.BeginPath();
    if(points_.size()) {
        context.SetLineDash(this->dash_);
        auto polygon = [&](const vector<Point> &points, const Point &point, bool reverse) {
            context.MoveTo(points[0].x + point.x, points[0].y + point.y);
            std::size_t len = points_.size();
            if(smooth_ && len > 2) {
                array<Point, 2> constraint = {Point(0, 0), Point(1, 1)};
                BezierPath b_path = util::PathUtil::getSplinePath(points, false, constraint);
                auto lineTo = [&](const BezierPoint &point) {
                    context.BezierCurveTo(point.cp1.x, point.cp1.y, point.cp2.x, point.cp2.y, point.p.x, point.p.y);
                };
                if (reverse) {
                    std::for_each(b_path.rbegin(), b_path.rend(), lineTo);
                }else {
                    std::for_each(b_path.begin(), b_path.end(), lineTo);
                }
                
            } else {
                auto lineTo = [&](const Point &point) { context.LineTo(point.x + point_.x, point.y + point_.y); };
                if (reverse) {
                    std::for_each(points.rbegin() + 1, points.rend(), lineTo);
                }else {
                    std::for_each(points.begin() + 1, points.end(), lineTo);
                }                
            }
            context.ClosePath();
        };
        
        polygon(points_, point_, false);
        
        if (points0_.size()) {
            polygon(points0_, point_, true);
        }
    }
}

BBox xg::shape::Polygon::CalculateBox(canvas::CanvasContext &context) const {
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

