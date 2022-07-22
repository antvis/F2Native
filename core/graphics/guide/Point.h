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

class Point : public GuideBase {
  public:
    Point(const nlohmann::json &config = {}) : GuideBase("point", MergeDefaultCfg(config)) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    util::BBox GetBBox() override { return bbox_; }

  protected:
    static nlohmann::json MergeDefaultCfg(const nlohmann::json &config) {
        nlohmann::json defaultCfg = {
            {"size", 3},  {"shape", "circle"}, {"fill", GLOBAL_COLORS[0]}, {"offsetX", 0}, {"offsetY", 0},
        };
        if(config.is_object()) {
            defaultCfg.merge_patch(config);
        }
        return defaultCfg;
    }
};
} // namespace guide
}

#endif /* XG_GRAPHICS_GUIDE_POINT_H */
