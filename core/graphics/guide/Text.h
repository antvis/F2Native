#ifndef XG_GRAPHICS_GUIDE_TEXT_H
#define XG_GRAPHICS_GUIDE_TEXT_H

#include "GuideBase.h"
#include "../global.h"

namespace xg {
namespace guide {

struct TextCfg {
    string textColor = "#808080";
    float textSize = DEFAULT_FONTSIZE;
    string content = "";
    vector<float> margin = {0, 0}; // margin: left & top
    string textAlign = "start";
    string textBaseline = "bottom";
    vector<string> position = {"median", "median"};
    bool top = false;
#if !defined(__EMSCRIPTEN__)
   BEGIN_TYPE(TextCfg)
       FIELDS(FIELD(&TextCfg::textColor),
              FIELD(&TextCfg::textSize),
              FIELD(&TextCfg::content),
              FIELD(&TextCfg::margin),
              FIELD(&TextCfg::textAlign),
              FIELD(&TextCfg::textBaseline),
              FIELD(&TextCfg::position),
              FIELD(&TextCfg::top))
       CTORS(DEFAULT_CTOR(TextCfg))
   END_TYPE
#endif
};

class Text : public GuideBase {
  public:
    Text(const TextCfg &cfg) : GuideBase("text"), config_(cfg) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    util::BBox GetBBox() override { return bbox_; }
    bool IsTop() const noexcept override { return config_.top; }
  protected:
    TextCfg config_;
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_TEXT_H
