//
// Created by weiqing.twq on 2022/1/4.
//

#if defined(__ANDROID__)

#include "BitmapCanvasContext.h"

#define F2SafeCallVoidMethod(env, obj, method, ...) if(env && obj && method) {env->CallVoidMethod(obj, method, __VA_ARGS__);}
#define F2SafeCallVoidMethodNoArg(env, obj, method) if(env && obj && method) {env->CallVoidMethod(obj, method);}
#define F2SafeCallFloatMethod(env, obj, method, ...) if(env && obj && method) {return env->CallFloatMethod(obj, method, __VA_ARGS__);} else { return 0;}

using namespace xg::jni;
using namespace xg::canvas;

ScopedJavaLocalRef<jfloatArray> BitmapCanvasContext::GetGradientPositions(JNIEnv *env, const CanvasGradient &gradient) {
    std::vector<float> positions;
    std::for_each(gradient.colorStops.begin(), gradient.colorStops.end(),
                  [&](const ColorStop &item) -> void { positions.push_back(item.offset); });
    return VectorToJFloatArray(env, positions);
}

ScopedJavaLocalRef<jintArray> BitmapCanvasContext::GetGradientColors(JNIEnv *env, const CanvasGradient &gradient) {
    std::vector<int> colors;
    std::for_each(gradient.colorStops.begin(), gradient.colorStops.end(), [&](const ColorStop &item) -> void {
        CanvasColor rgba;
        CanvasColorParser::Parse(item.color, rgba);
        colors.push_back(CanvasColorParser::RGBAToHex(rgba));
    });
    return VectorToJIntArray(env, colors);
}

