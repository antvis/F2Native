
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
#include "android/F2CanvasView.h"
#else
#include <gcanvas/GCanvas2dContext.h>
#include <gcanvas/GConvert.h>
#endif
#endif // TARGET_ALIPAY

#if defined(TARGET_ALIPAY)
#include <AntGraphic/AntGraphic.h>
#endif // TARGET_ALIPAY

#include <utils/Tracer.h>
#include <vector>

#ifndef XG_GRAPHICS_CANVAS_CONTEXT_H
#define XG_GRAPHICS_CANVAS_CONTEXT_H

namespace xg {
namespace canvas {

/**
 *  Android 和 iOS 的 Canvas 承载对象不一样，在此抹平差异
 */
class CanvasContext {
  public:
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
    F2CanvasView *canvasContext_ = nullptr;
#else
    GCanvasContext *canvasContext_ = nullptr;
#endif
#elif defined(TARGET_ALIPAY)
    ag::CanvasRenderingContext2D *canvasContext_ = nullptr;
#endif

  public:
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
    CanvasContext(F2CanvasView *_canvasContext, float devicePixelRatio, utils::Tracer *tracer)
        : canvasContext_(_canvasContext), devicePixelRatio_(devicePixelRatio), tracer_(tracer) {}
#else
    CanvasContext(GCanvasContext *_canvasContext, float devicePixelRatio, utils::Tracer *tracer)
        : canvasContext_(_canvasContext), devicePixelRatio_(devicePixelRatio), tracer_(tracer) {}
#endif
#elif defined(TARGET_ALIPAY)
    CanvasContext(ag::CanvasRenderingContext2D *_canvasContext, float devicePixelRatio, utils::Tracer *tracer)
        : canvasContext_(_canvasContext), devicePixelRatio_(devicePixelRatio), tracer_(tracer) {}
#endif

    ~CanvasContext() {
        canvasContext_ = nullptr;
        tracer_ = nullptr;
    }

    bool IsValid() { return canvasContext_ != nullptr; }

    void SetFillStyle(const std::string &color) {
        this->AppendCommand("SetFillStyle: " + color);
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        canvasContext_->SetFillStyle(color);
#else
        GColorRGBA _color = gcanvas::StrValueToColorRGBA(color.c_str());
        canvasContext_->SetFillStyle(_color);
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->setFillStyle(color);
#endif
    }

    void SetStrokeStyle(const std::string &color) {
        this->AppendCommand("SetStrokeStyle: " + color);
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        canvasContext_->SetStrokeStyle(color);
#else
        GColorRGBA _color = gcanvas::StrValueToColorRGBA(color.c_str());
        canvasContext_->SetStrokeStyle(_color);
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->setStrokeStyle(color);
#endif
    }

    void SetLineCap(const std::string &lineCap) {
        this->AppendCommand("SetLineCap: " + lineCap);
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        std::string _lineCap = lineCap;
        canvasContext_->SetLineCap(std::forward<std::string>(_lineCap));
#else
        canvasContext_->SetLineCap(lineCap.data());
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->setLineCap(lineCap);
#endif
    }

    void SetLineJoin(const std::string &lineJoin) {
        this->AppendCommand("SetLineJoin: " + lineJoin);
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        std::string _lineJoin = lineJoin;
        canvasContext_->SetLineJoin(std::forward<std::string>(_lineJoin));
#else
        canvasContext_->SetLineJoin(lineJoin.data());
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->setLineJoin(lineJoin);
#endif
    }

    void SetLineWidth(float lineWidth) {
        this->AppendCommand("SetLineWidth: " + std::to_string(lineWidth));
#if defined(TARGET_STANDALONE)
        canvasContext_->SetLineWidth(lineWidth);
#elif defined(TARGET_ALIPAY)
        canvasContext_->setLineWidth(lineWidth);
#endif
    }

    void SetLineDashOffset(float v) {
        this->AppendCommand("SetLineDashOffset: " + std::to_string(v));
#if defined(TARGET_STANDALONE)
        canvasContext_->SetLineDashOffset(v);
#elif defined(TARGET_ALIPAY)
        canvasContext_->setLineDashOffset(v);
#endif
    }

