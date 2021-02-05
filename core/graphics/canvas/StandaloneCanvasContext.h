//#if defined(TARGET_STANDALONE)
#ifdef __ANDROID__
#include "android/F2CanvasView.h"
#else // iOS
#include <gcanvas/GCanvas2dContext.h>
#include <gcanvas/GConvert.h>
#endif

#include "CanvasContext.h"

#ifndef XG_GRAPHICS_CANVAS_CONTEXT_STANDALONE_H
#define XG_GRAPHICS_CANVAS_CONTEXT_STANDALONE_H

namespace xg {
namespace canvas {

/**
 *  Android 和 iOS 的 Canvas 承载对象不一样，在此抹平差异
 */
class StandaloneCanvasContext : public CanvasContext {
  public:
#ifdef __ANDROID__
    F2CanvasView *canvasContext_ = nullptr;
#else
    GCanvasContext *canvasContext_ = nullptr;
#endif

  public:
#ifdef __ANDROID__
    StandaloneCanvasContext(F2CanvasView *_canvasContext, float devicePixelRatio, utils::Tracer *tracer)
        : CanvasContext(devicePixelRatio, tracer), canvasContext_(_canvasContext) {}

#else
    StandaloneCanvasContext(GCanvasContext *_canvasContext, float devicePixelRatio, utils::Tracer *tracer)
        : CanvasContext(devicePixelRatio, tracer), canvasContext_(_canvasContext) {}
#endif

    ~StandaloneCanvasContext() { canvasContext_ = nullptr; }

    bool IsValid() override { return canvasContext_ != nullptr; }

    void SetFillStyle(const std::string &color) override {
        this->AppendCommand("SetFillStyle: " + color);
#ifdef __ANDROID__
        canvasContext_->SetFillStyle(color);
#else
        GColorRGBA _color = gcanvas::StrValueToColorRGBA(color.c_str());
        canvasContext_->SetFillStyle(_color);
#endif
    }

    void SetFillStyle(const util::CanvasFillStrokeStyle &style) override {
        switch(style.type) {
            case util::CanvasFillStrokeStyleType::kColor:
                this->SetFillStyle(style.color);
                break;
            case util::CanvasFillStrokeStyleType::kLinearGradient: {
                std::vector<float> offsets;
                std::vector<std::string> colors;

                for(int i = 0; i < style.linearGradient.colorStops.size(); i++) {
                    offsets.push_back(style.linearGradient.colorStops[i].offset);
                    colors.push_back(style.linearGradient.colorStops[i].color);
                }

                float startX = static_cast<float>(style.linearGradient.start.x);
                float startY = static_cast<float>(style.linearGradient.start.y);
                float endX = static_cast<float>(style.linearGradient.end.x);
                float endY = static_cast<float>(style.linearGradient.end.y);
#ifdef __ANDROID__
                canvasContext_->SetLinearGradientFillStyle(startX, startY, endX, endY, offsets, colors, false);
#else
                float startArr[2] = {startX, startY};
                float endArr[2] = {endX, endY};
                canvasContext_->SetFillStyleLinearGradient(startArr, endArr, (int)colors.size(), &offsets[0], &colors[0], false);
#endif
                break;
            }
            case util::CanvasFillStrokeStyleType::kRadialGradient: {
                std::vector<float> offsets;
                std::vector<std::string> colors;

                for(int i = 0; i < style.radialGradient.colorStops.size(); i++) {
                    offsets.push_back(style.radialGradient.colorStops[i].offset);
                    colors.push_back(style.radialGradient.colorStops[i].color);
                }
#ifdef __ANDROID__
                canvasContext_->SetRadialGradientFillStyle(style.radialGradient.start[0], style.radialGradient.start[1],
                                                           style.radialGradient.start[2], style.radialGradient.end[0],
                                                           style.radialGradient.end[1], style.radialGradient.end[2], offsets,
                                                           colors, false);
#else
                float start[3] = {style.radialGradient.start[0], style.radialGradient.start[1], style.radialGradient.start[2]};
                float end[3] = {style.radialGradient.end[0], style.radialGradient.end[1], style.radialGradient.end[2]};
                canvasContext_->SetFillStyleRadialGradient(start, end, (int)colors.size(), &offsets[0], &colors[0], false);
#endif
                break;
            }
            default:
                break;
        }
    }

