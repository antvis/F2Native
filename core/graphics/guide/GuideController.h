#ifndef XG_GRAPHICS_GUIDE_CONTROLLER_H
#define XG_GRAPHICS_GUIDE_CONTROLLER_H

#include "Background.h"
#include "Flag.h"
#include "Line.h"
#include "Text.h"
#include "Image.h"
#include "Point.h"
#include "../shape/Group.h"

namespace xg {
class XChart;

namespace guide {

class GuideController {
  public:
    GuideController(shape::Group *container, shape::Group *backContainer)
        : container_(container), backContainer_(backContainer) {}

    ~GuideController() {
        Clear();
        container_ = nullptr;
        backContainer_ = nullptr;
    }

    // 分时图小旗子
    void Flag(const std::string &json = "");

    // 文字
    void Text(const std::string &json = "");

    // 线
    void Line(const std::string &json = "");

    // 背景色
    void Background(const std::string &json = "");
    
    // 图片
    void Image(const std::string &json = "");
    
    // 圆点
//    void Point(const std::string &json = "");
        
    void FlagObject(const FlagCfg &config);
    void TextObject(const TextCfg &config);
    void LineObject(const LineCfg &config);
    void BackgroundObject(const BackgroundCfg &config);
    void ImageObject(const ImageCfg &config);
    void PointObject(const PointCfg &config);

    void Clear() {
        this->container_->Clear();
        this->backContainer_->Clear();
        this->guides.clear();
        this->dangerRects.clear();
    }
    
    void ClearInner() {
        this->container_->Clear();
        this->backContainer_->Clear();
    }

    void Render(XChart &chart, canvas::CanvasContext &context);

  private:
    std::vector<std::unique_ptr<GuideBase>> guides;
    std::vector<util::Rect> dangerRects = {};
    shape::Group *container_ = nullptr;
    // backLayout 层
    shape::Group *backContainer_ = nullptr;
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_CONTROLLER_H
