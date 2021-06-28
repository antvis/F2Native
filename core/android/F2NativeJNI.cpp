#include "JNIUtil.h"

#include <android/log.h>
#include <jni.h>

#if defined(TARGET_STANDALONE)
#include "F2CanvasView.h"
#elif defined(TARGET_ALIPAY)
#include <AntGraphic.h>
#endif

#include "F2NativeJNI.h"
#include "JavaRef.h"

#include "android/JavaF2Function.h"
#include "graphics/XChart.h"
#include "graphics/geom/Geom.h"
#include "graphics/geom/Interval.h"
#include "graphics/guide/GuideController.h"

#ifndef xg_jni_arraysize

template <typename T, size_t N> char (&_xg_ArraySizeHelper(T (&array)[N]))[N];

#define xg_jni_arraysize(array) (sizeof(_xg_ArraySizeHelper(array)))
#endif // xg_jni_arraysize

// extern GCanvasSystemLog gcanvasSystemLog;

namespace xg {
namespace jni {

#ifdef TARGET_STANDALONE
static jlong CreateNativeCanvasView(JNIEnv *env, jclass clazz, jstring stringOptions) {
    std::string _options = JavaStringToString(env, stringOptions);
    F2CanvasView *view = new F2CanvasView(std::move(_options));
    return reinterpret_cast<jlong>(view);
}

static jint OnSurfaceAvailable(JNIEnv *env, jclass clazz, jlong view, jobject surface) {
    F2_LOG_I("F2ChartView", "%s", "#OnSurfaceAvailable");
    if(surface != nullptr) {
        ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
        F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
        F2_LOG_I(chartView->GetCanvasId(), "%s", "#OnSurfaceAvailable");
        return chartView->OnSurfaceCreated(window);
    }
    return 0;
}

static jint InitCanvasContext(JNIEnv *env, jclass clazz, jlong view) {
    return reinterpret_cast<F2CanvasView *>(view)->InitCanvasContext();
}

static jboolean IsCanvasContextInitted(JNIEnv *env, jclass clazz, jlong view) {
    return reinterpret_cast<F2CanvasView *>(view)->IsInitCanvasContext();
}

static jint OnSurfaceSizeChanged(JNIEnv *env, jclass clazz, jlong view, jint width, jint height) {
    return reinterpret_cast<F2CanvasView *>(view)->OnSurfaceChanged(width, height);
}

static jint OnSurfaceDestroy(JNIEnv *env, jclass clazz, jlong view) {
    return reinterpret_cast<F2CanvasView *>(view)->OnSurfaceDestroyed();
}

static jint OnViewDestroy(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    delete chartView;
    return 0;
}

static jint PerformSwapBuffer(JNIEnv *env, jclass clazz, jlong view) {
    return reinterpret_cast<F2CanvasView *>(view)->PerformSwapFrame();
}

// ########################## graphic canvas apis #########################################
static void CanvasSetFillStyle(JNIEnv *env, jclass clazz, jlong view, jstring style) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _style = JavaStringToString(env, style);
    chartView->SetFillStyle(std::move(_style));
}

static void CanvasSetStrokeStyle(JNIEnv *env, jclass clazz, jlong view, jstring style) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _style = JavaStringToString(env, style);
    chartView->SetStrokeStyle(std::move(_style));
}

static void CanvasRect(JNIEnv *env, jclass clazz, jlong view, jint x, jint y, jint w, jint h) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Rect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
}

static void CanvasFillRect(JNIEnv *env, jclass clazz, jlong view, jint x, jint y, jint w, jint h) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->FillRect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
}
static void CanvasStrokeRect(JNIEnv *env, jclass clazz, jlong view, jint x, jint y, jint w, jint h) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->StrokeRect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
}
static void CanvasClearRect(JNIEnv *env, jclass clazz, jlong view, jint x, jint y, jint w, jint h) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->ClearRect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
}
static void CanvasFill(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Fill();
}
static void CanvasStroke(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Stroke();
}
static void CanvasBeginPath(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->BeginPath();
}
static void CanvasMoveTo(JNIEnv *env, jclass clazz, jlong view, jfloat x, jfloat y) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->MoveTo(x, y);
}
static void CanvasClosePath(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->ClosePath();
}
static void CanvasLineTo(JNIEnv *env, jclass clazz, jlong view, jfloat x, jfloat y) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->LineTo(x, y);
}
static void CanvasClip(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Clip();
}
static void CanvasQuadraticCurveTo(JNIEnv *env, jclass clazz, jlong view, jfloat cpx, jfloat cpy, jfloat x, jfloat y) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->QuadraticCurveTo(cpx, cpy, x, y);
}
static void CanvasBezierCurveTo(JNIEnv *env, jclass clazz, jlong view, jfloat cp1x, jfloat cp1y, jfloat cp2x, jfloat cp2y, jfloat x, jfloat y) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->BezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
}
static void CanvasArc(JNIEnv *env, jclass clazz, jlong view, jfloat x, jfloat y, jfloat r, jfloat sAngle, jfloat eAngle, bool antiClockwise) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Arc(x, y, r, sAngle, eAngle, antiClockwise);
}
static void CanvasArcTo(JNIEnv *env, jclass clazz, jlong view, jfloat x1, jfloat y1, jfloat x2, jfloat y2, jfloat r) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->ArcTo(x1, y1, x2, y2, r);
}
static void CanvasScale(JNIEnv *env, jclass clazz, jlong view, jfloat sw, jfloat sh) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Scale(sw, sh);
}
static void CanvasRotate(JNIEnv *env, jclass clazz, jlong view, jfloat r) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Rotate(r);
}
static void CanvasTranslate(JNIEnv *env, jclass clazz, jlong view, jfloat x, jfloat y) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Translate(x, y);
}
static void CanvasTransform(JNIEnv *env, jclass clazz, jlong view, jfloat a, jfloat b, jfloat c, jfloat d, jfloat e, jfloat f) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Transform(a, b, c, d, e, f);
}
static void CanvasSetTransform(JNIEnv *env, jclass clazz, jlong view, jfloat a, jfloat b, jfloat c, jfloat d, jfloat e, jfloat f) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->SetTransform(a, b, c, d, e, f);
}
static void CanvasSetLineCap(JNIEnv *env, jclass clazz, jlong view, jstring lineCap) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _lineCap = JavaStringToString(env, lineCap);
    chartView->SetLineCap(std::move(_lineCap));
}
static void CanvasSetLineJoin(JNIEnv *env, jclass clazz, jlong view, jstring lineJoin) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _lineJoin = JavaStringToString(env, lineJoin);
    chartView->SetLineCap(std::move(_lineJoin));
}
static void CanvasSetLineWidth(JNIEnv *env, jclass clazz, jlong view, jfloat lineWidth) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->SetLineWidth(lineWidth);
}
static void CanvasSetLineDashOffset(JNIEnv *env, jclass clazz, jlong view, jfloat offset) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->SetLineDashOffset(offset);
}
static void CanvasSetLineDash(JNIEnv *env, jclass clazz, jlong view, jfloatArray dash) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    // std::vector<float> _dash = JFloatArrayToVector(env, dash);
    chartView->SetLineDash(std::vector<float>{});
}
static jfloatArray CanvasGetLineDash(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::vector<float> dash = chartView->GetLineDash();
    jfloatArray rst = env->NewFloatArray(dash.size());
    env->SetFloatArrayRegion(rst, 0, dash.size(), &dash[0]);
    return rst;
}