    void SetLineDash(std::vector<float> params) {
        this->AppendCommand("SetLineDash: vec{}");
#if defined(TARGET_STANDALONE)
        canvasContext_->SetLineDash(params);
#elif defined(TARGET_ALIPAY)
        canvasContext_->setLineDash(params);
#endif
    }

    void SetMiterLimit(float limit) {
        this->AppendCommand("SetMiterLimit: " + std::to_string(limit));
#if defined(TARGET_STANDALONE)
        canvasContext_->SetMiterLimit(limit);
#elif defined(TARGET_ALIPAY)
        canvasContext_->setMiterLimit(limit);
#endif
    }

    //    void SetGlobalCompositeOp(const std::string &op) {
    //#ifdef ANDROID
    //        canvasContext_->SetGlobalCompositeOp(op);
    //#else
    //    const char *pp = op.data();
    //    GCompositeOperation gop = gcanvas::StringToGlobalCompositeOp(pp);
    //    if (gop != GCompositeOperation::COMPOSITE_OP_NONE) {
    //        canvasContext_->SetGlobalCompositeOperation(gop);
    //    }
    //#endif
    //    }

    void SetGlobalAlpha(float globalAlpha) {
        this->AppendCommand("SetGlobalAlpha: " + std::to_string(globalAlpha));
#if defined(TARGET_STANDALONE)
        canvasContext_->SetGlobalAlpha(globalAlpha);
#elif defined(TARGET_ALIPAY)
        canvasContext_->setGlobalAlpha(globalAlpha);
#endif
    }

    float GlobalAlpha() {
#if defined(TARGET_STANDALONE)
        return canvasContext_->GlobalAlpha();
#elif defined(TARGET_ALIPAY)
        return canvasContext_->getGlobalAlpha();
#endif
        return .0;
    }

    void SetFont(const std::string &font) {
        this->AppendCommand("SetFont: " + font);
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        std::string _font = font;
        canvasContext_->SetFont(std::forward<std::string>(_font));
#else
        canvasContext_->SetFont(font.data());
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->setFont(font);
#endif
    }

    void FillText(const std::string &text, float x, float y, float maxWidth = SHRT_MAX) {
        this->AppendCommand("FillText: " + text);
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        std::string _text = text;
        canvasContext_->FillText(_text, x, y, maxWidth);
#else
        if(isnan(maxWidth) || maxWidth <= 0) {
            return;
        }
        canvasContext_->DrawText(text.c_str(), x, y, maxWidth);
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->fillText(text, x, y, maxWidth);
#endif
    }

    void StrokeText(std::string &text, float x, float y, float maxWidth) {
        this->AppendCommand("StrokeText: " + text);
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        canvasContext_->StrokeText(text, x, y, maxWidth);
#else
        canvasContext_->StrokeText(text.c_str(), x, y, maxWidth);
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->strokeText(text, x, y, maxWidth);
#endif
    }

    std::string TextAlign() const {
#if defined(TARGET_STANDALONE)
        GTextAlign textAlign = canvasContext_->TextAlign();
        return TextAlignToString(textAlign);
#elif defined(TARGET_ALIPAY)
        return canvasContext_->getTextAlign();
#endif
    }

#if defined(TARGET_STANDALONE)
    void SetTextAlign(GTextAlign &textAlign) { canvasContext_->SetTextAlign(textAlign); }
#endif

    void SetTextAlign(const std::string &textAlign) {
        this->AppendCommand("SetTextAlign ");
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        canvasContext_->SetTextAlign(textAlign);
#else
        bool validType = true;
        GTextAlign textAlignEnum;
        if(textAlign == "start") {
            textAlignEnum = TEXT_ALIGN_START;
        } else if(textAlign == "end") {
            textAlignEnum = TEXT_ALIGN_END;
        } else if(textAlign == "center") {
            textAlignEnum = TEXT_ALIGN_CENTER;
        } else if(textAlign == "left") {
            textAlignEnum = TEXT_ALIGN_LEFT;
        } else if(textAlign == "right") {
            textAlignEnum = TEXT_ALIGN_RIGHT;
        } else {
            validType = false;
        }
        if(validType) {
            canvasContext_->SetTextAlign(textAlignEnum);
        }
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->setTextAlign(textAlign);
#endif
    }

