#include "WasiCanvasContext.h"
#include <stdlib.h>
#include <string.h>

using namespace xg;
using namespace xg::canvas;

// extern "C" void _2d_clearRect(float, float, float, float);

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
}

void WasiCanvasContext::SetFillStyle(const canvas::CanvasFillStrokeStyle &style) {
}

void WasiCanvasContext::SetStrokeStyle(const std::string &color) {
    
}

void WasiCanvasContext::SetStrokeStyle(const canvas::CanvasFillStrokeStyle &style) {
}

void WasiCanvasContext::SetLineCap(const std::string &lineCap) {
   
}

void WasiCanvasContext::SetLineJoin(const std::string &lineJoin) {
}

void WasiCanvasContext::SetLineWidth(float lineWidth) {
}

void WasiCanvasContext::SetLineDashOffset(float v) {
}

void WasiCanvasContext::SetLineDash(const std::vector<float> &params) {
}

void WasiCanvasContext::SetMiterLimit(float limit) {}

void WasiCanvasContext::SetGlobalAlpha(float globalAlpha) {
}

float WasiCanvasContext::GlobalAlpha() { return globalAlpha_; }

void WasiCanvasContext::SetFont(const std::string &font) {
}

void WasiCanvasContext::FillText(const std::string &text, float x, float y, float maxWidth) {
}

void WasiCanvasContext::StrokeText(const std::string &text, float x, float y, float maxWidth) {
}

std::string WasiCanvasContext::TextAlign() const { return textAlgin_; }

void WasiCanvasContext::SetTextAlign(const std::string &textAlign) {
}

std::string WasiCanvasContext::TextBaseline() const { return textBaseline_; }

void WasiCanvasContext::SetTextBaseline(const std::string &textBaseline) {
}

void WasiCanvasContext::StrokeRect(float x, float y, float width, float height) {
}

void WasiCanvasContext::Save() {
}

void WasiCanvasContext::Restore() {
}

bool WasiCanvasContext::HasClip() { return clip_; }

void WasiCanvasContext::SetShadowOffsetX(float v) {}

void WasiCanvasContext::SetShadowOffsetY(float v) {}

void WasiCanvasContext::SetShadowColor(const char *v) {}

void WasiCanvasContext::SetShadowBlur(float v) {}

float WasiCanvasContext::MeasureTextWidth(const std::string &text) {
    return 0;
}

void WasiCanvasContext::Transform(float a, float b, float c, float d, float e, float f) {
}

void WasiCanvasContext::SetTransform(float a, float b, float c, float d, float e, float f) {
}

void WasiCanvasContext::Rect(float x, float y, float width, float height) {
}

void WasiCanvasContext::ClearRect(float x, float y, float width, float height) {
}

void WasiCanvasContext::FillRect(float x, float y, float width, float height) {
}

void WasiCanvasContext::Fill(const std::string &fillRule) {
}

void WasiCanvasContext::Stroke() {
}

void WasiCanvasContext::BeginPath() {
}

void WasiCanvasContext::MoveTo(float x, float y) {
}

void WasiCanvasContext::ClosePath() {
}

void WasiCanvasContext::LineTo(float x, float y) {
}

void WasiCanvasContext::Clip(const std::string &fillRule) {
    clip_ = true;
}

void WasiCanvasContext::QuadraticCurveTo(float cpx, float cpy, float x, float y) {
}

void WasiCanvasContext::BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
}

void WasiCanvasContext::Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise) {
}

void WasiCanvasContext::ArcTo(float x1, float y1, float x2, float y2, float r) {
}

void WasiCanvasContext::Scale(float x, float y) {
}

void WasiCanvasContext::Rotate(float angle) {
}

void WasiCanvasContext::Translate(float x, float y) {
}

void WasiCanvasContext::DrawImage(CanvasImage *image, float dx, float dy) {}

void WasiCanvasContext::DrawImage(CanvasImage *image, float dx, float dy, float width, float height) {}