static void CanvasSetMiterLimit(JNIEnv *env, jclass clazz, jlong view, jfloat limit) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->SetMiterLimit(limit);
}
static void CanvasSetFont(JNIEnv *env, jclass clazz, jlong view, jstring font) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _font = JavaStringToString(env, font);
    chartView->SetFont(std::move(_font));
}
static void CanvasSetTextAlign(JNIEnv *env, jclass clazz, jlong view, jstring textAlign) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _textAlign = JavaStringToString(env, textAlign);
    chartView->SetTextAlign(std::move(_textAlign));
}
static jstring CanvasGetTextAlign(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    GTextAlign textAlign = chartView->TextAlign();
    return StringToJString(env, "start");
}
static void CanvasSetTextBaseline(JNIEnv *env, jclass clazz, jlong view, jstring textBaseline) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _textBaseline = JavaStringToString(env, textBaseline);
    chartView->SetTextBaseline(std::move(_textBaseline));
}
static jstring CanvasGetTextBaseline(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    GTextBaseline textAlign = chartView->TextBaseline();
    return StringToJString(env, "bottom");
}
static void CanvasFillText(JNIEnv *env, jclass clazz, jlong view, jstring text, jfloat x, jfloat y) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _text = JavaStringToString(env, text);
    chartView->FillText(std::move(_text), x, y, SHRT_MAX);
}
static void CanvasFillTextMaxWdith(JNIEnv *env, jclass clazz, jlong view, jstring text, jfloat x, jfloat y, jfloat maxWidth) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _text = JavaStringToString(env, text);
    chartView->FillText(std::move(_text), x, y, maxWidth);
}
static void CanvasStrokeText(JNIEnv *env, jclass clazz, jlong view, jstring text, jfloat x, jfloat y) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _text = JavaStringToString(env, text);
    chartView->StrokeText(std::move(_text), x, y, SHRT_MAX);
}
static void CanvasStrokeTextMaxWdith(JNIEnv *env, jclass clazz, jlong view, jstring text, jfloat x, jfloat y, jfloat maxWidth) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _text = JavaStringToString(env, text);
    chartView->StrokeText(std::move(_text), x, y, maxWidth);
}
static jfloat CanvasMeasureTExt(JNIEnv *env, jclass clazz, jlong view, jstring text) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _text = JavaStringToString(env, text);
    return chartView->MeasureText(std::move(_text));
}
static void CanvasSetGlobalAlpha(JNIEnv *env, jclass clazz, jlong view, jfloat globalAlpha) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->SetGlobalAlpha(globalAlpha);
}
static jfloat CanvasGetGlobalAlpha(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    return chartView->GlobalAlpha();
}
static void CanvasSetGlobalCompositeOperation(JNIEnv *env, jclass clazz, jlong view, jstring op) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _op = JavaStringToString(env, op);
    chartView->SetGlobalCompositeOp(std::move(_op));
}
static void CanvasSave(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Save();
}
static void CanvasRestore(JNIEnv *env, jclass clazz, jlong view) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->Restore();
}
static void CanvasSetShadowColor(JNIEnv *env, jclass clazz, jlong view, jstring color) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    std::string _color = JavaStringToString(env, color);
    chartView->SetShadowColor(_color.data());
}
static void CanvasSetShadowBlur(JNIEnv *env, jclass clazz, jlong view, jint blur) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->SetShadowBlur(static_cast<float>(blur));
}
static void CanvasSetShadowOffsetX(JNIEnv *env, jclass clazz, jlong view, jfloat offsetX) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->SetShadowOffsetX(offsetX);
}
static void CanvasSetShadowOffsetY(JNIEnv *env, jclass clazz, jlong view, jfloat offsetY) {
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);
    chartView->SetShadowOffsetY(offsetY);
}

