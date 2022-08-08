//
//  Circle.hpp
//  F2
//
//  Created by weiqing.twq on 2021/9/9.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#ifndef XG_GRAPHICS_GUIDE_POINT_H
#define XG_GRAPHICS_GUIDE_POINT_H

#include "GuideBase.h"
#include "../../reflection/reflection.h"

namespace xg {
namespace guide {

struct PointCfg {
    float size = 3;
    string shape = "circle";
    string fill = GLOBAL_COLORS[0];
    string stroke;
    float lineWidth;
    bool top = false;
    vector<string> position = {"median", "median"};
    vector<float> margin = {0, 0};//left top
    
    BEGIN_TYPE(PointCfg)
        FIELDS(FIELD(&PointCfg::size),
               FIELD(&PointCfg::shape),
               FIELD(&PointCfg::fill),
               FIELD(&PointCfg::stroke),
               FIELD(&PointCfg::lineWidth),
               FIELD(&PointCfg::top),
               FIELD(&PointCfg::margin),
               FIELD(&PointCfg::position))
        CTORS(DEFAULT_CTOR(PointCfg))
    END_TYPE
};

class Point : public GuideBase {
  public:
    Point(const PointCfg &config) : GuideBase("point"), config_(config) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    util::BBox GetBBox() override { return bbox_; }
    bool IsTop() const noexcept override { return config_.top; }
  protected:
    PointCfg config_;
};
} // namespace guide
}

#endif /* XG_GRAPHICS_GUIDE_POINT_H */
