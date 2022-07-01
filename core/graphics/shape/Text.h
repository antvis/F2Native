#ifndef XG_GRAPHICS_SHAPE_TEXT_H
#define XG_GRAPHICS_SHAPE_TEXT_H

#include "Shape.h"

namespace xg {
namespace shape {

class Text : public Shape {
  public:
    virtual ~Text() override {}

    Text(const std::string &, const util::Point &, const float fontSize, const std::string &strokeColor, const std::string &fillColor);

    /// 获取包围盒，每一次都会计算，建议使用GetBox
    /// @param context 上下文
    BBox CalculateBox(canvas::CanvasContext &context) const override;

    float GetTextHeight() const;
    float GetTextWidth(canvas::CanvasContext &context) const;

    inline util::Point GetPoint() const { return point_; }
    inline void SetPoint(const util::Point &pt) { point_ = pt; }

    inline void SetTextAlign(const std::string &align) { textAlign_ = align; }
    inline void SetTextBaseline(const std::string &baseline) { textBaseline_ = baseline; }

    float GetSpacingY() const;

    int GetLineCount() const { return this->lineCount_; }

    const std::string &GetTextAlign() noexcept { return textAlign_; }
  protected:
    void DrawInner(canvas::CanvasContext &context) const override;

  private:
    std::string GetFontStyle() const;

  protected:
    int lineCount_ = 1;
    float fontSize_ = DEFAULT_FONTSIZE;
    float lineHeight_ = std::nan("0");
    float height_ = std::nan("0");
    float width_ = std::nan("0");
    std::string fontFamily_ = "sans-serif";
    std::string fontStyle_ = "normal";
    std::string fontWeight_ = "normal";
    std::string fontVariant_ = "normal";
    std::string text_ = "";
    vector<std::string> textArr_; //\n分割后的字符串

  public:
    nlohmann::json ext;
};

std::string CreateFontStyle(float fontSize = DEFAULT_FONTSIZE,
                            const std::string &fontStyle = "normal",
                            const std::string &fontVariant = "normal",
                            const std::string &fontWeight = "normal",
                            const std::string &fontFamily = "sans-serif");

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_TEXT_H */
