#include "Canvas.h"

using namespace xg;

void canvas::Canvas::Draw(canvas::CanvasContext &context) {
    if (this->IsDestroyed()) {
        return;
    }

    context.ClearRect(origin.x, origin.y, size.width, size.height);

    this->DrawInner(context);
}
