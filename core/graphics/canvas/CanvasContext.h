#ifndef XG_GRAPHICS_CANVAS_CONTEXT_H
#define XG_GRAPHICS_CANVAS_CONTEXT_H

#ifdef ANDROID
#include "android/F2CanvasView.h"
#else
#include <gcanvas/GCanvas2dContext.h>
#include <gcanvas/GConvert.h>
#endif

#include <utils/Tracer.h>

namespace xg {
namespace canvas {

/**
 *  Android 和 iOS 的 Canvas 承载对象不一样，在此抹平差异
 */
class CanvasContext {
  public:
#ifdef ANDROID
    F2CanvasView *canvasContext_ = nullptr;
#else
    GCanvasContext *canvasContext_ = nullptr;
#endif

  public:
#ifdef ANDROID
    CanvasContext(F2CanvasView *_canvasContext, float devicePixelRatio, utils::Tracer *tracer)
        : canvasContext_(_canvasContext), devicePixelRatio_(devicePixelRatio), tracer_(tracer) {}
#else
    CanvasContext(GCanvasContext *_canvasContext, float devicePixelRatio, utils::Tracer *tracer)
        : canvasContext_(_canvasContext), devicePixelRatio_(devicePixelRatio), tracer_(tracer) {}
#endif

    ~CanvasContext() {
        canvasContext_ = nullptr;
        tracer_ = nullptr;
    }

    bool IsValid() { return canvasContext_ != nullptr; }

    void SetFillStyle(const std::string &color) {
        this->AppendCommand("SetFillStyle: " + color);
#ifdef ANDROID
        canvasContext_->SetFillStyle(color);
#else
        GColorRGBA _color = gcanvas::StrValueToColorRGBA(color.c_str());
        canvasContext_->SetFillStyle(_color);
#endif
    }

    void SetStrokeStyle(const std::string &color) {
        this->AppendCommand("SetStrokeStyle: " + color);
#ifdef ANDROID
        canvasContext_->SetStrokeStyle(color);
#else
        GColorRGBA _color = gcanvas::StrValueToColorRGBA(color.c_str());
        canvasContext_->SetStrokeStyle(_color);
#endif
    }

    void SetLineCap(const std::string &lineCap) {
        this->AppendCommand("SetLineCap: " + lineCap);
#ifdef ANDROID
        std::string _lineCap = lineCap;
        canvasContext_->SetLineCap(std::forward<std::string>(_lineCap));
#else
        canvasContext_->SetLineCap(lineCap.data());
#endif
    }

    void SetLineJoin(const std::string &lineJoin) {
        this->AppendCommand("SetLineJoin: " + lineJoin);
#ifdef ANDROID
        std::string _lineJoin = lineJoin;
        canvasContext_->SetLineJoin(std::forward<std::string>(_lineJoin));
#else
        canvasContext_->SetLineJoin(lineJoin.data());
#endif
    }

    void SetLineWidth(float lineWidth) {
        this->AppendCommand("SetLineWidth: " + std::to_string(lineWidth));
        canvasContext_->SetLineWidth(lineWidth);
    }

    void SetLineDashOffset(float v) {
        this->AppendCommand("SetLineDashOffset: " + std::to_string(v));
        canvasContext_->SetLineDashOffset(v);
    }

    void SetLineDash(std::vector<float> params) {
        this->AppendCommand("SetLineDash: vec{}");
        canvasContext_->SetLineDash(params);
    }

    void SetMiterLimit(float limit) {
        this->AppendCommand("SetMiterLimit: " + std::to_string(limit));
        canvasContext_->SetMiterLimit(limit);
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
        canvasContext_->SetGlobalAlpha(globalAlpha);
    }

    float GlobalAlpha() { return canvasContext_->GlobalAlpha(); }

    void SetFont(const std::string &font) {
        this->AppendCommand("SetFont: " + font);
#ifdef ANDROID
        std::string _font = font;
        canvasContext_->SetFont(std::forward<std::string>(_font));
#else
        canvasContext_->SetFont(font.data());
#endif
    }

    void FillText(const std::string &text, float x, float y, float maxWidth = SHRT_MAX) {
        this->AppendCommand("FillText: " + text);
#ifdef ANDROID
        std::string _text = text;
        canvasContext_->FillText(_text, x, y, maxWidth);
#else
        if(isnan(maxWidth) || maxWidth <= 0) {
            return;
        }
        canvasContext_->DrawText(text.c_str(), x, y, maxWidth);
//        canvasContext_->FillText((const unsigned short *)text.c_str(), text.length(), x, y, false);
#endif
    }

    void StrokeText(std::string &text, float x, float y, float maxWidth) {
        this->AppendCommand("StrokeText: " + text);
#ifdef ANDROID
        canvasContext_->StrokeText(text, x, y, maxWidth);
#else
        canvasContext_->StrokeText(text.c_str(), x, y, maxWidth);
#endif
    }

    GTextAlign TextAlign() const { return canvasContext_->TextAlign(); }

    void SetTextAlign(GTextAlign &textAlign) { canvasContext_->SetTextAlign(textAlign); }

    void SetTextAlign(const std::string &textAlign) {
        this->AppendCommand("SetTextAlign ");
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
    }

    GTextBaseline TextBaseline() const {
//        this->AppendCommand("GetTextBaseline");
#ifdef ANDROID
        return canvasContext_->TextBaseline();
#else
        return canvasContext_->TextBaseline();
#endif
    }