static const JNINativeMethod native_canvas_methods[] = {{
                                                            .name = "nCreateNativeChartView",
                                                            .signature = "(Ljava/lang/String;)J",
                                                            .fnPtr = reinterpret_cast<void *>(CreateNativeCanvasView),
                                                        },
                                                        {
                                                            .name = "nSurfaceAvailable",
                                                            .signature = "(JLandroid/view/Surface;)I",
                                                            .fnPtr = reinterpret_cast<void *>(OnSurfaceAvailable),
                                                        },
                                                        {
                                                            .name = "nInitCanvasContext",
                                                            .signature = "(J)I",
                                                            .fnPtr = reinterpret_cast<void *>(InitCanvasContext),
                                                        },
                                                        {
                                                            .name = "nIsCanvasContextInitted",
                                                            .signature = "(J)Z",
                                                            .fnPtr = reinterpret_cast<void *>(IsCanvasContextInitted),
                                                        },
                                                        {
                                                            .name = "nSurfaceSizeChanged",
                                                            .signature = "(JII)I",
                                                            .fnPtr = reinterpret_cast<void *>(OnSurfaceSizeChanged),
                                                        },
                                                        {
                                                            .name = "nSurfaceDestroyed",
                                                            .signature = "(J)I",
                                                            .fnPtr = reinterpret_cast<void *>(OnSurfaceDestroy),
                                                        },
                                                        {
                                                            .name = "nDestroyNativeChartView",
                                                            .signature = "(J)I",
                                                            .fnPtr = reinterpret_cast<void *>(OnViewDestroy),
                                                        },
                                                        {
                                                            .name = "nPerformSwapFrame",
                                                            .signature = "(J)I",
                                                            .fnPtr = reinterpret_cast<void *>(PerformSwapBuffer),
                                                        },
                                                        {
                                                            .name = "nSetFillStyle",
                                                            .signature = "(JLjava/lang/String;)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetFillStyle),
                                                        },
                                                        {
                                                            .name = "nSetStrokeStyle",
                                                            .signature = "(JLjava/lang/String;)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetStrokeStyle),
                                                        },
                                                        {
                                                            .name = "nRect",
                                                            .signature = "(JIIII)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasRect),
                                                        },
                                                        {
                                                            .name = "nFillRect",
                                                            .signature = "(JIIII)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasFillRect),
                                                        },
                                                        {
                                                            .name = "nStrokeRect",
                                                            .signature = "(JIIII)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasStrokeRect),
                                                        },
                                                        {
                                                            .name = "nClearRect",
                                                            .signature = "(JIIII)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasClearRect),
                                                        },
                                                        {
                                                            .name = "nFill",
                                                            .signature = "(J)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasFill),
                                                        },
                                                        {
                                                            .name = "nStroke",
                                                            .signature = "(J)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasStroke),
                                                        },
                                                        {
                                                            .name = "nBeginPath",
                                                            .signature = "(J)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasBeginPath),
                                                        },
                                                        {
                                                            .name = "nMoveTo",
                                                            .signature = "(JFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasMoveTo),
                                                        },
                                                        {
                                                            .name = "nClosePath",
                                                            .signature = "(J)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasClosePath),
                                                        },
                                                        {
                                                            .name = "nLineTo",
                                                            .signature = "(JFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasLineTo),
                                                        },
                                                        {
                                                            .name = "nClip",
                                                            .signature = "(J)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasClip),
                                                        },
                                                        {
                                                            .name = "nQuadraticCurveTo",
                                                            .signature = "(JFFFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasQuadraticCurveTo),
                                                        },
                                                        {
                                                            .name = "nBezierCurveTo",
                                                            .signature = "(JFFFFFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasBezierCurveTo),
                                                        },
                                                        {
                                                            .name = "nArc",
                                                            .signature = "(JFFFFFZ)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasArc),
                                                        },
                                                        {
                                                            .name = "nArcTo",
                                                            .signature = "(JFFFFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasArcTo),
                                                        },
                                                        {
                                                            .name = "nScale",
                                                            .signature = "(JFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasScale),
                                                        },
                                                        {
                                                            .name = "nRotate",
                                                            .signature = "(JF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasRotate),
                                                        },
                                                        {
                                                            .name = "nTranslate",
                                                            .signature = "(JFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasTranslate),
                                                        },
                                                        {
                                                            .name = "nTransform",
                                                            .signature = "(JFFFFFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasTransform),
                                                        },
                                                        {
                                                            .name = "nSetTransform",
                                                            .signature = "(JFFFFFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetTransform),
                                                        },
                                                        {
                                                            .name = "nSetLineCap",
                                                            .signature = "(JLjava/lang/String;)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetLineCap),
                                                        },
                                                        {
                                                            .name = "nSetLineJoin",
                                                            .signature = "(JLjava/lang/String;)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetLineJoin),
                                                        },
                                                        {
                                                            .name = "nSetLineWidth",
                                                            .signature = "(JF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetLineWidth),
                                                        },
                                                        {
                                                            .name = "nSetLineDashOffset",
                                                            .signature = "(JF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetLineDashOffset),
                                                        },
                                                        {
                                                            .name = "nSetLineDash",
                                                            .signature = "(J[F)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetLineDash),
                                                        },
                                                        {
                                                            .name = "nGetLineDash",
                                                            .signature = "(J)[F",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasGetLineDash),
                                                        },
                                                        {
                                                            .name = "nSetMiterLimit",
                                                            .signature = "(JF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetMiterLimit),
                                                        },
                                                        {
                                                            .name = "nSetFont",
                                                            .signature = "(JLjava/lang/String;)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetFont),
                                                        },
                                                        {
                                                            .name = "nSetTextAlign",
                                                            .signature = "(JLjava/lang/String;)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetTextAlign),
                                                        },
                                                        {
                                                            .name = "nGetTextAlign",
                                                            .signature = "(J)Ljava/lang/String;",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasGetTextAlign),
                                                        },
                                                        {
                                                            .name = "nSetTextBaseline",
                                                            .signature = "(JLjava/lang/String;)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetTextBaseline),
                                                        },
                                                        {
                                                            .name = "nGetTextBaseline",
                                                            .signature = "(J)Ljava/lang/String;",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasGetTextBaseline),
                                                        },
                                                        {
                                                            .name = "nGetTextBaseline",
                                                            .signature = "(J)Ljava/lang/String;",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasGetTextBaseline),
                                                        },
                                                        {
                                                            .name = "nFillText",
                                                            .signature = "(JLjava/lang/String;FF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasFillText),
                                                        },
                                                        {
                                                            .name = "nFillText",
                                                            .signature = "(JLjava/lang/String;FFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasFillTextMaxWdith),
                                                        },
                                                        {
                                                            .name = "nStrokeText",
                                                            .signature = "(JLjava/lang/String;FF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasStrokeText),
                                                        },
                                                        {
                                                            .name = "nStrokeText",
                                                            .signature = "(JLjava/lang/String;FFF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasStrokeTextMaxWdith),
                                                        },
                                                        {
                                                            .name = "nMeasureText",
                                                            .signature = "(JLjava/lang/String;)F",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasMeasureTExt),
                                                        },
                                                        {
                                                            .name = "nSetGlobalAlpha",
                                                            .signature = "(JF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetGlobalAlpha),
                                                        },
                                                        {
                                                            .name = "nGetGlobalAlpha",
                                                            .signature = "(J)F",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasGetGlobalAlpha),
                                                        },
                                                        {
                                                            .name = "nSetGlobalCompositeOperation",
                                                            .signature = "(JLjava/lang/String;)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetGlobalCompositeOperation),
                                                        },
                                                        {
                                                            .name = "nSave",
                                                            .signature = "(J)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSave),
                                                        },
                                                        {
                                                            .name = "nRestore",
                                                            .signature = "(J)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasRestore),
                                                        },
                                                        {
                                                            .name = "nSetShadowColor",
                                                            .signature = "(JLjava/lang/String;)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetShadowColor),
                                                        },
                                                        {
                                                            .name = "nSetShadowBlur",
                                                            .signature = "(JI)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetShadowBlur),
                                                        },
                                                        {
                                                            .name = "nSetShadowOffsetX",
                                                            .signature = "(JF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetShadowOffsetX),
                                                        },
                                                        {
                                                            .name = "nSetShadowOffsetY",
                                                            .signature = "(JF)V",
                                                            .fnPtr = reinterpret_cast<void *>(CanvasSetShadowOffsetY),
                                                        }};
