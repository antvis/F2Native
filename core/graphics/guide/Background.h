
#ifndef XG_GRAPHICS_GUIDE_BACKGROUND_H
#define XG_GRAPHICS_GUIDE_BACKGROUND_H

#include "GuideBase.h"

namespace xg {
namespace guide {

struct BackgroundCfg {
    string color = "#1CAA3DB2";
    vector<string> leftBottom, rightTop;
    float opacity = 1.0f;
    bool top = false;

#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(BackgroundCfg)
        FIELDS(FIELD(&BackgroundCfg::color),
               FIELD(&BackgroundCfg::leftBottom),
               FIELD(&BackgroundCfg::rightTop),
               FIELD(&BackgroundCfg::opacity),
               FIELD(&BackgroundCfg::top))
        CTORS(DEFAULT_CTOR(BackgroundCfg))
    END_TYPE
#endif
};

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
