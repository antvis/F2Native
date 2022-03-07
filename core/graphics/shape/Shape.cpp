#include "graphics/shape/Shape.h"

xg::shape::Shape::Shape() : Element() { isShape_ = true; }

void xg::shape::Shape::DrawInner(canvas::CanvasContext &context) const {
    float originOpacity = context.GlobalAlpha();
    //coregraphics context path和fill必须配对 path和stroke必须配对
    //在既有fill又有stroke的case 会浪费一点性能
    
    // fill case
    if(HasFill()) {
        CreatePath(context);
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
        CreatePath(context);
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
