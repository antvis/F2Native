#include "WasiCanvasContext.h"
#include <stdlib.h>
#include <string.h>

using namespace xg;
using namespace xg::canvas;

extern "C" void clearRect(float, float, float, float);
extern "C" void lineTo(float, float);
extern "C" void moveTo(float, float);
extern "C" void setFillStyle(const char *);
extern "C" void setStrokeStyle(const char *);
extern "C" void setLineWidth(float);
extern "C" void setFont(const char *);
extern "C" float globalAlpha();
extern "C" void fillText(const char *, float, float, float);
extern "C" void strokeText(const char *, float, float, float);
extern "C" void setGlobalAlpha(float);
extern "C" const char *textAlign();
extern "C" void setTextAlign(const char *);
extern "C" const char *textBaseline();
extern "C" void setTextBaseline(const char *);
extern "C" void strokeRect(float, float, float, float);
extern "C" void fillRect(float, float, float, float);
extern "C" void rect(float, float, float, float);
extern "C" void save();
extern "C" void restore();
extern "C" void fill(const char *);
extern "C" void stroke();
extern "C" void beginPath();
extern "C" void closePath();
extern "C" void quadraticCurveTo(float, float, float, float);
extern "C" void bezierCurveTo(float, float, float, float, float, float);
extern "C" void arc(float, float, float, float, float, int);
extern "C" void arcTo(float, float, float, float, float);
extern "C" void scale(float, float);
extern "C" void rotate(float);
extern "C" void translate(float, float);
extern "C" float measureTextWidth(const char *);
extern "C" void transform(float, float, float, float, float, float);
extern "C" void setTransform(float, float, float, float, float, float);

WasiCanvasContext::WasiCanvasContext(const std::string &contextName, float width, float height, double devicePixelRatio)
    : CanvasContext(devicePixelRatio, nullptr) {
    canvasContext_ = contextName;
}

WasiCanvasContext::~WasiCanvasContext() {}

bool WasiCanvasContext::IsValid() {
    return true;
}

void WasiCanvasContext::ChangeSize(float width, float height) {

}

void WasiCanvasContext::SetFillStyle(const std::string &color) {
    setFillStyle(color.c_str());
}

void WasiCanvasContext::SetFillStyle(const canvas::CanvasFillStrokeStyle &style) {
    switch (style.type) {
    case CanvasFillStrokeStyleType::kColor: {
      this->SetFillStyle(style.color);
      break;
    }
    default:
      break;
  }
}

void WasiCanvasContext::SetStrokeStyle(const std::string &color) {
    setStrokeStyle(color.c_str());
}

void WasiCanvasContext::SetStrokeStyle(const canvas::CanvasFillStrokeStyle &style) {
    switch (style.type) {
        case CanvasFillStrokeStyleType::kColor: {
            this->SetStrokeStyle(style.color);
            break;
        }
        default:
        break;
  }
}

void WasiCanvasContext::SetLineCap(const std::string &lineCap) {
}

void WasiCanvasContext::SetLineJoin(const std::string &lineJoin) {
}

void WasiCanvasContext::SetLineWidth(float lineWidth) {
    setLineWidth(lineWidth);
}

void WasiCanvasContext::SetLineDashOffset(float v) {
}

void WasiCanvasContext::SetLineDash(const std::vector<float> &params) {
}

void WasiCanvasContext::SetMiterLimit(float limit) {}

void WasiCanvasContext::SetGlobalAlpha(float globalAlpha) {
    setGlobalAlpha(globalAlpha);
}

float WasiCanvasContext::GlobalAlpha() { return globalAlpha(); }

void WasiCanvasContext::SetFont(const std::string &font) {
    setFont(font.c_str());
}

void WasiCanvasContext::FillText(const std::string &text, float x, float y, float maxWidth) {
    fillText(text.c_str(), x, y, maxWidth);
}

void WasiCanvasContext::StrokeText(const std::string &text, float x, float y, float maxWidth) {
    strokeText(text.c_str(), x, y, maxWidth);
}

std::string WasiCanvasContext::TextAlign() const { return textAlign(); }

void WasiCanvasContext::SetTextAlign(const std::string &textAlign) {
    setTextAlign(textAlign.c_str());
}

std::string WasiCanvasContext::TextBaseline() const { return textBaseline(); }

void WasiCanvasContext::SetTextBaseline(const std::string &textBaseline) {
    setTextBaseline(textBaseline.c_str());
}

void WasiCanvasContext::StrokeRect(float x, float y, float width, float height) {
    strokeRect(x, y, width, height);
}

void WasiCanvasContext::Save() {
    save();
}

void WasiCanvasContext::Restore() {
    restore();
}

bool WasiCanvasContext::HasClip() { return clip_; }

void WasiCanvasContext::SetShadowOffsetX(float v) {}

void WasiCanvasContext::SetShadowOffsetY(float v) {}

void WasiCanvasContext::SetShadowColor(const char *v) {}

void WasiCanvasContext::SetShadowBlur(float v) {}

float WasiCanvasContext::MeasureTextWidth(const std::string &text) {
    return measureTextWidth(text.c_str());
}

void WasiCanvasContext::Transform(float a, float b, float c, float d, float e, float f) {
    transform(a, b, c, d, e, f);
}

void WasiCanvasContext::SetTransform(float a, float b, float c, float d, float e, float f) {
    setTransform(a, b, c, d, e, f);
}

void WasiCanvasContext::Rect(float x, float y, float width, float height) {
    rect(x, y, width, height);
}

void WasiCanvasContext::ClearRect(float x, float y, float width, float height) {
    clearRect(x, y, width, height);
}

void WasiCanvasContext::FillRect(float x, float y, float width, float height) {
    fillRect(x, y, width, height);
}

void WasiCanvasContext::Fill(const std::string &fillRule) {
    fill(fillRule.c_str());
}

void WasiCanvasContext::Stroke() {
    stroke();
}

void WasiCanvasContext::BeginPath() {
    beginPath();
}

void WasiCanvasContext::MoveTo(float x, float y) {
    moveTo(x, y);
}

void WasiCanvasContext::ClosePath() {
    closePath();
}

void WasiCanvasContext::LineTo(float x, float y) {
    lineTo(x, y);
}

void WasiCanvasContext::Clip(const std::string &fillRule) {
    clip_ = true;
}

void WasiCanvasContext::QuadraticCurveTo(float cpx, float cpy, float x, float y) {
    quadraticCurveTo(cpx, cpy, x, y);
}

void WasiCanvasContext::BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
}

void WasiCanvasContext::Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise) {
    arc(x, y, r, sAngle, eAngle, antiClockwise);
}

void WasiCanvasContext::ArcTo(float x1, float y1, float x2, float y2, float r) {
    arcTo(x1, y1, x2, y2, r);
}

void WasiCanvasContext::Scale(float x, float y) {
    scale(x, y);
}

void WasiCanvasContext::Rotate(float angle) {
    rotate(angle);
}

void WasiCanvasContext::Translate(float x, float y) {
    Translate(x, y);
}

void WasiCanvasContext::DrawImage(CanvasImage *image, float dx, float dy) {}

void WasiCanvasContext::DrawImage(CanvasImage *image, float dx, float dy, float width, float height) {}