#endif // TARGET_STANDALONE

#if defined(TARGET_ALIPAY)

// static void NativeCanvasSwap(JNIEnv *env, jclass clazz, jlong view) {
//    ag::Canvas *canvas = reinterpret_cast<ag::Canvas *>(view);
//    canvas->swap();
//}
//
// static const JNINativeMethod native_canvas_methods[] = {
//        {
//    .name = "nSwap",
//    .signature = "(J)V",
//    .fnPtr = reinterpret_cast<void *>(NativeCanvasSwap),
//}
//};

#endif // TARGET_ALIPAY
static ScopedJavaGlobalRef<jclass> *gNativeCanvasProxyClass = nullptr;
static ScopedJavaGlobalRef<jclass> *gNativeChartProxyClass = nullptr;
static ScopedJavaGlobalRef<jclass> *gNativeFunctionProxyClass = nullptr;

//###################### F2Chart Begin ###################################
static jlong CreateNativeChart(JNIEnv *env, jclass clazz, jstring name, jdouble width, jdouble height, jdouble ratio) {
    std::string _name = JavaStringToString(env, name);
    F2_LOG_I("#CreateNativeChart", "create NativeChart:%s", _name.data());
    xg::XChart *chart = new xg::XChart(_name, width, height, ratio);
    return reinterpret_cast<jlong>(chart);
}

static jint SetChartCanvas(JNIEnv *env, jclass clazz, jlong chart, jlong view, jstring requestFrameHandleId) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    std::string _requestFrameHandleId = JavaStringToString(env, requestFrameHandleId);
    F2_LOG_I("#SetChartCanvas", "bind canvas requestFrameHandleId: %s", _requestFrameHandleId.data());
    _chart->SetRequestFrameFuncId(_requestFrameHandleId);

#if defined(TARGET_STANDALONE)
    F2CanvasView *chartView = reinterpret_cast<F2CanvasView *>(view);

    _chart->SetCanvasContext(chartView);
#elif defined(TARGET_ALIPAY)
    ag::Canvas *canvas = reinterpret_cast<ag::Canvas *>(view);
    auto ctx = (ag::CanvasRenderingContext2D *)canvas->getContext("2d");
    _chart->SetCanvasContext(ctx);
