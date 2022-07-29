
#ifndef XG_GRAPHICS_GUIDE_BACKGROUND_H
#define XG_GRAPHICS_GUIDE_BACKGROUND_H

#include "GuideBase.h"

namespace xg {
namespace guide {

struct BackgroundCfg {
    string color = "#1CAA3DB2";
    array<string, 2> leftBottom, rightTop;
    bool top = false;
};
extern void from_json(const nlohmann::json &j, BackgroundCfg &b);


class Background : public GuideBase {
  public:
    Background(const BackgroundCfg &cfg) : GuideBase("background"), config_(cfg) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;
    util::BBox GetBBox() override { return bbox_; }
    bool IsTop() const noexcept override { return config_.top; }

  protected:
    BackgroundCfg config_;
};

} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_BACKGROUND_H
