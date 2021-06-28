
#ifndef XG_GRAPHICS_GUIDE_BACKGROUND_H
#define XG_GRAPHICS_GUIDE_BACKGROUND_H

#include "GuideBase.h"

namespace xg {
namespace guide {

class Background : public GuideBase {
  public:
    Background(nlohmann::json config = {}) : GuideBase("background", MergeDefaultCfg(config)) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;
    util::BBox GetBBox() override { return bbox_; }

  protected:
    static nlohmann::json MergeDefaultCfg(const nlohmann::json &cfg) {
        nlohmann::json defaultCfg = {{"color", "#1CAA3DB2"}};
        if(cfg.is_object()) {
            defaultCfg.merge_patch(cfg);
        }
        return defaultCfg;
    }
};

} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_BACKGROUND_H
