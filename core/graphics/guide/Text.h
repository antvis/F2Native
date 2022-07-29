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
    array<float, 2> margin = {0, 0}; // margin: left & top
    string textAlign = "start";
    string textBaseline = "bottom";
    array<string, 2> position;
    bool top = false;
};

extern void from_json(const nlohmann::json &j, TextCfg &c);

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
