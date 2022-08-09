#ifndef XG_GRAPHICS_GUIDE_LINE_H
#define XG_GRAPHICS_GUIDE_LINE_H

#include "GuideBase.h"

namespace xg {
namespace guide {

struct LineCfg {
    string color = "#1890FF";
    float lineWidth = 1.f;
    string orientation = "horizontal"; // [horizontal, vertical,
    vector<float> dash;
    vector<string> position = {"median", "median"};
    bool top = false;
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(LineCfg)
        FIELDS(FIELD(&LineCfg::color),
               FIELD(&LineCfg::lineWidth),
               FIELD(&LineCfg::orientation),
               FIELD(&LineCfg::dash),
               FIELD(&LineCfg::position),
               FIELD(&LineCfg::top))
        CTORS(DEFAULT_CTOR(LineCfg))
    END_TYPE
#endif
};

class Line : public GuideBase {
  public:
    Line(const LineCfg &cfg) : GuideBase("line"), config_(cfg) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    util::BBox GetBBox() override { return bbox_; }
    bool IsTop() const noexcept override { return config_.top; }
  protected:
    LineCfg config_;
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_LINE_H
