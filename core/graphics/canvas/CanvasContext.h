#ifndef XG_GRAPHICS_CANVAS_CONTEXT_H
#define XG_GRAPHICS_CANVAS_CONTEXT_H

#include <vector>
#include "CanvasImage.h"
#include "CanvasFillStrokeStyle.h"
#include "../../utils/Tracer.h"

/// release版本提升性能，减少日志中std::to_string的消耗
/// debug的时候可以输出绘制指令
/// 其它模式只累加cmdCount
#ifdef DEBUG
#define TraceCommand(val) AppendCommand(val);
#else
#define TraceCommand(val) AppendCommandCount();
#endif

#ifdef DEBUG
#   define F2ASSERT(condition, message) \
   do { \
    if (!(condition)) { printf((message)); } \
    assert ((condition)); } while(false)
#else
#   define F2ASSERT(condition, message) do { } while (false)
#endif

namespace xg {
namespace canvas {

/**
 *  Android 和 iOS 的 Canvas 承载对象不一样，在此抹平差异
 */
class CanvasContext {
  public:
    CanvasContext(float devicePixelRatio, utils::Tracer *tracer) : devicePixelRatio_(devicePixelRatio), tracer_(tracer) {}

    virtual ~CanvasContext() { tracer_ = nullptr; }

    virtual bool IsValid() = 0;

    virtual void SetFillStyle(const std::string &color) = 0;

    virtual void SetFillStyle(const CanvasFillStrokeStyle &style) = 0;

    virtual void SetStrokeStyle(const std::string &color) = 0;

    virtual void SetStrokeStyle(const CanvasFillStrokeStyle &style) = 0;

    virtual void SetLineCap(const std::string &lineCap) = 0;

    virtual void SetLineJoin(const std::string &lineJoin) = 0;

    virtual void SetLineWidth(float lineWidth) = 0;

    virtual void SetLineDashOffset(float v) = 0;

    virtual void SetLineDash(const std::vector<float> &params) = 0;

    virtual void SetMiterLimit(float limit) = 0;

    virtual void SetGlobalAlpha(float globalAlpha) = 0;

    virtual float GlobalAlpha() = 0;

    virtual void SetFont(const std::string &font) = 0;

    virtual void FillText(const std::string &text, float x, float y, float maxWidth = SHRT_MAX) = 0;

    virtual void StrokeText(const std::string &text, float x, float y, float maxWidth) = 0;

    virtual std::string TextAlign() const = 0;

    virtual void SetTextAlign(const std::string &textAlign) = 0;

    virtual std::string TextBaseline() const = 0;

    virtual void SetTextBaseline(const std::string &textBaseline) = 0;

    virtual void StrokeRect(float x, float y, float width, float height) = 0;

    virtual void Save() = 0;

    virtual void Restore() = 0;

    virtual bool HasClip() = 0;

    virtual void SetShadowOffsetX(float v) = 0;

    virtual void SetShadowOffsetY(float v) = 0;

    virtual void SetShadowColor(const char *v) = 0;

    virtual void SetShadowBlur(float v) = 0;

    virtual float MeasureTextWidth(const std::string &text) = 0;

    virtual void Transform(float a, float b, float c, float d, float e, float f) = 0;

    virtual void SetTransform(float a, float b, float c, float d, float e, float f) = 0;

    virtual void Rect(float x, float y, float width, float height) = 0;

    virtual void ClearRect(float x, float y, float width, float height) = 0;

    virtual void FillRect(float x, float y, float width, float height) = 0;

    virtual void Fill(const std::string &fillRule = "nonzero") = 0;

    virtual void Stroke() = 0;

    virtual void BeginPath() = 0;

    virtual void MoveTo(float x, float y) = 0;

    virtual void ClosePath() = 0;

    virtual void LineTo(float x, float y) = 0;

    virtual void Clip(const std::string &fillRule = "nonzero") = 0;

    virtual void QuadraticCurveTo(float cpx, float cpy, float x, float y) = 0;

    virtual void BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) = 0;

    virtual void Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise = false) = 0;

    virtual void ArcTo(float x1, float y1, float x2, float y2, float r) = 0;

    virtual void Scale(float scale_width, float scale_height) = 0;

    virtual void Rotate(float angle) = 0;

    virtual void Translate(float x, float y) = 0;
    
    virtual void DrawImage(CanvasImage *image, float dx, float dy) = 0;
    
    virtual void DrawImage(CanvasImage *image, float dx, float dy, float width, float height) = 0;

  public:
    inline float GetDevicePixelRatio() { return devicePixelRatio_; }

    void Reset() { cmdCount_ = 0; }

    long GetRenderCount() { return cmdCount_; }

  protected:
    long cmdCount_ = 0;

    void AppendCommand(const std::string &cmd) {
        AppendCommandCount();
        if(tracer_ != nullptr) {
            tracer_->trace("%s", cmd.c_str());
        }
    }
    
    inline void AppendCommandCount() {
        cmdCount_++;
    }

  protected:
    float devicePixelRatio_ = 1.0f;
    utils::Tracer *tracer_ = nullptr;
};
} // namespace canvas

} // namespace xg

#endif // XG_GRAPHICS_CANVAS_CONTEXT_H
