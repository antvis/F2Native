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
    array<float, 2> margin;
    float width, height;
    string src;
    array<string, 2> position;
    bool top = false;
};

extern void from_json(const nlohmann::json &j, ImageCfg &i);

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