BitmapCanvasContext::BitmapCanvasContext(jobject canvasContext, float devicePixelRatio)
    : CanvasContext(devicePixelRatio, nullptr) {
    //多线程的情况下不能缓存 因为env和线程相关需要在使用的地方调用GetJniEnvSafe方法中获取
    env_ = GetJniEnvSafe();
    canvasContext_ = new ScopedJavaGlobalRef<jobject>(env_, canvasContext);
    jclass contextCls = env_->GetObjectClass(canvasContext);
    if(env_->ExceptionCheck()) {return;}
    lineTo_ = env_->GetMethodID(contextCls, "lineTo", "(FF)V");
    F2ASSERT(lineTo_, "lineTo_ method is null");
    if(env_->ExceptionCheck()) {return;}
    moveTo_ = env_->GetMethodID(contextCls, "moveTo", "(FF)V");
    F2ASSERT(moveTo_, "moveTo_ method is null");
    if(env_->ExceptionCheck()) {return;}
    stroke_ = env_->GetMethodID(contextCls, "stroke", "()V");
    F2ASSERT(stroke_, "stroke_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setStrokeStyle_ = env_->GetMethodID(contextCls, "setStrokeStyle", "(I)V");
    F2ASSERT(setStrokeStyle_, "setStrokeStyle_ method is null");
    if(env_->ExceptionCheck()) {return;}
    fill_ = env_->GetMethodID(contextCls, "fill", "()V");
    F2ASSERT(fill_, "fill_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setFillStyle_ = env_->GetMethodID(contextCls, "setFillStyle", "(I)V");
    F2ASSERT(setFillStyle_, "setFillStyle_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setLineWidth_ = env_->GetMethodID(contextCls, "setLineWidth", "(F)V");
    F2ASSERT(setLineWidth_, "setLineWidth_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setGlobalAlpha_ = env_->GetMethodID(contextCls, "setGlobalAlpha", "(F)V");
    F2ASSERT(setGlobalAlpha_, "setGlobalAlpha_ method is null");
    if(env_->ExceptionCheck()) {return;}
    getGlobalAlpha_ = env_->GetMethodID(contextCls, "getGlobalAlpha", "()F");
    F2ASSERT(getGlobalAlpha_, "getGlobalAlpha_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setFont_ = env_->GetMethodID(contextCls, "setFont", "(IIILjava/lang/String;F)V");
    F2ASSERT(setFont_, "setFont_ method is null");
    if(env_->ExceptionCheck()) {return;}
    fillText_ = env_->GetMethodID(contextCls, "fillText", "(Ljava/lang/String;FF)V");
    F2ASSERT(fillText_, "fillText_ method is null");
    if(env_->ExceptionCheck()) {return;}
    strokeText_ = env_->GetMethodID(contextCls, "strokeText", "(Ljava/lang/String;FF)V");
    F2ASSERT(strokeText_, "strokeText_ method is null");
    if(env_->ExceptionCheck()) {return;}
    getTextAlign_ = env_->GetMethodID(contextCls, "getTextAlign", "()Ljava/lang/String;");
    F2ASSERT(getTextAlign_, "getTextAlign_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setTextAlign_ = env_->GetMethodID(contextCls, "setTextAlign", "(Ljava/lang/String;)V");
    F2ASSERT(setTextAlign_, "setTextAlign_ method is null");
    if(env_->ExceptionCheck()) {return;}
    getTextBaseline_ = env_->GetMethodID(contextCls, "getTextBaseline", "()Ljava/lang/String;");
    F2ASSERT(getTextBaseline_, "getTextBaseline_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setTextBaseline_ = env_->GetMethodID(contextCls, "setTextBaseline", "(Ljava/lang/String;)V");
    F2ASSERT(setTextBaseline_, "setTextBaseline_ method is null");
    if(env_->ExceptionCheck()) {return;}
    strokeRect_ = env_->GetMethodID(contextCls, "strokeRect", "(FFFF)V");
    F2ASSERT(strokeRect_, "strokeRect_ method is null");
    if(env_->ExceptionCheck()) {return;}
    save_ = env_->GetMethodID(contextCls, "save", "()V");
    F2ASSERT(save_, "save_ method is null");
    if(env_->ExceptionCheck()) {return;}
    restore_ = env_->GetMethodID(contextCls, "restore", "()V");
    F2ASSERT(restore_, "restore_ method is null");
    if(env_->ExceptionCheck()) {return;}
    measureTextWidth_ = env_->GetMethodID(contextCls, "measureText", "(Ljava/lang/String;)F");
    F2ASSERT(measureTextWidth_, "measureTextWidth_ method is null");
    if(env_->ExceptionCheck()) {return;}
    transform_ = env_->GetMethodID(contextCls, "transform", "(FFFFFF)V");
    F2ASSERT(transform_, "transform_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setTransform_ = env_->GetMethodID(contextCls, "setTransform", "(FFFFFF)V");
    F2ASSERT(setTransform_, "setTransform_ method is null");
    if(env_->ExceptionCheck()) {return;}
    rect_ = env_->GetMethodID(contextCls, "rect", "(FFFF)V");
    F2ASSERT(rect_, "rect_ method is null");
    if(env_->ExceptionCheck()) {return;}
    clearRect_ = env_->GetMethodID(contextCls, "clearRect", "(FFFF)V");
    F2ASSERT(clearRect_, "clearRect_ method is null");
    if(env_->ExceptionCheck()) {return;}
    fillRect_ = env_->GetMethodID(contextCls, "fillRect", "(FFFF)V");
    F2ASSERT(fillRect_, "fillRect_ method is null");
    if(env_->ExceptionCheck()) {return;}
    beginPath_ = env_->GetMethodID(contextCls, "beginPath", "()V");
    F2ASSERT(beginPath_, "beginPath_ method is null");
    if(env_->ExceptionCheck()) {return;}
    closePath_ = env_->GetMethodID(contextCls, "closePath", "()V");
    F2ASSERT(closePath_, "closePath_ method is null");
    if(env_->ExceptionCheck()) {return;}
    quadraticCurveTo_ = env_->GetMethodID(contextCls, "quadraticCurveTo", "(FFFF)V");
    F2ASSERT(quadraticCurveTo_, "quadraticCurveTo_ method is null");
    if(env_->ExceptionCheck()) {return;}
    bezierCurveTo_ = env_->GetMethodID(contextCls, "bezierCurveTo", "(FFFFFF)V");
    F2ASSERT(bezierCurveTo_, "bezierCurveTo_ method is null");
    if(env_->ExceptionCheck()) {return;}
    arc_ = env_->GetMethodID(contextCls, "arc", "(FFFFFZ)V");
    F2ASSERT(arc_, "arc_ method is null");
    if(env_->ExceptionCheck()) {return;}
    arcTo_ = env_->GetMethodID(contextCls, "arcTo", "(FFFFF)V");
    F2ASSERT(arcTo_, "arcTo_ method is null");
    if(env_->ExceptionCheck()) {return;}
    scale_ = env_->GetMethodID(contextCls, "scale", "(FF)V");
    F2ASSERT(scale_, "scale_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setLineDash_ = env_->GetMethodID(contextCls, "setLineDash", "([F)V");
    F2ASSERT(setLineDash_, "setLineDash_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setLinearGradient_ = env_->GetMethodID(contextCls, "setLinearGradient", "(FFFF[I[F)V");
    F2ASSERT(setLinearGradient_, "setLinearGradient_ method is null");
    if(env_->ExceptionCheck()) {return;}
    setRadialGradient_ = env_->GetMethodID(contextCls, "setRadialGradient", "(FFFFFF[I[F)V");
    F2ASSERT(setRadialGradient_, "setRadialGradient_ method is null");
    if(env_->ExceptionCheck()) {return;}
    rotate_ = env_->GetMethodID(contextCls, "rotate", "(F)V");
    F2ASSERT(rotate_, "rotate_ method is null");
    if(env_->ExceptionCheck()) {return;}
    clip_ = env_->GetMethodID(contextCls, "clip", "()V");
    F2ASSERT(clip_, "clip_ method is null");
    if(env_->ExceptionCheck()) {return;}
}

BitmapCanvasContext::~BitmapCanvasContext() {
    if(canvasContext_) {
        delete canvasContext_;
        canvasContext_ = nullptr;
    }
}

bool BitmapCanvasContext::IsValid() { return canvasContext_->obj() != nullptr; };

void BitmapCanvasContext::SetFillStyle(const std::string &color) {
    TraceCommand("SetFillStyle: " + color);
    if(color != fillColorCache_ && CanvasColorParser::Parse(color, fillColor_)) {
        fillColorCache_ = color;
    }
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), setFillStyle_, CanvasColorParser::RGBAToHex(fillColor_));
}

void BitmapCanvasContext::SetFillStyle(const CanvasFillStrokeStyle &style) {
    env_ = GetJniEnvSafe();
    switch(style.type) {
        case CanvasFillStrokeStyleType::kColor: {
            this->SetFillStyle(style.color);
        } break;
        case CanvasFillStrokeStyleType::kLinearGradient: {
            TraceCommand("SetFillStyle setLinearGradient_");
            F2SafeCallVoidMethod(env_, canvasContext_->obj(), setLinearGradient_, std::get<0>(style.linearGradient.start),
                                 std::get<1>(style.linearGradient.start), std::get<0>(style.linearGradient.end),
                                 std::get<1>(style.linearGradient.end), GetGradientColors(env_, style.linearGradient).obj(),
                                 GetGradientPositions(env_, style.linearGradient).obj());
            break;
        }
        case CanvasFillStrokeStyleType::kRadialGradient: {
            TraceCommand("SetFillStyle setRadialGradient_");
            F2SafeCallVoidMethod(env_, canvasContext_->obj(), setRadialGradient_, std::get<0>(style.radialGradient.start),
                                 std::get<1>(style.radialGradient.start), std::get<2>(style.radialGradient.start),
                                 std::get<0>(style.radialGradient.end), std::get<1>(style.radialGradient.end),
                                 std::get<2>(style.radialGradient.end), GetGradientColors(env_, style.radialGradient).obj(),
                                 GetGradientPositions(env_, style.radialGradient).obj());
            break;
        }
        default:
            break;
    }
}

void BitmapCanvasContext::SetStrokeStyle(const std::string &color) {
    TraceCommand("SetStrokeStyle: " + color);
    if(color != strokeColorCache_ && CanvasColorParser::Parse(color, strokeColor_)) {
        strokeColorCache_ = color;
    }
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), setStrokeStyle_, CanvasColorParser::RGBAToHex(strokeColor_));
}

void BitmapCanvasContext::SetStrokeStyle(const CanvasFillStrokeStyle &style) {
    TraceCommand("SetStrokeStyle style");
    env_ = GetJniEnvSafe();
    switch(style.type) {
        case CanvasFillStrokeStyleType::kColor: {

            this->SetStrokeStyle(style.color);
        } break;
        case CanvasFillStrokeStyleType::kLinearGradient: {
            TraceCommand("SetStrokeStyle setLinearGradient_");
            F2SafeCallVoidMethod(env_, canvasContext_->obj(), setLinearGradient_, std::get<0>(style.linearGradient.start),
                                 std::get<1>(style.linearGradient.start), std::get<0>(style.linearGradient.end),
                                 std::get<1>(style.linearGradient.end), GetGradientColors(env_, style.linearGradient).obj(),
                                 GetGradientPositions(env_, style.linearGradient).obj());
            break;
        }
        case CanvasFillStrokeStyleType::kRadialGradient: {
            TraceCommand("SetStrokeStyle setRadialGradient_");
            F2SafeCallVoidMethod(env_, canvasContext_->obj(), setRadialGradient_, std::get<0>(style.radialGradient.start),
                                 std::get<1>(style.radialGradient.start), std::get<2>(style.radialGradient.start),
                                 std::get<0>(style.radialGradient.end), std::get<1>(style.radialGradient.end),
                                 std::get<2>(style.radialGradient.end), GetGradientColors(env_, style.radialGradient).obj(),
                                 GetGradientPositions(env_, style.radialGradient).obj());
            break;
        }
        default:

            break;
    }
}

void BitmapCanvasContext::SetLineCap(const std::string &lineCap) {}

void BitmapCanvasContext::SetLineJoin(const std::string &lineJoin) {}

void BitmapCanvasContext::SetLineWidth(float lineWidth) {
    TraceCommand("SetLineWidth: " + std::to_string(lineWidth));
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), setLineWidth_, lineWidth);
}

void BitmapCanvasContext::SetLineDashOffset(float v) {}

void BitmapCanvasContext::SetLineDash(const std::vector<float> &params) {
    TraceCommand("SetLineDash ");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), setLineDash_, VectorToJFloatArray(env_, params).obj());
}

void BitmapCanvasContext::SetMiterLimit(float limit) {}

void BitmapCanvasContext::SetGlobalAlpha(float globalAlpha) {
    TraceCommand("SetGlobalAlpha: " + std::to_string(globalAlpha));
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), setGlobalAlpha_, globalAlpha);
}

float BitmapCanvasContext::GlobalAlpha() { return globalAlpha_; }

void BitmapCanvasContext::SetFont(const std::string &font) {
    TraceCommand("SetFont: " + font);
    if(font != fontStyleCache_ && CanvasFontParser::Parse(font, fontStyle_)) {
        fontStyleCache_ = font;
    }
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), setFont_, fontStyle_.fontStyle, fontStyle_.fontVariant,
                         fontStyle_.fontWeight, jni::StringToJavaString(env_, fontStyle_.fontFamily).obj(), fontStyle_.fontSize);


}

void BitmapCanvasContext::FillText(const std::string &text, float x, float y, float maxWidth) {
    TraceCommand("FillText: " + text);
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), fillText_, jni::StringToJavaString(env_, text).obj(), x, y);
}

