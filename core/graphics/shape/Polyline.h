#ifndef XG_GRAPHICS_SHAPE_POLYLINE_H
#define XG_GRAPHICS_SHAPE_POLYLINE_H

#include "Shape.h"

using namespace xg::util;

namespace xg {
namespace shape {

class Polyline : public Shape {
  public:
    Polyline(const float lineWidth, const vector<util::Point> &, const bool smooth);

    Polyline(const float lineWidth, const vector<util::Point> &, const std::string &strokeColor, const std::string &fillColor, const bool smooth);

    BBox CalculateBox(canvas::CanvasContext &context) const override;

    void SetDashLine(const std::vector<float> &params) { this->dash_ = params; }

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;

  private:
    vector<xg::util::Point> points_;
    bool smooth_ = false;

    std::vector<float> dash_;
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_POLYLINE_H */
