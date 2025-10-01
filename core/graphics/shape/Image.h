//
//  Image.hpp
//  F2
//
//  Created by weiqing.twq on 2021/9/1.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#ifndef XG_GRAPHICS_SHAPE_IMAGE_H
#define XG_GRAPHICS_SHAPE_IMAGE_H

#include "Shape.h"
#include "../canvas/CanvasImage.h"

namespace xg {
namespace shape {

class Image : public Shape {
  public:
    Image() = delete;
    
    Image(const std::string &url, const util::Point &point, const util::Size &size): url_(url), point_(point), size_(size) {
        type_ = "image";
        image_ = std::make_unique<canvas::CanvasImage>(url);
    };
    
    void OnLoad(std::function<void()> finished);
    
    ~Image() = default;

    BBox CalculateBox(canvas::CanvasContext &context) const override;
    
    inline canvas::CanvasImage *GetImage() { return image_.get(); }

    inline void SetPoint(const util::Point &pt) { point_ = pt; }

  protected:
    void DrawInner(canvas::CanvasContext &context) const override;

  public:
    std::string url_;
    util::Point point_;
    util::Size size_;
    std::unique_ptr<canvas::CanvasImage> image_;
    std::function<void()> callback_;
};

} // namespace shape
} // namespace xg

#endif /* Image_hpp */