    void SetStrokeStyle(const std::string &color) override {
        this->AppendCommand("SetStrokeStyle: " + color);
#ifdef __ANDROID__
        canvasContext_->SetStrokeStyle(color);
#else
        GColorRGBA _color = gcanvas::StrValueToColorRGBA(color.c_str());
        canvasContext_->SetStrokeStyle(_color);
#endif
    }

    void SetStrokeStyle(const util::CanvasFillStrokeStyle &style) override {
        switch(style.type) {
            case util::CanvasFillStrokeStyleType::kColor:
                this->SetStrokeStyle(style.color);
                break;
            case util::CanvasFillStrokeStyleType::kLinearGradient: {
                std::vector<float> offsets;
                std::vector<std::string> colors;

                for(int i = 0; i < style.linearGradient.colorStops.size(); i++) {
                    offsets.push_back(style.linearGradient.colorStops[i].offset);
                    colors.push_back(style.linearGradient.colorStops[i].color);
                }

                float startX = static_cast<float>(style.linearGradient.start.x);
                float startY = static_cast<float>(style.linearGradient.start.y);
                float endX = static_cast<float>(style.linearGradient.end.x);
                float endY = static_cast<float>(style.linearGradient.end.y);
#ifdef __ANDROID__
                canvasContext_->SetLinearGradientFillStyle(startX, startY, endX, endY, offsets, colors, true);
#else
                float startArr[2] = {startX, startY};
                float endArr[2] = {endX, endY};
                canvasContext_->SetFillStyleLinearGradient(startArr, endArr, (int)colors.size(), &offsets[0], &colors[0], true);
#endif
                break;
            }
            case util::CanvasFillStrokeStyleType::kRadialGradient: {
                std::vector<float> offsets;
                std::vector<std::string> colors;

                for(int i = 0; i < style.radialGradient.colorStops.size(); i++) {
                    offsets.push_back(style.radialGradient.colorStops[i].offset);
                    colors.push_back(style.radialGradient.colorStops[i].color);
                }
#ifdef __ANDROID__
                canvasContext_->SetRadialGradientFillStyle(style.radialGradient.start[0], style.radialGradient.start[1],
                                                           style.radialGradient.start[2], style.radialGradient.end[0],
                                                           style.radialGradient.end[1], style.radialGradient.end[2], offsets,
                                                           colors, true);
#else
                float start[3] = {style.radialGradient.start[0], style.radialGradient.start[1], style.radialGradient.start[2]};
                float end[3] = {style.radialGradient.end[0], style.radialGradient.end[1], style.radialGradient.end[2]};
                canvasContext_->SetFillStyleRadialGradient(start, end, (int)colors.size(), &offsets[0], &colors[0], true);
#endif
                break;
            }
            default:
                break;
        }
    }

    void SetLineCap(const std::string &lineCap) override {
        this->AppendCommand("SetLineCap: " + lineCap);
#ifdef __ANDROID__
        std::string _lineCap = lineCap;
        canvasContext_->SetLineCap(std::forward<std::string>(_lineCap));
#else
        canvasContext_->SetLineCap(lineCap.data());
#endif
    }

    void SetLineJoin(const std::string &lineJoin) override {
        this->AppendCommand("SetLineJoin: " + lineJoin);
#ifdef __ANDROID__
        std::string _lineJoin = lineJoin;
        canvasContext_->SetLineJoin(std::forward<std::string>(_lineJoin));
#else
        canvasContext_->SetLineJoin(lineJoin.data());
#endif
    }

    void SetLineWidth(float lineWidth) override {
        this->AppendCommand("SetLineWidth: " + std::to_string(lineWidth));
        canvasContext_->SetLineWidth(lineWidth);
    }

