#if defined(__APPLE__)

#include <array>
#include <unordered_map>
#include "CanvasContext.h"
#include "CanvasFontParser.h"
#include "CanvasColorParser.h"

#ifndef XG_GRAPHICS_COREGRAPHICS_CONTEXT_H
#define XG_GRAPHICS_COREGRAPHICS_CONTEXT_H

namespace xg {
namespace canvas {

enum class Gradient {
    None,
    Linear,
    Radial
};

class ScopedCFObject {
public:
    ScopedCFObject(void *obj);
    ~ScopedCFObject();
public:
    void *object_;
};

/**
 *  CoreGraphic实现的cavnas
 */
class CoreGraphicsContext : public CanvasContext {
  public:
    void *canvasContext_ = nullptr;
    CanvasFont fontStyle_;
    std::string fontStyleCache_;
    std::string textAlgin_;
    std::string textBaseline_;
    
    CanvasColor fillColor_;
    std::string fillColorCache_;
    CanvasColor strokeColor_;
    std::string strokeColorCache_;
    
    float globalAlpha_ = 1.0;
    Gradient gradient_;
    canvas::CanvasFillStrokeStyle gradientStyle_;
    float width_;
    float height_;
    
    std::unordered_map<size_t, std::unique_ptr<ScopedCFObject>> cfObjects_;
  public:
    CoreGraphicsContext(void *_canvasContext, float width, float height, float devicePixelRatio, utils::Tracer *tracer)
        : CanvasContext(devicePixelRatio, tracer), canvasContext_(_canvasContext), width_(width), height_(height) {
            if (tracer != nullptr) {
                tracer->trace("Use CoreGraphicsContext");
            }
        }

    ~CoreGraphicsContext() { canvasContext_ = nullptr; }

    bool IsValid() override;

    void SetFillStyle(const std::string &color) override;

    void SetFillStyle(const CanvasFillStrokeStyle &style) override;

    void SetStrokeStyle(const std::string &color) override ;

    void SetStrokeStyle(const CanvasFillStrokeStyle &style) override;

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
    
    void DrawLinearGradient(const canvas::CanvasFillStrokeStyle &gradientStyle);
    void DrawRadialGradinet(const canvas::CanvasFillStrokeStyle &gradientStyle);
private:
    void DrawText(const std::string &text, const CanvasColor &color, float x, float y, float maxWidth = SHRT_MAX);
    
    template <class T>
    inline void hash_combine(std::size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }

    template <typename T, typename... Rest>
    inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        hash_combine(seed, rest...);
    }
};
} // namespace canvas

} // namespace xg

#endif // XG_GRAPHICS_COREGRAPHICS_CONTEXT_ALIPAY_H
#endif // APPLE