    void SetTextBaseline(GTextBaseline &textBaseline) {
        this->AppendCommand("SetTextBaseline");
        canvasContext_->SetTextBaseline(textBaseline);
    }

    void SetTextBaseline(const std::string &textBaseline) {
        this->AppendCommand("SetTextBaseline");
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
    }

    void StrokeRect(float x, float y, float width, float height) {
        this->AppendCommand("StrokeRect");
#ifdef ANDROID
        canvasContext_->StrokeRect(x, y, width, height);
#else
        canvasContext_->StrokeRect(x, y, width, height);
#endif
    }

    void Save() {
        this->AppendCommand("Save");
        canvasContext_->Save();
    }

    void Restore() {
        this->AppendCommand("Restore");
        canvasContext_->Restore();
    }

    bool HasClip() {
        this->AppendCommand("HasClip");
#ifdef ANDROID
        return canvasContext_->HasClip();
#else
        if(canvasContext_ == nullptr)
            return false;
        return canvasContext_->HasClipRegion();
#endif
    }

    void SetShadowOffsetX(float v) {
        this->AppendCommand("SetShadowOffsetX: " + std::to_string(v));
        canvasContext_->SetShadowOffsetX(v);
    }

    void SetShadowOffsetY(float v) {
        this->AppendCommand("SetShadowOffsetX: " + std::to_string(v));
        canvasContext_->SetShadowOffsetY(v);
    }

    void SetShadowColor(const char *v) {
        this->AppendCommand("SetShadowColor ");
        canvasContext_->SetShadowColor(v);
    }

    void SetShadowBlur(float v) {
        this->AppendCommand("SetShadowBlur: " + std::to_string(v));
        canvasContext_->SetShadowBlur(v);
    }

    float MeasureTextWidth(const std::string &text) {
        float width = 0;
#ifdef ANDROID
        std::string _text = text;
        width = canvasContext_->MeasureText(_text);
#else
        width = canvasContext_->MeasureTextWidth(text.data());
#endif
        this->AppendCommand("MeasureTextWidth: " + text + " result: " + std::to_string(width));
        return width;
    }

    void Transform(float a, float b, float c, float d, float e, float f) {
        this->AppendCommand("Transform ");
#ifdef ANDROID
        canvasContext_->Transform(a, b, c, d, e, f);
#else
        canvasContext_->Transfrom(a, b, c, d, e, f);
#endif
    }

    void SetTransform(float a, float b, float c, float d, float e, float f) {
        this->AppendCommand("SetTransform ");
        canvasContext_->SetTransform(a, b, c, d, e, f);
    }

    void Rect(float x, float y, float width, float height) {
        this->AppendCommand("Rect ");
        canvasContext_->Rect(x, y, width, height);
    }

    void ClearRect(float x, float y, float width, float height) {
        this->AppendCommand("ClearRect ");
        canvasContext_->ClearRect(x, y, width, height);
    }

    void FillRect(float x, float y, float width, float height) {
        this->AppendCommand("FillRect");
        canvasContext_->FillRect(x, y, width, height);
    }

    void Fill(GFillRule fr = FILL_RULE_NONZERO) {
        this->AppendCommand("Fill");
        canvasContext_->Fill(fr);
    }

    void Stroke() {
        this->AppendCommand("Stroke");
        canvasContext_->Stroke();
    }

    void BeginPath() {
        this->AppendCommand("BeginPath");
        canvasContext_->BeginPath();
    }

    void MoveTo(float x, float y) {
        this->AppendCommand("MoveTo x: " + std::to_string(x) + ", " + std::to_string(y));
        canvasContext_->MoveTo(x, y);
    }

    void ClosePath() {
        this->AppendCommand("ClosePath");
        canvasContext_->ClosePath();
    }

    void LineTo(float x, float y) {
        this->AppendCommand("LienTo x: " + std::to_string(x) + ", " + std::to_string(y));
        canvasContext_->LineTo(x, y);
    }

    void Clip(GFillRule f) {
        this->AppendCommand("Clip");
        canvasContext_->Clip(f);
    }

    void QuadraticCurveTo(float cpx, float cpy, float x, float y) { canvasContext_->QuadraticCurveTo(cpx, cpy, x, y); }

    void BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
        this->AppendCommand("BezierCurveTo");
        canvasContext_->BezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
    }

    void Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise = false) {
        this->AppendCommand("Arc");
        canvasContext_->Arc(x, y, r, sAngle, eAngle, antiClockwise);
    }

    void ArcTo(float x1, float y1, float x2, float y2, float r) {
        this->AppendCommand("ArcTo");
        canvasContext_->ArcTo(x1, y1, x2, y2, r);
    }

    void Scale(float scale_width, float scale_height) {
        this->AppendCommand("Scale width: " + std::to_string(scale_width) + ", height: " + std::to_string(scale_height));
        canvasContext_->Scale(scale_width, scale_height);
    }

    void Rotate(float angle) {
        this->AppendCommand("Rotate angle: " + std::to_string(angle));
        canvasContext_->Rotate(angle);
    }

    void Translate(float x, float y) {
        this->AppendCommand("Translate x: " + std::to_string(x) + ", y: " + std::to_string(y));
        canvasContext_->Translate(x, y);
    }

  private:
    std::string TextAlignToString(GTextAlign &op) {
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

    std::string TextBaselineToString(GTextBaseline &op) {
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