#endif

    return 0;
}

static jint SetChartSource(JNIEnv *env, jclass clazz, jlong chart, jstring source) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    std::string data = JavaStringToString(env, source);
    _chart->Source(std::move(data));
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartSource");
    return 0;
}

static jint SetChartPadding(JNIEnv *env, jclass clazz, jlong chart, jdouble left, jdouble top, jdouble right, jdouble bottom) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Padding(left, top, right, bottom);
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartPadding");
    return 0;
}

static jint SetChartMargin(JNIEnv *env, jclass clazz, jlong chart, jdouble left, jdouble top, jdouble right, jdouble bottom) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Margin(left, top, right, bottom);
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartMargin");
    return 0;
}

static jint SetChartScale(JNIEnv *env, jclass clazz, jlong chart, jstring field, jstring config) {
    std::string _field = JavaStringToString(env, field);
    std::string _config = JavaStringToString(env, config);

    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Scale(std::move(_field), std::move(_config));
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetScaleConfig");
    return 0;
}

static jint SetChartAxis(JNIEnv *env, jclass clazz, jlong chart, jstring field, jstring config) {
    std::string _field = JavaStringToString(env, field);
    std::string _config = JavaStringToString(env, config);

    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Axis(std::move(_field), std::move(_config));
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetAxisConfig");
    return 0;
}

static jint SetChartCoord(JNIEnv *env, jclass clazz, jlong chart, jstring config) {
    std::string _config = JavaStringToString(env, config);

    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Coord(std::move(_config));
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartCoord");
    return 0;
}

static jint SetChartAnimate(JNIEnv *env, jclass clazz, jlong chart, jstring config) {
    std::string _config = JavaStringToString(env, config);

    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Animate(std::move(_config));
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartAnimate");
    return 0;
}

static jint SetChartInteraction(JNIEnv *env, jclass clazz, jlong chart, jstring type, jstring config) {
    std::string _type = JavaStringToString(env, type);
    nlohmann::json cfg = nlohmann::json::parse(JavaStringToString(env, config), nullptr, false);

    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Interaction(_type, cfg);
    return 0;
}

static jint SetChartToolTip(JNIEnv *env, jclass clazz, jlong chart, jstring config) {
    std::string _config = JavaStringToString(env, config);

    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Tooltip(std::move(_config));
    return 0;
}

static jint SetChartLegend(JNIEnv *env, jclass clazz, jlong chart, jstring field, jstring config) {
    std::string _field = JavaStringToString(env, field);
    std::string _config = JavaStringToString(env, config);

    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    _chart->Legend(_field, std::move(_config));
    F2_LOG_I(_chart->GetChartName(), "%s", "#SetAxisConfig");
    return 0;
}

static jint SetChartGuideType(JNIEnv *env, jclass clazz, jlong chart, jstring type, jstring config) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);

    std::string _type = JavaStringToString(env, type);
    std::string _config = JavaStringToString(env, config);
    if(_type == "text") {
        _chart->Guide().Text(std::move(_config));
        F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartGuideText");
    } else if(_type == "flag") {
        _chart->Guide().Flag(std::move(_config));
        F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartGuideFlag");
    } else if(_type == "line") {
        _chart->Guide().Line(std::move(_config));
        F2_LOG_I(_chart->GetChartName(), "%s", "#SetChartGuideLine");
    }
    return 0;
}

static jint SendChartTouchEvent(JNIEnv *env, jclass clazz, jlong chart, jstring event) {
    std::string _event = JavaStringToString(env, event);
    bool ret = false;
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    ret = _chart->OnTouchEvent(std::move(_event));
    return ret ? 1 : 0;
}

static jlong CreateChartGeom(JNIEnv *env, jclass clazz, jlong chart, jstring type) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    std::string _type = JavaStringToString(env, type);
    xg::geom::AbstractGeom *geom = nullptr;
    if(_type == "line") {
        geom = &_chart->Line();
    } else if(_type == "area") {
        geom = &_chart->Area();
    } else if(_type == "interval") {
        geom = &_chart->Interval();
    } else if(_type == "point") {
        geom = &_chart->Point();
    } else if(_type == "candle") {
        geom = &_chart->Candle();
    }

    F2_LOG_I(_chart->GetChartName(), "createGeom: %s", _type.data());

    if(geom == nullptr) {
        return 0;
    }
    return reinterpret_cast<jlong>(geom);
}

static jstring GetChartRenderDumpInfo(JNIEnv *env, jclass clazz, jlong chart) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    F2_LOG_I(_chart->GetChartName(), "%s", "#:GetChartRenderDumpInfo");
    std::string dumpInfo = _chart->GetRenderInfo();
    return env->NewStringUTF(dumpInfo.data());
}

static jstring GetChartScaleTicks(JNIEnv *env, jclass clazz, jlong chart, jstring field) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    std::string _field = JavaStringToString(env, field);
    std::string ticks = _chart->GetScaleTicks(_field);
    return env->NewStringUTF(ticks.data());
}

static jint DestroyChart(JNIEnv *env, jclass clazz, jlong chart) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    F2_LOG_I(_chart->GetChartName(), "%s", "#destroy");
    delete _chart;
    return 0;
}

static jint ChartRender(JNIEnv *env, jclass clazz, jlong chart) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    F2_LOG_I(_chart->GetChartName(), "%s", "#render");
    _chart->Render();
    return 0;
}