void BitmapCanvasContext::StrokeText(const std::string &text, float x, float y, float maxWidth) {
    TraceCommand("StrokeText: " + text);
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), strokeText_, jni::StringToJavaString(env_, text).obj(), x, y);
}

std::string BitmapCanvasContext::TextAlign() const { return textAlign_; }

void BitmapCanvasContext::SetTextAlign(const std::string &textAlign) {
    TraceCommand("SetTextAlign " + textAlign);
    textAlign_ = textAlign;
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), setTextAlign_, jni::StringToJavaString(env_, textAlign_).obj());
}

std::string BitmapCanvasContext::TextBaseline() const { return textBaseline_; }

void BitmapCanvasContext::SetTextBaseline(const std::string &textBaseline) {
    TraceCommand("SetTextBaseline " + textBaseline);
    textBaseline_ = textBaseline;
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), setTextBaseline_, jni::StringToJavaString(env_, textBaseline_).obj());
}

void BitmapCanvasContext::StrokeRect(float x, float y, float width, float height) {
    TraceCommand("StrokeRect");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), strokeRect_, x, y, width, height);
}

void BitmapCanvasContext::Save() {
    TraceCommand("Save");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethodNoArg(env_, canvasContext_->obj(), save_);
}

void BitmapCanvasContext::Restore() {
    TraceCommand("Restore");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethodNoArg(env_, canvasContext_->obj(), restore_);
}

