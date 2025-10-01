//
//  Polygon.hpp
//  F2
//
//  Created by weiqing.twq on 2021/11/12.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#ifndef XG_GRAPHICS_SHAPE_POLYGON_H
#define XG_GRAPHICS_SHAPE_POLYGON_H

#include "Shape.h"

using namespace xg::util;

namespace xg {
namespace shape {

class Polygon : public Shape {
  public:
    //多边形
    Polygon(const float lineWidth, const vector<util::Point> &points, const std::string &strokeColor, const std::string &fillColor, const bool smooth)
    : Shape(), points_(points) {
        lineWidth_ = lineWidth;
        smooth_ = smooth;
        type_ = "polygon";
        SetFillStrokeStyle(strokeColor, fillColor);
    }
    
    Polygon(const float lineWidth, const vector<util::Point> &points, const canvas::CanvasFillStrokeStyle &strokeStyle, canvas::CanvasFillStrokeStyle &fillStyle, const bool smooth)
    : Shape(), points_(points) {
        lineWidth_ = lineWidth;
        smooth_ = smooth;
        type_ = "polygon";
        SetStorkStyle(strokeStyle);
        SetFillStyle(fillStyle);
    }
    
    //镂空多边形
    Polygon(const float lineWidth, const vector<util::Point> &points, const vector<util::Point> &points0, const std::string &strokeColor, const std::string &fillColor, const bool smooth)
    : Shape(), points_(points), points0_(points0) {
        lineWidth_ = lineWidth;
        smooth_ = smooth;
        type_ = "polygon";
        SetFillStrokeStyle(strokeColor, fillColor);
    }

    BBox CalculateBox(canvas::CanvasContext &context) const override;

    void SetDashLine(const std::vector<float> &params) { this->dash_ = params; }

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;
    
  private:
    vector<xg::util::Point> points_;
    vector<xg::util::Point> points0_;//镂空多边形
    bool smooth_ = false;

    std::vector<float> dash_;
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_POLYGON_H */
