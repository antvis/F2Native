

#include "graphics/guide/GuideController.h"
#include "graphics/util/BBox.h"

void xg::guide::GuideController::Render(xg::XChart &chart, xg::shape::Group *container, canvas::CanvasContext &context) {
    std::for_each(guides.begin(), guides.end(), [&](const std::unique_ptr<xg::guide::GuideBase> &guide) -> void {
        guide->Render(chart, container, context, this->dangerRects);
        util::BBox bbox = guide->GetBBox();
        if(bbox.width > 0 && bbox.height > 0) {
            util::Rect rect(bbox.x, bbox.y, bbox.width, bbox.height);
            this->dangerRects.push_back(std::move(rect));
        }
    });
}
