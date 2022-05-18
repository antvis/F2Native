#ifndef XG_GRAPHICS_GUIDE_FLAG_H
#define XG_GRAPHICS_GUIDE_FLAG_H

#include "GuideBase.h"
#include "../util/BBox.h"
#include "../util/Point.h"
#include "../../nlohmann/json.hpp"

namespace xg {
class XChart;

namespace guide {

class Flag : public GuideBase {
  public:
    Flag(nlohmann::json config = {}) : GuideBase("flag", MergeDefaultCfg(config)) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

  private:
    void DrawFlagCircleAndLine(XChart &chart, shape::Group *container, canvas::CanvasContext &context, util::Point &position);

    void PreDrawFlagContent(XChart &chart,
                            shape::Group *container,
                            canvas::CanvasContext &context,
                            const util::Point &position,
                            const float padding[],
                            const std::vector<util::Rect> &dangerRects);

    void DrawFragContent(XChart &chart, shape::Group *container, canvas::CanvasContext &context, util::Point &position, const float padding[]);

  protected:
    static nlohmann::json MergeDefaultCfg(const nlohmann::json &config) {
        nlohmann::json defaultCfg = {
            {"color", "#1890FF"}, // 子元素统一颜色(点/线/方块边框)
            {"textSize", 9.f},
            {"textColor", "#1890FF"},
            {"textAlign", "start"},
            {"textBaseline", "bottom"},
            {"content", ""},
            {"radius", 1.5f}, //圆心半径
            {"padding", {0.f, 0.f, 0.f, 0.f}},
            {"lineWidth", 1.0f},              // 旗子延长线线宽
            {"backgroundColor", "#FFFFFF7D"}, // 旗子方块背景色
        };
        if(config.is_object()) {
            defaultCfg.merge_patch(config);
        }
        return defaultCfg;
    }

    static bool collide(const util::Rect &r1, const util::Rect &r2) {
        double maxX, maxY, minX, minY;
        maxX = fmax(r1.x + r1.width, r2.x + r2.width);
        maxY = fmax(r1.y + r1.height, r2.y + r2.height);
        minX = fmin(r1.x, r2.x);
        minY = fmin(r1.y, r2.y);
        return (maxX - minX <= r1.width + r2.width && maxY - minY <= r1.height + r2.height);
    }

  private:
    util::Rect contentRect_;
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_FLAG_H
