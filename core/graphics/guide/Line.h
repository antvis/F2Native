#ifndef XG_GRAPHICS_GUIDE_LINE_H
#define XG_GRAPHICS_GUIDE_LINE_H

#include "GuideBase.h"

namespace xg {
namespace guide {

class Line : public GuideBase {
  public:
    Line(nlohmann::json cfg = {}) : GuideBase("line", MergeDefaultCfg(cfg)) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    util::BBox GetBBox() override { return bbox_; }

  protected:
    static nlohmann::json MergeDefaultCfg(const nlohmann::json &config) {
        nlohmann::json defaultCfg = {
            {"color", "#1890FF"}, {"lineWidth", 1.f}, {"orientation", "horizontal"} // [horizontal, vertical, crossed]
        };
        if(config.is_object()) {
            defaultCfg.merge_patch(config);
        }
        return defaultCfg;
    }
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_LINE_H