bool BitmapCanvasContext::HasClip() { return hasClip_; }

void BitmapCanvasContext::SetShadowOffsetX(float v) {}

void BitmapCanvasContext::SetShadowOffsetY(float v) {}

void BitmapCanvasContext::SetShadowColor(const char *v) {}

void BitmapCanvasContext::SetShadowBlur(float v) {}

float BitmapCanvasContext::MeasureTextWidth(const std::string &text) {
    TraceCommand("MeasureTextWidth: " + text);
    env_ = GetJniEnvSafe();
    F2SafeCallFloatMethod(env_, canvasContext_->obj(), measureTextWidth_, jni::StringToJavaString(env_, text).obj());
}

void BitmapCanvasContext::Transform(float a, float b, float c, float d, float e, float f) {
    TraceCommand("Transform " );
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), transform_, a, b, c, d, e, f);
}

void BitmapCanvasContext::SetTransform(float a, float b, float c, float d, float e, float f) {
    TraceCommand("SetTransform " );
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), setTransform_, a, b, c, d, e, f);
}

void BitmapCanvasContext::Rect(float x, float y, float width, float height) {
    TraceCommand("Rect " );
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), rect_, x, y, width, height);
}

void BitmapCanvasContext::ClearRect(float x, float y, float width, float height) {
    TraceCommand("ClearRect " );
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), clearRect_, x, y, width, height);
}

