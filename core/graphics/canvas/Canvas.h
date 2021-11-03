#include "graphics/canvas/CanvasContext.h"
#include "graphics/shape/Group.h"
#include <array>

#ifndef XG_GRAPHICS_CANVAS_H
#define XG_GRAPHICS_CANVAS_H

namespace xg {
class XChart;
namespace canvas {

class Canvas : public shape::Group {
    friend class XChart;

  public:
    Canvas(XChart *chart) : Group(), chart_(chart) {}

    void ChangeSize(double x, double y, double width, double height) {
        origin = {x, y};
        size = {width, height};
    }

    void Draw();

    void Destroy() override {
        Group::Destroy();
        chart_ = nullptr;
    }
    
    void Clear() { animateHandler_ = false; }

    virtual ~Canvas() { chart_ = nullptr; }

  protected:
    // // clear Rect
    // void BeforeDraw();

    // void EndDraw();

  private:
    void _beginDraw() { toDraw_ = true; }

    void _endDraw() { toDraw_ = false; }

    void innerDrawInner();

  private:
    bool animateHandler_ = false;
    bool toDraw_ = false;
    XChart *chart_ = nullptr;
    util::Point origin{0, 0};
    util::Size size{0, 0};
};
} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_CANVAS_H
