#ifndef XG_GRAPHICS_SHAPE_AREA_H
#define XG_GRAPHICS_SHAPE_AREA_H

#include "graphics/shape/Shape.h"

namespace xg {
namespace shape {

class Area : public Shape {
  public:
    Area(const vector<util::Point> &topPoints, const vector<util::Point> &bottomPoints, const bool smooth);

    BBox CalculateBox(canvas::CanvasContext &context) const override;

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;

  public:
    vector<xg::util::Point> topPoints_;
    vector<xg::util::Point> bottomPoints_;
    vector<xg::util::Point> mergePoints_;
    bool smooth_ = false;
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_AREA_H */
