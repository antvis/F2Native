//
//  Image.hpp
//  F2
//
//  Created by weiqing.twq on 2021/9/1.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#ifndef XG_GRAPHICS_GUIDE_IMAGE_H
#define XG_GRAPHICS_GUIDE_IMAGE_H

#include "GuideBase.h"

namespace xg {
namespace guide {

struct ImageCfg {
    vector<float> margin; //left top
    float width, height;
    string src;
    vector<string> position = {"median", "median"};
    bool top = false;
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(ImageCfg)
    FIELDS(FIELD(&ImageCfg::width),
           FIELD(&ImageCfg::height),
           FIELD(&ImageCfg::src),
           FIELD(&ImageCfg::top),
           FIELD(&ImageCfg::margin),
           FIELD(&ImageCfg::position))
    CTORS(DEFAULT_CTOR(ImageCfg))
    END_TYPE
#endif
};

class Image : public GuideBase {
public:
    Image(const ImageCfg &cfg) : GuideBase("image"), config_(cfg) {}
    
    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;
    
    util::BBox GetBBox() override { return bbox_; }
    bool IsTop() const noexcept override { return config_.top; }
protected:
    ImageCfg config_;
};

} // namespace guide
} // namespace xg



#endif /* Image_h */
