#include "graphics/guide/GuideBase.h"
#include "graphics/shape/Group.h"

#ifndef XG_GRAPHICS_GUIDE_CONTROLLER_H
#define XG_GRAPHICS_GUIDE_CONTROLLER_H

namespace xg {
class XChart;

namespace guide {

class GuideController {
  public:
    GuideController(shape::Group *container) : container_(container) {}

    ~GuideController() {
        Clear();
        container_ = nullptr;
    }

    // 分时图小旗子
    void Flag(const std::string &json = "");

    // 文字
    void Text(const std::string &json = "");

    // 线
    void Line(const std::string &json = "");

    void Clear() {
        this->container_->Clear();
        this->guides.clear();
        this->dangerRects.clear();
    }

    void Render(XChart &chart, canvas::CanvasContext &context);

  private:
    std::vector<std::unique_ptr<GuideBase>> guides;
    std::vector<util::Rect> dangerRects = {};
    shape::Group *container_ = nullptr;
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_CONTROLLER_H
