//
// Created by 青原 on 2020-09-17.
//

#include <android/native_window_jni.h>
#include <jni.h>

#include <nlohmann/json.hpp>
#include <string>

#include <GCanvas/core/src/gcanvas/GFontManager.h>
#include <GCanvas/core/src/platform/Android/GCanvas2DContextAndroid.h>
#include <GCanvas/core/src/platform/Android/GCanvasAndroid.h>
#include <GCanvas/core/src/platform/Android/egl/GEGLContext.h>
#include <GCanvas/core/src/support/Log.h>

#ifndef XG_ANDROID_F2_CHARTVIEW_H
#define XG_ANDROID_F2_CHARTVIEW_H

namespace xg {

class F2CanvasView {
  public:
    F2CanvasView(const nlohmann::json &cfg);

    ~F2CanvasView();

    int InitCanvasContext();

    bool IsInitCanvasContext() { return canvas_context_init_; }

    int PerformSwapFrame();

    int OnSurfaceCreated(ANativeWindow *nativeWindow);

    int OnSurfaceChanged(int width, int height);

    int OnSurfaceDestroyed();

    const std::string GetCanvasId() { return this->canvasId_; }

    ////////////////////////////////////// graphic 2d apis ///////////////////////////////////////////////////////
    float MeasureText(const std::string &text);

    void SetFillStyle(const std::string &style);

    void SetFillStyle(int r, int g, int b, float a);

    void SetStrokeStyle(const std::string &style);

    void SetLinearGradientFillStyle(float x0, float y0, float x1, float y1, std::vector<float> &positions, std::vector<std::string> &colors, bool is_stroke = false);

    void SetRadialGradientFillStyle(float x0,
                                    float y0,
                                    float r0,
                                    float x1,
                                    float y1,
                                    float r1,
                                    std::vector<float> &positions,
                                    std::vector<std::string> &colors,
                                    bool is_stroke = false);

    void SetPatternFillStyle(int textureId, int width, int height, std::string &&mode, bool is_stroke = false);

    void Rect(float x, float y, float width, float height);

    void FillRect(float x, float y, float width, float height);

    void StrokeRect(float x, float y, float width, float height);

    void ClearRect(float x, float y, float width, float height);

    void Fill(GFillRule fr = GFillRule::FILL_RULE_NONZERO);

    void Stroke();

    void BeginPath();

    void MoveTo(float x, float y);

    void ClosePath();

    void LineTo(float x, float y);

    void Clip(GFillRule f = GFillRule::FILL_RULE_NONZERO);

    void QuadraticCurveTo(float cpx, float cpy, float x, float y);

    void BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);

    void Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise);

    void ArcTo(float x1, float y1, float x2, float y2, float r);

    void Scale(float scalewidth, float scaleheight);

    void Rotate(float angle);

    void Translate(float x, float y);

    void Transform(float a, float b, float c, float d, float e, float f);

    void SetTransform(float a, float b, float c, float d, float e, float f);

    void SetLineCap(const std::string &lineCap);

    void SetLineJoin(const std::string &lineJoin);

    void SetLineWidth(float lineWidth);

    void SetLineDashOffset(float offset);

    void SetLineDash(std::vector<float> params);

    std::vector<float> &GetLineDash();

    void SetMiterLimit(float limit);

    void SetFont(const std::string &font);

    void SetTextAlign(const std::string &textAlign);

    void SetTextAlign(GTextAlign &textAlign);

    GTextAlign TextAlign() const;

    void SetTextBaseline(const std::string &textBaseline);

    void SetTextBaseline(GTextBaseline &textBaseline);

    GTextBaseline TextBaseline() const;

    void FillText(const std::string &text, float x, float y, float maxWidth);

    void StrokeText(const std::string &text, float x, float y, float maxWidth);

    void SetGlobalAlpha(float globalAlpha);

    float GlobalAlpha();

    void SetGlobalCompositeOp(const std::string &op);

    void Save();

    void Restore();

    void SetShadowOffsetX(float v);

    void SetShadowOffsetY(float v);

    void SetShadowColor(const char *color);

    void SetShadowBlur(float v);

    bool HasClip();

  protected:
    void MakeCurrent();

    int CreateCanvas2DImpl();

    int ResumeCanvas(ANativeWindow *window);

    int Destroy();

  protected:
    GEGLContext *egl_context_ = nullptr;
    // java层surface
    ANativeWindow *system_native_window_ = nullptr;

    GCanvasAndroid *canvas_ = nullptr;
    GCanvas2DContextAndroid *canvas_2d_context_ = nullptr;

  private:
    void UpdateSurfaceCreatedStatus();

    void UpdateSurfaceChangedStatus(int width, int height);

    void UpdateSurfaceDestroyedStatus();

  private:
    std::string canvasId_;
    double devicePixelRatio_ = 1.0;
    double canvas_width_ = 0.;
    double canvas_height_ = 0;
    std::string backgroundColor_ = "#fff";

    double view_width_ = 0;
    double view_height_ = 0;

    bool surface_created_ = false;
    bool surface_changed_ = false;
    bool surface_destroyed_ = false;
    bool egl_surface_valid_ = false;
    bool canvas_context_init_ = false;
    bool canvas_context_valid_ = false;

    bool has_destroy_ = false;

    GCanvasHooks *hooks_ = nullptr;
    GFontManager *fontManager_ = nullptr;
};
} // namespace xg

#endif // XG_ANDROID_F2_CHARTVIEW_H