    void SetLineDashOffset(float v) override {
        this->AppendCommand("SetLineDashOffset: " + std::to_string(v));
        canvasContext_->SetLineDashOffset(v);
    }

    void SetLineDash(std::vector<float> params) override {
        this->AppendCommand("SetLineDash: vec{}");
        canvasContext_->SetLineDash(params);
    }

    void SetMiterLimit(float limit) override {
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

    void SetGlobalAlpha(float globalAlpha) override {
        this->AppendCommand("SetGlobalAlpha: " + std::to_string(globalAlpha));
        canvasContext_->SetGlobalAlpha(globalAlpha);
    }

    float GlobalAlpha() override { return canvasContext_->GlobalAlpha(); }

    void SetFont(const std::string &font) override {
        this->AppendCommand("SetFont: " + font);
#ifdef __ANDROID__
        std::string _font = font;
        canvasContext_->SetFont(std::forward<std::string>(_font));
#else
        canvasContext_->SetFont(font.data());
#endif
    }

    void FillText(const std::string &text, float x, float y, float maxWidth = SHRT_MAX) override {
        this->AppendCommand("FillText: " + text);
#ifdef __ANDROID__
        std::string _text = text;
        canvasContext_->FillText(_text, x, y, maxWidth);
#else
        if(isnan(maxWidth) || maxWidth <= 0) {
            return;
        }
        canvasContext_->DrawText(text.c_str(), x, y, maxWidth);
#endif
    }

    void StrokeText(std::string &text, float x, float y, float maxWidth) override {
        this->AppendCommand("StrokeText: " + text);
#ifdef __ANDROID__
        canvasContext_->StrokeText(text, x, y, maxWidth);
#else
        canvasContext_->StrokeText(text.c_str(), x, y, maxWidth);
#endif
    }

    std::string TextAlign() const override {
        GTextAlign textAlign = canvasContext_->TextAlign();
        return TextAlignToString(textAlign);
    }

    void SetTextAlign(const std::string &textAlign) override {
        this->AppendCommand("SetTextAlign ");
#ifdef __ANDROID__
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

    std::string TextBaseline() const override {
        GTextBaseline baseLine = canvasContext_->TextBaseline();
        return TextBaselineToString(baseLine);
    }

    void SetTextBaseline(const std::string &textBaseline) override {
        this->AppendCommand("SetTextBaseline");
#ifdef __ANDROID__
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

    void StrokeRect(float x, float y, float width, float height) override {
        this->AppendCommand("StrokeRect");
        canvasContext_->StrokeRect(x, y, width, height);
    }

    void Save() override {
        this->AppendCommand("Save");
        canvasContext_->Save();
    }

    void Restore() override {
        this->AppendCommand("Restore");
        canvasContext_->Restore();
    }

    bool HasClip() override {
        this->AppendCommand("HasClip");
#ifdef __ANDROID__
        return canvasContext_->HasClip();
#else
        if(canvasContext_ == nullptr)
            return false;
        return canvasContext_->HasClipRegion();
#endif
    }

    void SetShadowOffsetX(float v) override {
        this->AppendCommand("SetShadowOffsetX: " + std::to_string(v));
        canvasContext_->SetShadowOffsetX(v);
    }

    void SetShadowOffsetY(float v) override {
        this->AppendCommand("SetShadowOffsetX: " + std::to_string(v));
        canvasContext_->SetShadowOffsetY(v);
    }

    void SetShadowColor(const char *v) override {
        this->AppendCommand("SetShadowColor ");
        canvasContext_->SetShadowColor(v);
    }

    void SetShadowBlur(float v) override {
        this->AppendCommand("SetShadowBlur: " + std::to_string(v));
        canvasContext_->SetShadowBlur(v);
    }

    float MeasureTextWidth(const std::string &text) override {
        float width = 0;
#ifdef __ANDROID__
        std::string _text = text;
        width = canvasContext_->MeasureText(_text);
#else
        width = canvasContext_->MeasureTextWidth(text.data());
#endif
        this->AppendCommand("MeasureTextWidth: " + text + " result: " + std::to_string(width));
        return width;
    }

    void Transform(float a, float b, float c, float d, float e, float f) override {
        this->AppendCommand("Transform ");
#if defined(__ANDROID__)
        canvasContext_->Transform(a, b, c, d, e, f);
#else
        canvasContext_->Transfrom(a, b, c, d, e, f);
#endif
    }

    void SetTransform(float a, float b, float c, float d, float e, float f) override {
        this->AppendCommand("SetTransform ");
        canvasContext_->SetTransform(a, b, c, d, e, f);
    }

    void Rect(float x, float y, float width, float height) override {
        this->AppendCommand("Rect ");
        canvasContext_->Rect(x, y, width, height);
    }

    void ClearRect(float x, float y, float width, float height) override {
        this->AppendCommand("ClearRect ");
        canvasContext_->ClearRect(x, y, width, height);
    }

    void FillRect(float x, float y, float width, float height) override {
        this->AppendCommand("FillRect");
        canvasContext_->FillRect(x, y, width, height);
    }

    void Fill(std::string fillRule = "nonzero") override {
        this->AppendCommand("Fill");
        if(fillRule == "evenodd") {
            canvasContext_->Fill(FILL_RULE_EVENODD);
        } else {
            canvasContext_->Fill(FILL_RULE_NONZERO);
        }
    }

    void Stroke() override {
        this->AppendCommand("Stroke");
        canvasContext_->Stroke();
    }

    void BeginPath() override {
        this->AppendCommand("BeginPath");
        canvasContext_->BeginPath();
    }

    void MoveTo(float x, float y) override {
        this->AppendCommand("MoveTo x: " + std::to_string(x) + ", " + std::to_string(y));
        canvasContext_->MoveTo(x, y);
    }

    void ClosePath() override {
        this->AppendCommand("ClosePath");
        canvasContext_->ClosePath();
    }

    void LineTo(float x, float y) override {
        this->AppendCommand("LienTo x: " + std::to_string(x) + ", " + std::to_string(y));
        canvasContext_->LineTo(x, y);
    }

    void Clip(std::string fillRule = "nonzero") override {
        this->AppendCommand("Clip");
        if(fillRule == "evenodd") {
            canvasContext_->Clip(FILL_RULE_EVENODD);
        } else {
            canvasContext_->Clip(FILL_RULE_NONZERO);
        }
    }

    void QuadraticCurveTo(float cpx, float cpy, float x, float y) override { canvasContext_->QuadraticCurveTo(cpx, cpy, x, y); }

    void BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) override {
        this->AppendCommand("BezierCurveTo");
        canvasContext_->BezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
    }

    void Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise = false) override {
        this->AppendCommand("Arc");
        canvasContext_->Arc(x, y, r, sAngle, eAngle, antiClockwise);
    }

    void ArcTo(float x1, float y1, float x2, float y2, float r) override {
        this->AppendCommand("ArcTo");
        canvasContext_->ArcTo(x1, y1, x2, y2, r);
    }

    void Scale(float scale_width, float scale_height) override {
        this->AppendCommand("Scale width: " + std::to_string(scale_width) + ", height: " + std::to_string(scale_height));
        canvasContext_->Scale(scale_width, scale_height);
    }

    void Rotate(float angle) override {
        this->AppendCommand("Rotate angle: " + std::to_string(angle));
        canvasContext_->Rotate(angle);
    }

    void Translate(float x, float y) override {
        this->AppendCommand("Translate x: " + std::to_string(x) + ", y: " + std::to_string(y));
        canvasContext_->Translate(x, y);
    }

  private:
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
};
} // namespace canvas

} // namespace xg

#endif // XG_GRAPHICS_CANVAS_CONTEXT_STANDALONE_H
//#endif // TARGET_STANDALONE
