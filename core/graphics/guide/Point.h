//
// Created by luna on 2023/8/14.
//

#ifndef CHART_NATIVE_CORE_POINT_H
#define CHART_NATIVE_CORE_POINT_H

#include "GuideBase.h"

namespace xg {
namespace guide {
class Point : public GuideBase {
  public:
    Point(nlohmann::json config = {}) : GuideBase("point", MergeDefaultCfg(config)) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    util::BBox GetBBox() override { return bbox_; }

  protected:
    static nlohmann::json MergeDefaultCfg(const nlohmann::json &config) {
        nlohmann::json defaultCfg = {
                {"size", 6.f},    {"margin", {0, 0}}, // margin: left & top
                {"shape", "circle"}, // 'circle', 'rect'
                {"lineWidth", 1.f}
        };
        if(config.is_object()) {
            defaultCfg.merge_patch(config);
        }
        return defaultCfg;
    }
};

} // namespace guide
} // namespace xg



#endif //CHART_NATIVE_CORE_POINT_H
