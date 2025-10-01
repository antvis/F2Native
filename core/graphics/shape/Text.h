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
    /// @param context
    BBox CalculateBox(canvas::CanvasContext &context) const override;

    float GetTextHeight() const;
    float GetTextWidth(canvas::CanvasContext &context) const;
    float GetTextHeightWithContext(canvas::CanvasContext &context) const;

    inline util::Point GetPoint() const { return point_; }
    inline void SetPoint(const util::Point &pt) { point_ = pt; }

    inline void SetTextAlign(const std::string &align) { textAlign_ = align; }
    inline void SetTextBaseline(const std::string &baseline) { textBaseline_ = baseline; }

    float GetSpacingY() const;

    int GetLineCount() const { return this->lineCount_; }

    const std::string &GetTextAlign() noexcept { return textAlign_; }

    /// 设置文字大小
    /// @param fontSize 文字大小
    inline void SetTextFontSize(const float fontSize) {
        fontSize_ = fontSize;
        font_ = GetFontStyle();
    }

    /// 设置文字font
    inline void SetTextFont(const std::string fontStyle,
                             const std::string fontVariant, const std::string fontWeight,
                             const std::string fontFamily) {
        bool showReGet = false;
        if (!fontStyle.empty()){
            fontStyle_ = fontStyle;
            showReGet = true;
        }
        if (!fontVariant.empty()) {
            fontVariant_ = fontVariant;
            showReGet = true;
        }
        if (!fontWeight.empty()){
            fontWeight_ = fontWeight;
            showReGet = true;
        }
        if (!fontFamily.empty()) {
            fontFamily_ = fontFamily;
            showReGet = true;
        }
        if (showReGet){
            font_ = GetFontStyle();
        }
    }

    /// 根据最大宽度调整字体大小
    /// @param context 画布的context
    /// @param padding 文字的padding
    /// @param minSize 最小可显示的字体大小
    /// @param maxWidth 可显示的最大宽度
    void AdaptTextFontSize(canvas::CanvasContext &context,
                           int padding, int minSize, int maxWidth);

  protected:
    void DrawInner(canvas::CanvasContext &context) const override;

  private:
    std::string GetFontStyle() const;

  protected:
    int lineCount_ = 1;
    float fontSize_ = 12.f;
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

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_TEXT_H */
