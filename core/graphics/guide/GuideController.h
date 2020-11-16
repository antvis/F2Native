#ifndef XG_GRAPHICS_GUIDE_CONTROLLER_H
#define XG_GRAPHICS_GUIDE_CONTROLLER_H

#include "graphics/guide/Flag.h"
#include "graphics/guide/GuideBase.h"
#include "graphics/guide/Text.h"
#include "graphics/shape/Group.h"
#include "graphics/util/Point.h"
#include <nlohmann/json.hpp>
#include <utils/common.h>

namespace xg {
class XChart;

namespace guide {

class GuideController {
  public:
    GuideController() {}

    ~GuideController() { guides.clear(); }

    // 分时图小旗子
    void Flag(nlohmann::json config = {}) {
        auto guide = xg::make_unique<xg::guide::Flag>(config);
        this->guides.push_back(std::move(guide));
    }

    // 文字
    void Text(nlohmann::json config = {}) {
        auto text = xg::make_unique<xg::guide::Text>(config);
        this->guides.push_back(std::move(text));
    }

    void Clear() { this->guides.clear(); }

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context);

  private:
    std::vector<std::unique_ptr<GuideBase>> guides;
    std::vector<util::Rect> dangerRects = {};
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_CONTROLLER_H