static jdoubleArray ChartGetPosition(JNIEnv *env, jclass clazz, jlong chart, jstring itemData) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);

    std::string itemDataStr = JavaStringToString(env, itemData);
    nlohmann::json data = nlohmann::json::parse(itemDataStr, nullptr, false);
    const util::Point point = _chart->GetPosition(data);

    jdouble buf[] = {point.x, point.y};
    jdoubleArray rst = env->NewDoubleArray(2);
    env->SetDoubleArrayRegion(rst, 0, 2, &buf[0]);
    return rst;
}

static jint ChartClear(JNIEnv *env, jclass clazz, jlong chart) {
    xg::XChart *_chart = reinterpret_cast<xg::XChart *>(chart);
    F2_LOG_I(_chart->GetChartName(), "%s", "#clear");
    _chart->Clear();
    return 0;
}

static jint SetChartGeomPosition(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring fields) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _fields = JavaStringToString(env, fields);
    _geom->Position(_fields);
    return 0;
}

static jint SetChartGeomColors(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring field, jobjectArray colors) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _field = JavaStringToString(env, field);
    jsize colorsSize = env->GetArrayLength(colors);
    std::vector<std::string> _colors;
    for(int index = 0; index < colorsSize; ++index) {
        jstring color = (jstring)env->GetObjectArrayElement(colors, index);
        std::string _color = JavaStringToString(env, color);
        _colors.push_back(std::move(_color));
    }

    _geom->Color(_field, std::move(_colors));
    return 0;
}

static jint SetChartGeomColor(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring color) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _color = JavaStringToString(env, color);
    _geom->Color(_color);
    return 0;
}

static jint SetChartGeomSizes(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring field, jfloatArray sizes) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _field = JavaStringToString(env, field);
    std::vector<float> _sizes;
    jsize len = env->GetArrayLength(sizes);
    jfloat *array = env->GetFloatArrayElements(sizes, 0);
    for(int i = 0; i < len; ++i) {
        _sizes.push_back(array[i]);
    }
    _geom->Size(_field, std::move(_sizes));
    return 0;
}

static jint SetChartGeomSize(JNIEnv *env, jclass clazz, jlong geom, jstring type, jfloat size) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    _geom->Size(size);
    return 0;
}

static jint SetChartGeomIntervalTag(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring config) {
    std::string _type = JavaStringToString(env, type);
    if(_type != "interval") {
        return CODE_FAIL_UNKNOWN;
    }

    xg::geom::Interval *_interval = reinterpret_cast<xg::geom::Interval *>(geom);
    std::string _configStr = JavaStringToString(env, config);
    _interval->Tag(std::move(_configStr));
    return 0;
}

static jint SetChartGeomStyle(JNIEnv *env, jclass clazz, jlong geom, jstring config) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _configStr = JavaStringToString(env, config);
    _geom->Style(std::move(_configStr));
    return 0;
}

static jint SetChartGeomAttrs(JNIEnv *env, jclass clazz, jlong geom, jstring config) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _configStr = JavaStringToString(env, config);
    _geom->SetAttrs(std::move(_configStr));
    return 0;
}

static jint ExecuteCommand(JNIEnv *env, jclass clazz, jlong commandHandle) {
    xg::func::Command *command = reinterpret_cast<xg::func::Command *>(commandHandle);
    if(command != nullptr) {
        command->run();
        delete command;
    }
    return 0;
}

static jint DeallocCommand(JNIEnv *env, jclass clazz, jlong commandHandle) {
    xg::func::Command *command = reinterpret_cast<xg::func::Command *>(commandHandle);
    if(command != nullptr) {
        delete command;
    }
    return 0;
}

static jint SetChartGeomShapes(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring field, jobjectArray shapes) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _field = JavaStringToString(env, field);
    jsize len = env->GetArrayLength(shapes);
    std::vector<std::string> _shapes;
    for(int index = 0; index < len; ++index) {
        jstring shape = (jstring)env->GetObjectArrayElement(shapes, index);
        std::string _shape = JavaStringToString(env, shape);
        _shapes.push_back(std::move(_shape));
    }

    _geom->Shape(_field, std::move(_shapes));
    return 0;
}

static jint SetChartGeomShape(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring shape) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _shape = JavaStringToString(env, shape);
    _geom->Shape(_shape);
    return 0;
}

static jint SetChartGeomAdjust(JNIEnv *env, jclass clazz, jlong geom, jstring type, jstring adjust) {
    xg::geom::AbstractGeom *_geom = reinterpret_cast<xg::geom::AbstractGeom *>(geom);
    std::string _adjust = JavaStringToString(env, adjust);
    _geom->Adjust(_adjust);
    return 0;
}

