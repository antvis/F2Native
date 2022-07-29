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

namespace xg {
namespace guide {

struct PointCfg {
    size_t size = 3;
    string shape = "circle";
    string fill = GLOBAL_COLORS[0];
    string stroke;
    float lineWidth;
    array<string, 2> position;
    bool top = false;
    array<float, 2> margin = {0, 0};//left
};

extern void from_json(const nlohmann::json &j, PointCfg &c);

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
