/**
 *  浏览器上的canvas, 通过document.getElementById(cannvas)的方式去访问web canvas的方法和属性
 */

#ifdef __EMSCRIPTEN__
#include "graphics/canvas/CanvasContext.h"

#endif

#ifndef XG_GRAPHICS_CANVAS_CONTEXT_WASM_H
#define XG_GRAPHICS_CANVAS_CONTEXT_WASM_H

namespace xg {
namespace canvas {

class WebCanvasContext : public CanvasContext {
  public:
    std::string canvasContext_ = "";
    std::string textAlgin_;
    std::string textBaseline_;
    float globalAlpha_ = 1.0;
    bool clip_ = false;

  public:
    WebCanvasContext(const std::string &contextName, float width, float height, double devicePixelRatio);
    ~WebCanvasContext();

    bool IsValid() override;

    void ChangeSize(float width, float height) override;

    void SetFillStyle(const std::string &color) override;

    void SetFillStyle(const canvas::CanvasFillStrokeStyle &style) override;

    void SetStrokeStyle(const std::string &color) override;

    void SetStrokeStyle(const canvas::CanvasFillStrokeStyle &style) override;

    void SetLineCap(const std::string &lineCap) override;

    void SetLineJoin(const std::string &lineJoin) override;

    void SetLineWidth(float lineWidth) override;

    void SetLineDashOffset(float v) override;

    void SetLineDash(const std::vector<float> &params) override;

    void SetMiterLimit(float limit) override;

    void SetGlobalAlpha(float globalAlpha) override;

    float GlobalAlpha() override;

    void SetFont(const std::string &font) override;

    void FillText(const std::string &text, float x, float y, float maxWidth = SHRT_MAX) override;

    void StrokeText(const std::string &text, float x, float y, float maxWidth) override;

    std::string TextAlign() const override;

    void SetTextAlign(const std::string &textAlign) override;

    std::string TextBaseline() const override;

    void SetTextBaseline(const std::string &textBaseline) override;

    void StrokeRect(float x, float y, float width, float height) override;

    void Save() override;

    void Restore() override;

    bool HasClip() override;

    void SetShadowOffsetX(float v) override;

    void SetShadowOffsetY(float v) override;

    void SetShadowColor(const char *v) override;

    void SetShadowBlur(float v) override;

    float MeasureTextWidth(const std::string &text) override;

    void Transform(float a, float b, float c, float d, float e, float f) override;

    void SetTransform(float a, float b, float c, float d, float e, float f) override;

    void Rect(float x, float y, float width, float height) override;

    void ClearRect(float x, float y, float width, float height) override;

    void FillRect(float x, float y, float width, float height) override;

    void Fill(const std::string &fillRule = "nonzero") override;

    void Stroke() override;

    void BeginPath() override;

    void MoveTo(float x, float y) override;

    void ClosePath() override;

    void LineTo(float x, float y) override;

    void Clip(const std::string &fillRule = "nonzero") override;

    void QuadraticCurveTo(float cpx, float cpy, float x, float y) override;

    void BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) override;

    void Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise = false) override;

    void ArcTo(float x1, float y1, float x2, float y2, float r) override;

    void Scale(float scale_width, float scale_height) override;

    void Rotate(float angle) override;

    void Translate(float x, float y) override;

    void DrawImage(CanvasImage *image, float dx, float dy) override;

    void DrawImage(CanvasImage *image, float dx, float dy, float width, float height) override;
};
} // namespace canvas

} // namespace xg

#endif // XG_GRAPHICS_CANVAS_CONTEXT_ALIPAY_H
//#endif // TARGET_ALIPAY