static const JNINativeMethod native_chart_methods[] = {{
                                                           .name = "nCreateNativeChart",
                                                           .signature = "(Ljava/lang/String;DDD)J",
                                                           .fnPtr = reinterpret_cast<void *>(CreateNativeChart),
                                                       },
                                                       {
                                                           .name = "nSetCanvasView",
                                                           .signature = "(JJLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartCanvas),
                                                       },
                                                       {
                                                           .name = "nSetSource",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartSource),
                                                       },
                                                       {
                                                           .name = "nSetPadding",
                                                           .signature = "(JDDDD)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartPadding),
                                                       },
                                                       {
                                                           .name = "nSetMargin",
                                                           .signature = "(JDDDD)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartMargin),
                                                       },
                                                       {
                                                           .name = "nSetScale",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartScale),
                                                       },
                                                       {
                                                           .name = "nSetAxis",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartAxis),
                                                       },
                                                       {
                                                           .name = "nSetCoord",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartCoord),
                                                       },
                                                       {
                                                           .name = "nSetAnimate",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartAnimate),
                                                       },
                                                       {
                                                           .name = "nSetInteraction",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartInteraction),
                                                       },
                                                       {
                                                           .name = "nSetToolTip",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartToolTip),
                                                       },
                                                       {
                                                           .name = "nSetLegend",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartLegend),
                                                       },
                                                       {
                                                           .name = "nSetGuideType",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGuideType),
                                                       },
                                                       {
                                                           .name = "nSendTouchEvent",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SendChartTouchEvent),
                                                       },
                                                       {
                                                           .name = "nCreateGeom",
                                                           .signature = "(JLjava/lang/String;)J",
                                                           .fnPtr = reinterpret_cast<void *>(CreateChartGeom),
                                                       },
                                                       {
                                                           .name = "nGetRenderDumpInfo",
                                                           .signature = "(J)Ljava/lang/String;",
                                                           .fnPtr = reinterpret_cast<void *>(GetChartRenderDumpInfo),
                                                       },
                                                       {
                                                           .name = "nGetScaleTicks",
                                                           .signature = "(JLjava/lang/String;)Ljava/lang/String;",
                                                           .fnPtr = reinterpret_cast<void *>(GetChartScaleTicks),
                                                       },
                                                       {
                                                           .name = "nDestroy",
                                                           .signature = "(J)I",
                                                           .fnPtr = reinterpret_cast<void *>(DestroyChart),
                                                       },
                                                       {
                                                           .name = "nRender",
                                                           .signature = "(J)I",
                                                           .fnPtr = reinterpret_cast<void *>(ChartRender),
                                                       },
                                                       {
                                                           .name = "nGetPosition",
                                                           .signature = "(JLjava/lang/String;)[D",
                                                           .fnPtr = reinterpret_cast<void *>(ChartGetPosition),
                                                       },
                                                       {
                                                           .name = "nClear",
                                                           .signature = "(J)I",
                                                           .fnPtr = reinterpret_cast<void *>(ChartClear),
                                                       },
                                                       {
                                                           .name = "nGeomPosition",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomPosition),
                                                       },
                                                       {
                                                           .name = "nGeomColor",
                                                           .signature =
                                                               "(JLjava/lang/String;Ljava/lang/String;[Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomColors),
                                                       },
                                                       {
                                                           .name = "nGeomColor",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomColor),
                                                       },
                                                       {
                                                           .name = "nGeomSize",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;[F)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomSizes),
                                                       },
                                                       {
                                                           .name = "nGeomSize",
                                                           .signature = "(JLjava/lang/String;F)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomSize),
                                                       },
                                                       {
                                                           .name = "nGeomShape",
                                                           .signature =
                                                               "(JLjava/lang/String;Ljava/lang/String;[Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomShapes),
                                                       },
                                                       {
                                                           .name = "nGeomShape",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomShape),
                                                       },
                                                       {
                                                           .name = "nGeomAdjust",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomAdjust),
                                                       },
                                                       {
                                                           .name = "nGeomIntervalTag",
                                                           .signature = "(JLjava/lang/String;Ljava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomIntervalTag),
                                                       },
                                                       {
                                                           .name = "nGeomStyle",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomStyle),
                                                       },
                                                       {
                                                           .name = "nGeomAttrs",
                                                           .signature = "(JLjava/lang/String;)I",
                                                           .fnPtr = reinterpret_cast<void *>(SetChartGeomAttrs),
                                                       },
                                                       {
                                                           .name = "nExecuteCommand",
                                                           .signature = "(J)I",
                                                           .fnPtr = reinterpret_cast<void *>(ExecuteCommand),
                                                       },
                                                       {
                                                           .name = "nDeallocCommand",
                                                           .signature = "(J)I",
                                                           .fnPtr = reinterpret_cast<void *>(DeallocCommand),
                                                       }};
//###################### F2Chart END ###################################

static ScopedJavaGlobalRef<jclass> *gF2LogClass = nullptr;
static jmethodID gF2LogInfoMethod = nullptr;
static jmethodID gF2LogWarnMethod = nullptr;
static jmethodID gF2LogErrorMethod = nullptr;

static bool InitF2Log(JNIEnv *env) {
    if(gF2LogClass != nullptr) {
        return true;
    }

    jclass logClass = env->FindClass("com/antgroup/antv/f2/F2Log");

    if(logClass == nullptr) {
        return false;
    }

    gF2LogInfoMethod = env->GetStaticMethodID(logClass, "i", "(Ljava/lang/String;Ljava/lang/String;)V");
    gF2LogWarnMethod = env->GetStaticMethodID(logClass, "w", "(Ljava/lang/String;Ljava/lang/String;)V");
    gF2LogErrorMethod = env->GetStaticMethodID(logClass, "e", "(Ljava/lang/String;Ljava/lang/String;)V");
    if(HasException(env)) {
        return false;
    }

    gF2LogClass = new ScopedJavaGlobalRef<jclass>(env, logClass);

    return true;
}
//###################### F2Function Begin ###################################

static jstring nCreateFunction(JNIEnv *env, jclass clazz, jobject jhandle) {
    ScopedJavaGlobalRef<jobject> *handle = new ScopedJavaGlobalRef<jobject>(env, jhandle);
    xg::func::F2Function *function = new xg::jni::JavaF2Function(handle);
    xg::func::FunctionManager::GetInstance().Add(function);
    return StringToJString(env, function->functionId);
}

