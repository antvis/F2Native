#ifndef XG_GRAPHICS_GUIDE_TEXT_H
#define XG_GRAPHICS_GUIDE_TEXT_H

#include "GuideBase.h"

namespace xg {
namespace guide {
class Text : public GuideBase {
  public:
    Text(nlohmann::json config = {}) : GuideBase("text", MergeDefaultCfg(config)) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    util::BBox GetBBox() override { return bbox_; }

  protected:
    static nlohmann::json MergeDefaultCfg(const nlohmann::json &config) {
        nlohmann::json defaultCfg = {
            {"textColor", "#808080"},  {"textSize", DEFAULT_FONTSIZE},         {"content", ""}, {"margin", {0, 0}}, // margin: left & top
            {"textAlign", "start"}, {"textBaseline", "bottom"},
        };
        if(config.is_object()) {
            defaultCfg.merge_patch(config);
        }
        return defaultCfg;
    }
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_TEXT_H