    std::string TextBaseline() const {
#if defined(TARGET_STANDALONE)
        GTextBaseline baseLine = canvasContext_->TextBaseline();
        return TextBaselineToString(baseLine);
#elif defined(TARGET_ALIPAY)
        return canvasContext_->getTextBaseline();
#endif
    }

#if defined(TARGET_STANDALONE)
    void SetTextBaseline(GTextBaseline &textBaseline) {
        this->AppendCommand("SetTextBaseline");
        canvasContext_->SetTextBaseline(textBaseline);
    }
#endif

    void SetTextBaseline(const std::string &textBaseline) {
        this->AppendCommand("SetTextBaseline");
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        canvasContext_->SetTextBaseline(textBaseline);
#else
        GTextBaseline baselineEnum;
        bool valid = true;

        if(textBaseline == "alphabetic") {
            baselineEnum = TEXT_BASELINE_ALPHABETIC;
        } else if(textBaseline == "top") {
            baselineEnum = TEXT_BASELINE_TOP;
        } else if(textBaseline == "hanging") {
            baselineEnum = TEXT_BASELINE_HANGING;
        } else if(textBaseline == "middle") {
            baselineEnum = TEXT_BASELINE_MIDDLE;
        } else if(textBaseline == "ideographic") {
            baselineEnum = TEXT_BASELINE_IDEOGRAPHIC;
        } else if(textBaseline == "bottom") {
            baselineEnum = TEXT_BASELINE_BOTTOM;
        } else {
            valid = false;
        }
        if(valid) {
            canvasContext_->SetTextBaseline(baselineEnum);
        }
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->setTextBaseline(textBaseline);
#endif
    }

    void StrokeRect(float x, float y, float width, float height) {
        this->AppendCommand("StrokeRect");
#if defined(TARGET_STANDALONE)
        canvasContext_->StrokeRect(x, y, width, height);
#elif defined(TARGET_ALIPAY)
        canvasContext_->strokeRect(x, y, width, height);
#endif
    }

    void Save() {
        this->AppendCommand("Save");
#if defined(TARGET_STANDALONE)
        canvasContext_->Save();
#elif defined(TARGET_ALIPAY)
        canvasContext_->save();
#endif
    }

    void Restore() {
        this->AppendCommand("Restore");
#if defined(TARGET_STANDALONE)
        canvasContext_->Restore();
#elif defined(TARGET_ALIPAY)
        canvasContext_->restore();
#endif
    }

    bool HasClip() {
        this->AppendCommand("HasClip");
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        return canvasContext_->HasClip();
#else
        if(canvasContext_ == nullptr)
            return false;
        return canvasContext_->HasClipRegion();
#endif
#elif defined(TARGET_ALIPAY)
        // unsupport
        return false;
#endif
    }

    void SetShadowOffsetX(float v) {
        this->AppendCommand("SetShadowOffsetX: " + std::to_string(v));
#if defined(TARGET_STANDALONE)
        canvasContext_->SetShadowOffsetX(v);
#elif defined(TARGET_ALIPAY)
        canvasContext_->setShadowOffsetX(v);
#endif
    }

    void SetShadowOffsetY(float v) {
        this->AppendCommand("SetShadowOffsetX: " + std::to_string(v));
#if defined(TARGET_STANDALONE)
        canvasContext_->SetShadowOffsetY(v);
#elif defined(TARGET_ALIPAY)
        canvasContext_->setShadowOffsetY(v);
#endif
    }

    void SetShadowColor(const char *v) {
        this->AppendCommand("SetShadowColor ");
#if defined(TARGET_STANDALONE)
        canvasContext_->SetShadowColor(v);
#elif defined(TARGET_ALIPAY)
        std::string color(v);
        canvasContext_->setShadowColor(color);
#endif
    }

    void SetShadowBlur(float v) {
        this->AppendCommand("SetShadowBlur: " + std::to_string(v));
#if defined(TARGET_STANDALONE)
        canvasContext_->SetShadowBlur(v);
#elif defined(TARGET_ALIPAY)
        canvasContext_->setShadowBlur(v);
#endif
    }

