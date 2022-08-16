#ifndef XG_GRAPHICS_GUIDE_FLAG_H
#define XG_GRAPHICS_GUIDE_FLAG_H

#include "GuideBase.h"
#include "../util/BBox.h"
#include "../util/Point.h"
#include "../global.h"

namespace xg {
class XChart;

namespace guide {

struct FlagCfg {
    string color = "#1890FF"; // 子元素统一颜色(点/线/方块边框)
    float textSize = DEFAULT_FONTSIZE;
    string textColor ="#1890FF";
    string textAlign = "start";
    string textBaseline = "bottom";
    string content = "";
    float radius = 1.5f; //圆心半径
    vector<float> padding =  {0.f, 0.f, 0.f, 0.f};
    vector<float> rounding =  {0.f, 0.f, 0.f, 0.f};
    float lineWidth =  1.0f;              // 旗子延长线线宽
    string backgroundColor = "#FFFFFF"; // 旗子方块背景色
    vector<string> position;
    float opacity = 1.0f;
    float textOpacity = 1.0f;
    float backgroundOpacity = 1.0f;
    bool top = false;
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(FlagCfg)
        FIELDS(FIELD(&FlagCfg::color),
               FIELD(&FlagCfg::textSize),
               FIELD(&FlagCfg::textColor),
               FIELD(&FlagCfg::textAlign),
               FIELD(&FlagCfg::textBaseline),
               FIELD(&FlagCfg::content),
               FIELD(&FlagCfg::radius),
               FIELD(&FlagCfg::padding),
               FIELD(&FlagCfg::rounding),
               FIELD(&FlagCfg::lineWidth),
               FIELD(&FlagCfg::backgroundColor),
               FIELD(&FlagCfg::position),
               FIELD(&FlagCfg::opacity),
               FIELD(&FlagCfg::textOpacity),
               FIELD(&FlagCfg::backgroundOpacity),
               FIELD(&FlagCfg::top))
        CTORS(DEFAULT_CTOR(FlagCfg))
    END_TYPE
#endif
};

class Flag : public GuideBase {
  public:
    Flag(const FlagCfg &cfg) : GuideBase("flag"), config_(cfg) {}

    void Render(XChart &chart, shape::Group *container, canvas::CanvasContext &context, const std::vector<util::Rect> &dangerRects) override;

    bool IsTop() const noexcept override { return config_.top; }
  private:
    void DrawFlagCircleAndLine(XChart &chart, shape::Group *container, canvas::CanvasContext &context, util::Point &position);

    void PreDrawFlagContent(XChart &chart,
                            shape::Group *container,
                            canvas::CanvasContext &context,
                            const util::Point &position,
                            const float padding[],
                            const std::vector<util::Rect> &dangerRects);

    void DrawFragContent(XChart &chart, shape::Group *container, canvas::CanvasContext &context, util::Point &position, const float padding[]);

  protected:
    static bool collide(const util::Rect &r1, const util::Rect &r2) {
        double maxX, maxY, minX, minY;
        maxX = fmax(r1.x + r1.width, r2.x + r2.width);
        maxY = fmax(r1.y + r1.height, r2.y + r2.height);
        minX = fmin(r1.x, r2.x);
        minY = fmin(r1.y, r2.y);
        return (maxX - minX <= r1.width + r2.width && maxY - minY <= r1.height + r2.height);
    }

  private:
    util::Rect contentRect_;
    FlagCfg config_;
};
} // namespace guide
} // namespace xg

#endif // XG_GRAPHICS_GUIDE_FLAG_H
