//
//  CanvasImage.hpp
//  F2
//
//  Created by weiqing.twq on 2022/1/17.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#ifndef XG_GRAPHICS_CANVAS_CANVASIMAGE_H
#define XG_GRAPHICS_CANVAS_CANVASIMAGE_H

#include <string>
#include <functional>

namespace xg {
namespace canvas {

class CanvasImage {
  public:
    CanvasImage() = delete;
    CanvasImage(const std::string &url) : url_(url){};
    ~CanvasImage();

    void OnLoad(std::function<void()> finished);

    inline void *GetImage() { return image_; }

    float GetWidth();

    float GetHeight();

    inline const std::string &GetURL() { return url_; }

  private:
    //在ios上是CGImageRef
    void *image_ = nullptr;
    //在ios上是一个下载器
    void *downloader_ = nullptr;
    std::string url_;
};

} // namespace canvas
} // namespace xg

#endif /* XG_GRAPHICS_CANVAS_CANVASIMAGE_H */
