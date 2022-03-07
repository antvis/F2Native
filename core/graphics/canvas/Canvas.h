#include "graphics/canvas/CanvasContext.h"
#include "graphics/shape/Group.h"

#ifndef XG_GRAPHICS_CANVAS_H
#define XG_GRAPHICS_CANVAS_H

namespace xg {
namespace canvas {

class Canvas : public shape::Group {
  public:
    Canvas() : Group() {}

    void ChangeSize(double x, double y, double width, double height) {
        origin = {x, y};
        size = {width, height};
    }

    void Draw(canvas::CanvasContext &context);

    void Destroy() override {
        Group::Destroy();
    }

  private:
    util::Point origin{0, 0};
    util::Size size{0, 0};
};
} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_CANVAS_H
