#include "Canvas.h"
#include "graphics/XChart.h"

using namespace xg;

void canvas::Canvas::innerDrawInner() {
    func::Command *c = func::CreateCommand([&]() -> void {
        animateHandler_ = false;
        if(toDraw_) {
            this->innerDrawInner();
        }
    });
    chart_->RequestAnimationFrame(c);
    animateHandler_ = true;

    chart_->GetLogTracer()->trace("%s", "beforeDraw and call canvas clearRect");
    chart_->GetCanvasContext().ClearRect(origin.x, origin.y, size.width, size.height);

    this->DrawInner(chart_->GetCanvasContext());

    this->_endDraw();
}

void canvas::Canvas::Draw() {
    if(this->IsDestroyed()) {
        return;
    }

    if(this->animateHandler_) {
        this->_beginDraw();
    } else {
        innerDrawInner();
    }
}
