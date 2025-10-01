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

class Image : public GuideBase {
public:
    Image(const nlohmann::json &config = {}) : GuideBase("image", MergeDefaultCfg(config)) {}

  void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

  util::BBox GetBBox() override { return bbox_; }

protected:
  static nlohmann::json MergeDefaultCfg(const nlohmann::json &config) {
      nlohmann::json defaultCfg = {
              {"width", 12},
              {"height", 12},
              {"margin", {0, 0}}// margin: left & top
      };
      if(config.is_object()) {
          defaultCfg.merge_patch(config);
      }
      return defaultCfg;
  }
};

} // namespace guide
} // namespace xg



#endif /* Image_h */