    float MeasureTextWidth(const std::string &text) {
        float width = 0;
#if defined(TARGET_STANDALONE)
#ifdef ANDROID
        std::string _text = text;
        width = canvasContext_->MeasureText(_text);
#else
        width = canvasContext_->MeasureTextWidth(text.data());
#endif
#elif defined(TARGET_ALIPAY)
        width = canvasContext_->measureText(text).width;
#endif
        this->AppendCommand("MeasureTextWidth: " + text + " result: " + std::to_string(width));
        return width;
    }

    void Transform(float a, float b, float c, float d, float e, float f) {
        this->AppendCommand("Transform ");
#if defined(TARGET_STANDALONE)
#if defined(__ANDROID__)
        canvasContext_->Transform(a, b, c, d, e, f);
#else
        canvasContext_->Transfrom(a, b, c, d, e, f);
#endif
#elif defined(TARGET_ALIPAY)
        canvasContext_->transform(a, b, c, d, e, f);
#endif
    }

    void SetTransform(float a, float b, float c, float d, float e, float f) {
        this->AppendCommand("SetTransform ");
#if defined(TARGET_STANDALONE)
        canvasContext_->SetTransform(a, b, c, d, e, f);
#elif defined(TARGET_ALIPAY)
        canvasContext_->setTransform(a, b, c, d, e, f);
#endif
    }

    void Rect(float x, float y, float width, float height) {
        this->AppendCommand("Rect ");
#if defined(TARGET_STANDALONE)
        canvasContext_->Rect(x, y, width, height);
#elif defined(TARGET_ALIPAY)
        canvasContext_->rect(x, y, width, height);
#endif
    }

    void ClearRect(float x, float y, float width, float height) {
        this->AppendCommand("ClearRect ");
#if defined(TARGET_STANDALONE)
        canvasContext_->ClearRect(x, y, width, height);
#elif defined(TARGET_ALIPAY)
        canvasContext_->clearRect(x, y, width, height);
#endif
    }

    void FillRect(float x, float y, float width, float height) {
        this->AppendCommand("FillRect");
#if defined(TARGET_STANDALONE)
        canvasContext_->FillRect(x, y, width, height);
#elif defined(TARGET_ALIPAY)
        canvasContext_->fillRect(x, y, width, height);
#endif
    }

#if defined(TARGET_STANDALONE)
    void Fill(GFillRule fr = FILL_RULE_NONZERO) {
        this->AppendCommand("Fill");
        canvasContext_->Fill(fr);
    }
#elif defined(TARGET_ALIPAY)
    void Fill(std::string fillRule = "nonzero") {
        this->AppendCommand("Fill");
        canvasContext_->fill(fillRule);
    }
#endif

    void Stroke() {
        this->AppendCommand("Stroke");
#if defined(TARGET_STANDALONE)
        canvasContext_->Stroke();
#elif defined(TARGET_ALIPAY)
        canvasContext_->stroke();
#endif
    }

    void BeginPath() {
        this->AppendCommand("BeginPath");
#if defined(TARGET_STANDALONE)
        canvasContext_->BeginPath();
#elif defined(TARGET_ALIPAY)
        canvasContext_->beginPath();
#endif
    }

    void MoveTo(float x, float y) {
        this->AppendCommand("MoveTo x: " + std::to_string(x) + ", " + std::to_string(y));
#if defined(TARGET_STANDALONE)
        canvasContext_->MoveTo(x, y);
#elif defined(TARGET_ALIPAY)
        canvasContext_->moveTo(x, y);
#endif
    }

    void ClosePath() {
        this->AppendCommand("ClosePath");
#if defined(TARGET_STANDALONE)
        canvasContext_->ClosePath();
#elif defined(TARGET_ALIPAY)
        canvasContext_->closePath();
#endif
    }

    void LineTo(float x, float y) {
        this->AppendCommand("LienTo x: " + std::to_string(x) + ", " + std::to_string(y));
#if defined(TARGET_STANDALONE)
        canvasContext_->LineTo(x, y);
#elif defined(TARGET_ALIPAY)
        canvasContext_->lineTo(x, y);
#endif
    }

#if defined(TARGET_STANDALONE)
    void Clip(GFillRule f = FILL_RULE_NONZERO) {
        this->AppendCommand("Clip");
        canvasContext_->Clip(f);
    }
#elif defined(TARGET_ALIPAY)
    void Clip(std::string fillRule = "nonzero") {
        this->AppendCommand("Clip");
        canvasContext_->clip(fillRule);
    }
#endif