void BitmapCanvasContext::FillRect(float x, float y, float width, float height) {
    TraceCommand("FillRect " );
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), fillRect_, x, y, width, height);
}

void BitmapCanvasContext::Fill(const std::string &fillRule) {
    TraceCommand("Fill");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethodNoArg(env_, canvasContext_->obj(), fill_);
}

void BitmapCanvasContext::Stroke() {
    TraceCommand("Stroke");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethodNoArg(env_, canvasContext_->obj(), stroke_);
}

void BitmapCanvasContext::BeginPath() {
    TraceCommand("BeginPath");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethodNoArg(env_, canvasContext_->obj(), beginPath_);
}

void BitmapCanvasContext::MoveTo(float x, float y) {
    TraceCommand("MoveTo x: " + std::to_string(x) + ", " + std::to_string(y));
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), moveTo_, x, y);
}

void BitmapCanvasContext::ClosePath() {
    TraceCommand("ClosePath");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethodNoArg(env_, canvasContext_->obj(), closePath_);
}

void BitmapCanvasContext::LineTo(float x, float y) {
    TraceCommand("LienTo x: " + std::to_string(x) + ", " + std::to_string(y));
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), lineTo_, x, y);
}

void BitmapCanvasContext::Clip(const std::string &fillRule) {
    TraceCommand("Clip ");
    hasClip_ = true;
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethodNoArg(env_, canvasContext_->obj(), clip_);
}

void BitmapCanvasContext::QuadraticCurveTo(float cpx, float cpy, float x, float y) {
    TraceCommand("QuadraticCurveTo");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), quadraticCurveTo_, cpx, cpy, x, y);
}

void BitmapCanvasContext::BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    TraceCommand("BezierCurveTo");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), bezierCurveTo_, cp1x, cp1y, cp2x, cp2y, x, y);
}

void BitmapCanvasContext::Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise) {
    TraceCommand("Arc");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), arc_, x, y, r, sAngle, eAngle, antiClockwise);
}

void BitmapCanvasContext::ArcTo(float x1, float y1, float x2, float y2, float r) {
    TraceCommand("ArcTo");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), arcTo_, x1, y1, x2, y2, r);
}

void BitmapCanvasContext::Scale(float scale_width, float scale_height) {
    TraceCommand("Scale");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), scale_, scale_width);
}

void BitmapCanvasContext::Rotate(float angle) {
    TraceCommand("Rotate");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), rotate_, angle);
}

void BitmapCanvasContext::Translate(float x, float y) {
    TraceCommand("Translate");
    env_ = GetJniEnvSafe();
    F2SafeCallVoidMethod(env_, canvasContext_->obj(), translate_, x, y);
}

void BitmapCanvasContext::DrawImage(CanvasImage *image, float dx, float dy) {}

void BitmapCanvasContext::DrawImage(CanvasImage *image, float dx, float dy, float width, float height) {}

#endif // ANDROID