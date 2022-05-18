#ifndef XG_GRAPHICS_GUIDE_BASE_H
#define XG_GRAPHICS_GUIDE_BASE_H

#include "../canvas/CanvasContext.h"
#include "../shape/Group.h"
#include "../util/BBox.h"
#include "../util/Point.h"
#include "../../nlohmann/json.hpp"

namespace xg {
class XChart;

namespace guide {

class GuideBase {
  public:
    GuideBase(std::string type, nlohmann::json config = {}) : type_(type), config_(config) {}
    virtual ~GuideBase() {}
    virtual void
    Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) = 0;

    virtual util::BBox GetBBox() { return bbox_; }

    std::string GetType() const noexcept { return this->type_; }
    
    //是否绘制在最顶层
    bool isTop() const noexcept {
        if (config_.contains("top")) {
            return config_["top"].get<bool>();
        }
        return true;
    }

    virtual util::Point GetPosition(XChart &chart, const nlohmann::json &position, const std::string &xField, const std::string &yField);

  protected:
    std::string type_ = "";
    nlohmann::json config_;
    util::BBox bbox_;
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_BASE_H
