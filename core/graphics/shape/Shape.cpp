#include "graphics/shape/Shape.h"

xg::shape::Shape::Shape() : Element() { isShape_ = true; }

void xg::shape::Shape::DrawInner(canvas::CanvasContext &context) const {
    CreatePath(context);

    float originOpacity = context.GlobalAlpha();
    // fill case
    if(HasFill()) {
        if(!std::isnan(fillOpacity_)) {
            context.SetGlobalAlpha(fillOpacity_);
            context.Fill();
            context.SetGlobalAlpha(originOpacity);
        } else {
            context.Fill();
        }
    }

    // stroke case
    if(HasStroke() && lineWidth_ > 0) {
        if(!std::isnan(strokeOpacity_)) {
            context.SetGlobalAlpha(strokeOpacity_);
            context.Stroke();
            context.SetGlobalAlpha(originOpacity);
        } else {
            context.Stroke();
        }
    }
}

const BBox &xg::shape::Shape::GetBBox(canvas::CanvasContext &context) {
    if(std::isnan(bbox_.minX)) {
        bbox_ = CalculateBox(context);
    }
    return bbox_;
}
