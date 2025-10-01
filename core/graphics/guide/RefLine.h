//
//  RefLine.hpp
//  AFWFinanceChart
//
//  Created by ougu on 2024/9/29.
//  Copyright © 2024 Alipay. All rights reserved.
//

#ifndef RefLine_hpp
#define RefLine_hpp

#include "GuideBase.h"

namespace xg {
class XChart;
namespace guide {

class RefLine : public GuideBase {
public:
    RefLine(const nlohmann::json cfg = {}) : GuideBase("refline", MergeDefaultCfg(cfg)) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    util::BBox GetBBox() override { return bbox_; }

protected:
  static nlohmann::json MergeDefaultCfg(const nlohmann::json &config) {
      nlohmann::json defaultCfg = {};
      if(config.is_object()) {
          defaultCfg.merge_patch(config);
      }
      return defaultCfg;
  }
};

} // namespace guide
} // namespace xg

#endif /* RefLine_hpp */
