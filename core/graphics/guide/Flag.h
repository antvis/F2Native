#ifndef XG_GRAPHICS_GUIDE_FLAG_H
#define XG_GRAPHICS_GUIDE_FLAG_H

#include "graphics/guide/GuideBase.h"
#include "graphics/util/BBox.h"
#include "graphics/util/Point.h"
#include <nlohmann/json.hpp>

namespace xg {
class XChart;

namespace guide {

class Flag : public GuideBase {
  public:
    Flag(nlohmann::json config = {}) : GuideBase("flag", MergeDefaultCfg(config)) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    float GetLineHeight(XChart &chart,
                        shape::Group *container,
                        util::Point &position,
                        float width,
                        float height,
                        bool horizon,
                        bool vertical,
                        float ratio,
                        util::Point &coordCenter,
                        const std::vector<util::Rect> &dangerRects);

  protected:
    static nlohmann::json MergeDefaultCfg(const nlohmann::json &config) {
        nlohmann::json defaultCfg = {
            {"color", "#1890FF"}, // 子元素统一颜色(点/线/方块边框)
            {"textSize", 9.f},
            {"textColor", "#1890FF"},
            {"content", ""},
            {"padding", {0.f, 0.f, 0.f, 0.f}},
            {"lineWidth", 1.0f},              // 旗子延长线线宽
            {"backgroundColor", "#FFFFFF7D"}, // 旗子方块背景色
        };
        if(config.is_object()) {
            defaultCfg.merge_patch(config);
        }
        return defaultCfg;
    }
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_FLAG_H