static void nFunctionBindChart(JNIEnv *env, jclass clazz, jstring jfunctionId, jlong jchart) {
    std::string functionId = JavaStringToString(env, jfunctionId);
    F2_LOG_I("#nFunctionBindChart", "functionId: %s", functionId.data());
    xg::func::F2Function *function = xg::func::FunctionManager::GetInstance().Find(functionId);

    if(function != nullptr) {
        xg::XChart *_chart = reinterpret_cast<xg::XChart *>(jchart);
        function->hostChartId = _chart->GetChartId();
    }
}

static const JNINativeMethod native_function_methods[] = {{
                                                              .name = "nCreateFunction",
                                                              .signature = "(Ljava/lang/Object;)Ljava/lang/String;",
                                                              .fnPtr = reinterpret_cast<void *>(nCreateFunction),
                                                          },
                                                          {
                                                              .name = "nBindChart",
                                                              .signature = "(Ljava/lang/String;J)V",
                                                              .fnPtr = reinterpret_cast<void *>(nFunctionBindChart),
                                                          }};

//###################### F2Function End ###################################
static bool
RegisterJNIInterface(JNIEnv *env, ScopedJavaGlobalRef<jclass> **holder, const char *class_path, const JNINativeMethod *methods, int array_size) {
    jclass clazz = env->FindClass(class_path);
    (*holder) = new ScopedJavaGlobalRef<jclass>(env, clazz);
    if((*holder)->isNull()) {
        F2_LOG_E("#RegisterJNIInterface", "can't find class %s", class_path);
        return false;
    }
    if(env->RegisterNatives((*holder)->obj(), methods, array_size) != 0) {
        F2_LOG_E("#RegisterJNIInterface", "%s", "Register JNI methods failed.");
        return false;
    }
    F2_LOG_I("#RegisterJNIInterface", "%s", "Register jni success");
    return true;
}

void InnerLog(int level, std::string traceId, const char *fmt, ...) {
    std::string _tag = "jni|" + traceId;

    char msg[2048] = {0};
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, 2048, fmt, args);
    va_end(args);

    jmethodID method = gF2LogInfoMethod;
    if(level == F2_LOG_WARN) {
        method = gF2LogWarnMethod;
    } else if(level == F2_LOG_ERROR) {
        method = gF2LogErrorMethod;
    }

    if(gF2LogClass == nullptr || gF2LogClass->isNull() || method == nullptr) {
        native_clog(level, _tag.data(), msg);
        return;
    }

    JNIEnv *env = GetJniEnvSafe();
    if(env == nullptr) {
        return;
    }
    ScopedJavaLocalRef<jstring> tagStr = StringToJavaString(env, _tag);
    ScopedJavaLocalRef<jstring> msgStr = StringToJavaString(env, msg);
    env->CallStaticVoidMethod(gF2LogClass->obj(), method, tagStr.obj(), msgStr.obj());
}

static bool OnJniLoad(JNIEnv *env) {
    if(!InitF2Log(env)) {
        return false;
    }

    F2_LOG_I("#OnJniLoad", "%s", "init log success");

#if defined(TARGET_STANDALONE)
    if(!RegisterJNIInterface(env, &gNativeCanvasProxyClass, "com/antgroup/antv/f2/NativeCanvasProxy", native_canvas_methods,
                             xg_jni_arraysize(native_canvas_methods))) {
        return false;
    }
#elif defined(TARGET_ALIPAY)
//    if(!RegisterJNIInterface(env, &gNativeCanvasProxyClass, "com/antgroup/antv/f2/F2CanvasView", native_canvas_methods,
//                             xg_jni_arraysize(native_canvas_methods))) {
//        return false;
//    }
#endif
    F2_LOG_I("#OnJniLoad", "%s", "register canvas view success");

    if(!RegisterJNIInterface(env, &gNativeChartProxyClass, "com/antgroup/antv/f2/NativeChartProxy", native_chart_methods,
                             xg_jni_arraysize(native_chart_methods))) {
        return false;
    }
    F2_LOG_I("#OnJniLoad", "%s", "register f2 chart success");

    if(!RegisterJNIInterface(env, &gNativeFunctionProxyClass, "com/antgroup/antv/f2/F2Function", native_function_methods,
                             xg_jni_arraysize(native_function_methods))) {
        return false;
    }
    F2_LOG_I("#OnJniLoad", "%s", "register f2function success");

    return true;
}

} // namespace jni
} // namespace xg

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    F2_LOG_I("#JNI_OnLoad", "%s", "JNI_OnLoad Start...");
    JavaVM *old = xg::jni::GetJVM();
    if(old == vm) {
        F2_LOG_I("#JNI_OnLoad", "%s", "JNI_OnLoad dupliation vm, skip OnLoad");
        return JNI_VERSION_1_4;
    }

    xg::jni::InitJavaVM(vm);

    JNIEnv *env = xg::jni::AttachCurrentThread();
    F2_LOG_I("#JNI_OnLoad", "%s", "AttachCurrentThread FINISH");
    bool result = xg::jni::OnJniLoad(env);

    if(!result) {
        F2_LOG_E("#JNI_OnLoad", "%s", "JNI_OnLoad FAIL");
    }
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) { F2_LOG_I("#JNI_OnUnload", "%s", "JNI_OnUnload Finished"); }
