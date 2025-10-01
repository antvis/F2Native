//
// Created by weiqing.twq on 2022/1/4.
//

#if defined(__ANDROID__)

#ifndef XG_GRAPHICS_CANVAS_BITMAPCANVASCONTEXTPROXY_H
#define XG_GRAPHICS_CANVAS_BITMAPCANVASCONTEXTPROXY_H

#include "JNIUtil.h"
#include "JavaRef.h"
#include "graphics/canvas/CanvasColorParser.h"
#include "graphics/canvas/CanvasContext.h"
#include "graphics/canvas/CanvasFontParser.h"
#include "../token/DarkModeManager.h"
#include <jni.h>

#define F2SafeCallVoidMethod(env, obj, method, ...) if(env && obj && method) {env->CallVoidMethod(obj, method, __VA_ARGS__);}
#define F2SafeCallVoidMethodNoArg(env, obj, method) if(env && obj && method) {env->CallVoidMethod(obj, method);}
#define F2SafeCallFloatMethod(env, obj, method, ...) if(env && obj && method) {return env->CallFloatMethod(obj, method, __VA_ARGS__);} else { return 0;}
#define F2SafeCallLongMethod(env, obj, method, ...) if(env && obj && method) {return env->CallLongMethod(obj, method, __VA_ARGS__);} else { return 0;}
#define F2SafeCallStringMethod(env, obj, method, ...) if(env && obj && method) {return jni::JavaStringToString(env,(jstring)env->CallObjectMethod(obj, method, __VA_ARGS__));} else { return "";}

using namespace xg::jni;

namespace xg {
namespace canvas {
/**
 *  android bitmap实现的cavnas
 */
class BitmapCanvasContext : public CanvasContext {
  private:
    CanvasFont fontStyle_;
    std::string fontStyleCache_;
    std::string textAlign_;
    std::string textBaseline_;
    float globalAlpha_ = 1.0;
    bool hasClip_ = false;

    CanvasColor fillColor_;
    std::string fillColorCache_;
    CanvasColor strokeColor_;
    std::string strokeColorCache_;
    token::DarkModeManager &darkModeManager_;

    // JNI
    JNIEnv *env_;
    ScopedJavaGlobalRef<jobject> *canvasContext_;

    jmethodID lineTo_ = nullptr;
    jmethodID moveTo_ = nullptr;
    jmethodID stroke_ = nullptr;
    jmethodID setStrokeStyle_ = nullptr;
    jmethodID fill_ = nullptr;
    jmethodID setFillStyle_ = nullptr;
    jmethodID setLineWidth_ = nullptr;
    jmethodID setGlobalAlpha_ = nullptr;
    jmethodID getGlobalAlpha_ = nullptr;
    jmethodID setFont_ = nullptr;
    jmethodID fillText_ = nullptr;
    jmethodID strokeText_ = nullptr;
    jmethodID getTextAlign_ = nullptr;
    jmethodID setTextAlign_ = nullptr;
    jmethodID getTextBaseline_ = nullptr;
    jmethodID setTextBaseline_ = nullptr;
    jmethodID strokeRect_ = nullptr;
    jmethodID save_ = nullptr;
    jmethodID restore_ = nullptr;
    jmethodID measureTextWidth_ = nullptr;
    jmethodID transform_ = nullptr;
    jmethodID setTransform_ = nullptr;
    jmethodID rect_ = nullptr;
    jmethodID clearRect_ = nullptr;
    jmethodID fillRect_ = nullptr;
    jmethodID beginPath_ = nullptr;
    jmethodID closePath_ = nullptr;
    jmethodID quadraticCurveTo_ = nullptr;
    jmethodID bezierCurveTo_ = nullptr;
    jmethodID arc_ = nullptr;
    jmethodID arcTo_ = nullptr;
    jmethodID scale_ = nullptr;
    jmethodID rotate_ = nullptr;
    jmethodID translate_ = nullptr;
    jmethodID setLineDash_ = nullptr;
    jmethodID setLinearGradient_ = nullptr;
    jmethodID setRadialGradient_ = nullptr;
    jmethodID setConicGradient_ = nullptr;
    jmethodID clip_ = nullptr;
    jmethodID drawImage_ = nullptr;
    jmethodID drawImageRect_ = nullptr;
    jmethodID setLineCap_ = nullptr;
    jmethodID setLineJoin_ = nullptr;
    jmethodID measureTextHeight_ = nullptr;

  public:
    BitmapCanvasContext(jobject canvasContext, float devicePixelRatio, token::DarkModeManager &manager) ;

    ~BitmapCanvasContext();

    bool IsValid() override;

    void SetFillStyle(const std::string &color) override;

    void SetFillStyle(const CanvasFillStrokeStyle &style) override;

    void SetStrokeStyle(const std::string &color) override;

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

    float MeasureTextHeight(const std::string &text) override;

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

    void ReplaceStroke() ;

private:
    static ScopedJavaLocalRef<jfloatArray> GetGradientPositions(JNIEnv *env, const CanvasGradient &gradient);
    static ScopedJavaLocalRef<jintArray> GetGradientColors(JNIEnv *env, const CanvasGradient &gradient, token::DarkModeManager &darkModeManager);
};

} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_CANVAS_BITMAPCANVASCONTEXTPROXY_H

#endif //__ANDROID__