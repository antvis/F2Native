#include "graphics/canvas/CanvasContext.h"
#include "graphics/shape/Group.h"
#include "graphics/util/BBox.h"
#include "graphics/util/Point.h"
#include <nlohmann/json.hpp>

#ifndef XG_GRAPHICS_GUIDE_BASE_H
#define XG_GRAPHICS_GUIDE_BASE_H

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

    virtual util::Point GetPosition(XChart &chart, const std::string &xField, const std::string &yField);

  protected:
    std::string type_ = "";
    nlohmann::json config_;
    util::BBox bbox_;
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_BASE_H