    void QuadraticCurveTo(float cpx, float cpy, float x, float y) {
#if defined(TARGET_STANDALONE)
        canvasContext_->QuadraticCurveTo(cpx, cpy, x, y);
#elif defined(TARGET_ALIPAY)
        canvasContext_->quadraticCurveTo(cpx, cpy, x, y);
#endif
    }

    void BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
        this->AppendCommand("BezierCurveTo");
#if defined(TARGET_STANDALONE)
        canvasContext_->BezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
#elif defined(TARGET_ALIPAY)
        canvasContext_->bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
#endif
    }

    void Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise = false) {
        this->AppendCommand("Arc");
#if defined(TARGET_STANDALONE)
        canvasContext_->Arc(x, y, r, sAngle, eAngle, antiClockwise);
#elif defined(TARGET_ALIPAY)
        canvasContext_->arc(x, y, r, sAngle, eAngle, antiClockwise);
#endif
    }

    void ArcTo(float x1, float y1, float x2, float y2, float r) {
        this->AppendCommand("ArcTo");
#if defined(TARGET_STANDALONE)
        canvasContext_->ArcTo(x1, y1, x2, y2, r);
#elif defined(TARGET_ALIPAY)
        canvasContext_->arcTo(x1, y1, x2, y2, r);
#endif
    }

    void Scale(float scale_width, float scale_height) {
        this->AppendCommand("Scale width: " + std::to_string(scale_width) + ", height: " + std::to_string(scale_height));
#if defined(TARGET_STANDALONE)
        canvasContext_->Scale(scale_width, scale_height);
#elif defined(TARGET_ALIPAY)
        canvasContext_->scale(scale_width, scale_height);
#endif
    }

    void Rotate(float angle) {
        this->AppendCommand("Rotate angle: " + std::to_string(angle));
#if defined(TARGET_STANDALONE)
        canvasContext_->Rotate(angle);
#elif defined(TARGET_ALIPAY)
        canvasContext_->rotate(angle);
#endif
    }

    void Translate(float x, float y) {
        this->AppendCommand("Translate x: " + std::to_string(x) + ", y: " + std::to_string(y));
#if defined(TARGET_STANDALONE)
        canvasContext_->Translate(x, y);
#elif defined(TARGET_ALIPAY)
        canvasContext_->translate(x, y);
#endif
    }

  private:
#if defined(TARGET_STANDALONE)
    std::string TextAlignToString(GTextAlign &op) const {
        std::string str = "";
        if(op == TEXT_ALIGN_START) {
            str = "start";
        } else if(op == TEXT_ALIGN_END) {
            str = "end";
        } else if(op == TEXT_ALIGN_LEFT) {
            str = "left";
        } else if(op == TEXT_ALIGN_CENTER) {
            str = "center";
        } else if(op == TEXT_ALIGN_RIGHT) {
            str = "right";
        }
        return str;
    }

    std::string TextBaselineToString(GTextBaseline &op) const {
        std::string str = "";
        if(op == TEXT_BASELINE_ALPHABETIC) {
            str = "alphabetic";
        } else if(op == TEXT_BASELINE_MIDDLE) {
            str = "middle";
        } else if(op == TEXT_BASELINE_TOP) {
            str = "top";
        } else if(op == TEXT_BASELINE_HANGING) {
            str = "hanging";
        } else if(op == TEXT_BASELINE_BOTTOM) {
            str = "bottom";
        } else if(op == TEXT_BASELINE_IDEOGRAPHIC) {
            str = "ideographic";
        }
        return str;
    }

#endif

  public:
    float GetDevicePixelRatio() { return devicePixelRatio_; }

    void Reset() { cmdCount_ = 0; }

    long GetRenderCount() { return cmdCount_; }

  private:
    long cmdCount_ = 0;

    void AppendCommand(const std::string &cmd) {
        cmdCount_++;
        if(tracer_ != nullptr) {
            tracer_->trace("%s", cmd.c_str());
        }
    }

  private:
    float devicePixelRatio_ = 1.0f;
    utils::Tracer *tracer_ = nullptr;
};
} // namespace canvas

} // namespace xg

#endif // XG_GRAPHICS_CANVAS_CONTEXT_H
