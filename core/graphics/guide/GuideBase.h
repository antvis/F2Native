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
    GuideBase(const std::string &type) : type_(type) {}
    virtual ~GuideBase() {}
    virtual void
    Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) = 0;

    virtual util::BBox GetBBox() { return bbox_; }

    std::string GetType() const noexcept { return this->type_; }

    virtual bool IsTop() const noexcept = 0;
    
    virtual util::Point GetPosition(XChart &chart, const array<string, 2> &position, const std::string &xField, const std::string &yField);

  protected:
    std::string type_ = "";
    util::BBox bbox_;
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_BASE_H
