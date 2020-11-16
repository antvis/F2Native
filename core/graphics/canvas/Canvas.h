#include "graphics/canvas/CanvasContext.h"
#include "graphics/shape/Group.h"
#include <array>

#ifndef XG_GRAPHICS_CANVAS_H
#define XG_GRAPHICS_CANVAS_H

namespace xg {
namespace canvas {

class Canvas : public shape::Group {
    friend class XChart;

  public:
    Canvas() : Group() {}

    void ChangeSize(double width, double height) {
        // TODO
    }

    void Draw(canvas::CanvasContext &context) {
        if(this->IsDestroyed()) {
            return;
        }
        this->DrawInner(context);
    }

    void Destroy() override { Group::Destroy(); }

  protected:
    // // clear Rect
    // void BeforeDraw();

    // void EndDraw();
};
} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_CANVAS_H
