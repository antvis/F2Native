#ifndef XG_GRAPHICS_SHAPE_TEXT_H
#define XG_GRAPHICS_SHAPE_TEXT_H

#include "graphics/shape/Shape.h"
#include <nlohmann/json.hpp>

namespace xg {
namespace shape {

class Text : public Shape {
  public:
    virtual ~Text() override {}

    Text(const string &, const util::Point &, const float fontSize, const string &strokeColor, const string &fillColor);

    /// 获取包围盒，每一次都会计算，建议使用GetBox
    /// @param context
    BBox CalculateBox(canvas::CanvasContext &context) const override;

    float GetTextHeight() const;
    float GetTextWidth(canvas::CanvasContext &context) const;

    inline util::Point GetPoint() const { return point_; }
    inline void SetPoint(const util::Point &pt) { point_ = pt; }

    inline void SetTextAlign(const std::string &align) { textAlign_ = align; }
    inline void SetTextBaseline(const std::string &baseline) { textBaseline_ = baseline; }

    float GetSpacingY() const;

    int GetLineCount() const { return this->lineCount_; }

  protected:
    void DrawInner(canvas::CanvasContext &context) const override;

  private:
    string GetFontStyle() const;

  protected:
    int lineCount_ = 1;
    float fontSize_ = 12.f;
    float lineHeight_ = std::nan("0");
    float height_ = std::nan("0");
    float width_ = std::nan("0");
    string fontFamily_ = "sans-serif";
    string fontStyle_ = "normal";
    string fontWeight_ = "normal";
    string fontVariant_ = "normal";
    string text_ = "";
    vector<string> textArr_; //\n分割后的字符串

  public:
    nlohmann::json ext;
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_TEXT_H */
