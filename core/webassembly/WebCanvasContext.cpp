#include "WebCanvasContext.h"
#include <emscripten/emscripten.h>
#include <stdlib.h>
#include <string.h>

using namespace xg;
using namespace xg::canvas;

WebCanvasContext::WebCanvasContext(const std::string &contextName, float width, float height, double devicePixelRatio, token::DarkModeManager &manager)
    : CanvasContext(devicePixelRatio, nullptr, manager) {
    canvasContext_ = contextName;
    EM_ASM(
        {
            var canvas = document.getElementById(UTF8ToString($0));
            canvas.width = $1;
            canvas.height = $2;
            canvas.style.width = $3 + 'px';
            canvas.style.height = $4 + 'px';
        },
        canvasContext_.c_str(), width, height, width / devicePixelRatio, height / devicePixelRatio);
}

WebCanvasContext::~WebCanvasContext() {}

bool WebCanvasContext::IsValid() {
    // cause js error EM_ASM_INT({ return document.getElementById(UTF8ToString($0)); }, canvasContext_.c_str());
    return !canvasContext_.empty();
}

void WebCanvasContext::SetFillStyle(const std::string &color) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').fillStyle = UTF8ToString($1); }, canvasContext_.c_str(),
           color.c_str());
}

void WebCanvasContext::SetFillStyle(const canvas::CanvasFillStrokeStyle &style) {
    switch(style.type) {
        case canvas::CanvasFillStrokeStyleType::kColor:
            this->SetFillStyle(style.color);
            break;
        case canvas::CanvasFillStrokeStyleType::kLinearGradient: {
            {
                // ag::CanvasLinearGradient gradient;
                // std::for_each(style.linearGradient.colorStops.begin(), style.linearGradient.colorStops.end(),
                //               [&](auto &item) -> void { gradient.addColorStop(item.offset, item.color); });
                // gradient.start.x = style.linearGradient.start.x;
                // gradient.start.y = style.linearGradient.start.y;
                // gradient.end.x = style.linearGradient.end.x;
                // gradient.end.y = style.linearGradient.end.y;
                // canvasContext_->setFillStyle(gradient);
            }
            break;
        }
        case canvas::CanvasFillStrokeStyleType::kRadialGradient: {
            {
                // ag::CanvasRadialGradient gradient;
                // std::for_each(style.radialGradient.colorStops.begin(), style.radialGradient.colorStops.end(),
                //               [&](auto &item) -> void { gradient.addColorStop(item.offset, item.color); });
                // memcpy(gradient.start, style.radialGradient.start, sizeof(float) * 3);
                // memcpy(gradient.end, style.radialGradient.end, sizeof(float) * 3);
                // canvasContext_->setFillStyle(gradient);
            }
            break;
        }
        default:
            break;
    }
}

void WebCanvasContext::SetStrokeStyle(const std::string &color) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').strokeStyle = UTF8ToString($1); },
           canvasContext_.c_str(), color.c_str());
}

void WebCanvasContext::SetStrokeStyle(const canvas::CanvasFillStrokeStyle &style) {
    switch(style.type) {
        case canvas::CanvasFillStrokeStyleType::kColor:
            this->SetStrokeStyle(style.color);
            break;
        case canvas::CanvasFillStrokeStyleType::kLinearGradient: {
            {
                // ag::CanvasLinearGradient gradient;
                // std::for_each(style.linearGradient.colorStops.begin(), style.linearGradient.colorStops.end(),
                //               [&](auto &item) -> void { gradient.addColorStop(item.offset, item.color); });
                // gradient.start.x = style.linearGradient.start.x;
                // gradient.start.y = style.linearGradient.start.y;
                // gradient.end.x = style.linearGradient.end.x;
                // gradient.end.y = style.linearGradient.end.y;
                // canvasContext_->setStrokeStyle(gradient);
            }
            break;
        }
        case canvas::CanvasFillStrokeStyleType::kRadialGradient: {
            {
                // ag::CanvasRadialGradient gradient;
                // std::for_each(style.radialGradient.colorStops.begin(), style.radialGradient.colorStops.end(),
                //               [&](auto &item) -> void { gradient.addColorStop(item.offset, item.color); });
                // memcpy(gradient.start, style.radialGradient.start, sizeof(float) * 3);
                // memcpy(gradient.end, style.radialGradient.end, sizeof(float) * 3);
                // canvasContext_->setStrokeStyle(gradient);
            }
            break;
        }
        default:
            break;
    }
}

void WebCanvasContext::SetLineCap(const std::string &lineCap) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').lineCap = UTF8ToString($1); }, canvasContext_.c_str(),
           lineCap.c_str());
}

void WebCanvasContext::SetLineJoin(const std::string &lineJoin) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').lineJoin = UTF8ToString($1); }, canvasContext_.c_str(),
           lineJoin.c_str());
}

void WebCanvasContext::SetLineWidth(float lineWidth) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').lineWidth = ($1); }, canvasContext_.c_str(), lineWidth);
}

void WebCanvasContext::SetLineDashOffset(float v) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').lineDashOffset = ($1); }, canvasContext_.c_str(), v);
}

void WebCanvasContext::SetLineDash(const std::vector<float> &params) {
    EM_ASM(
        {
            var dashes = [];
            for(var i = 0; i < $2; ++i) {
                dashes.push(HEAPF32[(($1 + i * 4) >> 2)]); // int是HEAP32
            }
            document.getElementById(UTF8ToString($0)).getContext('2d').setLineDash(dashes);
        },
        canvasContext_.c_str(), params.data(), params.size());
}

void WebCanvasContext::SetMiterLimit(float limit) {}

void WebCanvasContext::SetGlobalAlpha(float globalAlpha) {
    globalAlpha_ = globalAlpha;
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').globalAlpha = $1; }, canvasContext_.c_str(), globalAlpha);
}

float WebCanvasContext::GlobalAlpha() { return globalAlpha_; }

void WebCanvasContext::SetFont(const std::string &font) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').font = UTF8ToString($1); }, canvasContext_.c_str(), font.c_str());
}

void WebCanvasContext::FillText(const std::string &text, float x, float y, float maxWidth) {
    if(maxWidth < 0.0) {
        EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').fillText(UTF8ToString($1), $2, $3); },
               canvasContext_.c_str(), text.c_str(), x, y);
    } else {
        EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').fillText(UTF8ToString($1), $2, $3, $4); },
               canvasContext_.c_str(), text.c_str(), x, y, maxWidth);
    }
}

void WebCanvasContext::StrokeText(const std::string &text, float x, float y, float maxWidth) {
    if(maxWidth < 0.0) {
        EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').strokeText(UTF8ToString($1), $2, $3); },
               canvasContext_.c_str(), text.c_str(), x, y);
    } else {
        EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').strokeText(UTF8ToString($1), $2, $3, $4); },
               canvasContext_.c_str(), text.c_str(), x, y, maxWidth);
    }
}

std::string WebCanvasContext::TextAlign() const { return textAlgin_; }

void WebCanvasContext::SetTextAlign(const std::string &textAlign) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').textAlign = UTF8ToString($1); }, canvasContext_.c_str(),
           textAlign.c_str());
}

std::string WebCanvasContext::TextBaseline() const { return textBaseline_; }

void WebCanvasContext::SetTextBaseline(const std::string &textBaseline) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').textBaseline = UTF8ToString($1); },
           canvasContext_.c_str(), textBaseline.c_str());
}

void WebCanvasContext::StrokeRect(float x, float y, float width, float height) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').strokeRect($1, $2, $3, $4); }, canvasContext_.c_str(),
           x, y, width, height);
}

void WebCanvasContext::Save() {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').save(); }, canvasContext_.c_str());
}

void WebCanvasContext::Restore() {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').restore(); }, canvasContext_.c_str());
}

bool WebCanvasContext::HasClip() { return clip_; }

void WebCanvasContext::SetShadowOffsetX(float v) {}

void WebCanvasContext::SetShadowOffsetY(float v) {}

void WebCanvasContext::SetShadowColor(const char *v) {}

void WebCanvasContext::SetShadowBlur(float v) {}

float WebCanvasContext::MeasureTextWidth(const std::string &text) {
    return EM_ASM_DOUBLE({ return document.getElementById(UTF8ToString($0)).getContext('2d').measureText(UTF8ToString($1)).width; },
                         canvasContext_.c_str(), text.c_str());
}

void WebCanvasContext::Transform(float a, float b, float c, float d, float e, float f) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').transform($1, $2, $3, $4, $5, $6); },
           canvasContext_.c_str(), a, b, c, d, e, f);
}

void WebCanvasContext::SetTransform(float a, float b, float c, float d, float e, float f) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').setTransform($1, $2, $3, $4, $5, $6); },
           canvasContext_.c_str(), a, b, c, d, e, f);
}

void WebCanvasContext::Rect(float x, float y, float width, float height) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').rect($1, $2, $3, $4); }, canvasContext_.c_str(), x, y, width, height);
}

void WebCanvasContext::ClearRect(float x, float y, float width, float height) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').clearRect($1, $2, $3, $4); }, canvasContext_.c_str(), x,
           y, width, height);
}

void WebCanvasContext::FillRect(float x, float y, float width, float height) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').fillRect($1, $2, $3, $4); }, canvasContext_.c_str(), x,
           y, width, height);
}

void WebCanvasContext::Fill(const std::string &fillRule) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').fill(); }, canvasContext_.c_str());
}

void WebCanvasContext::Stroke() {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').stroke(); }, canvasContext_.c_str());
}

void WebCanvasContext::BeginPath() {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').beginPath(); }, canvasContext_.c_str());
}

void WebCanvasContext::MoveTo(float x, float y) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').moveTo($1, $2); }, canvasContext_.c_str(), x, y);
}

void WebCanvasContext::ClosePath() {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').closePath(); }, canvasContext_.c_str());
}

void WebCanvasContext::LineTo(float x, float y) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').lineTo($1, $2); }, canvasContext_.c_str(), x, y);
}

void WebCanvasContext::Clip(const std::string &fillRule) {
    clip_ = true;
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').clip(); }, canvasContext_.c_str());
}

void WebCanvasContext::QuadraticCurveTo(float cpx, float cpy, float x, float y) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').quadraticCurveTo($1, $2, $3, $4); },
           canvasContext_.c_str(), cpx, cpy, x, y);
}

void WebCanvasContext::BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').bezierCurveTo($1, $2, $3, $4, $5, $6); },
           canvasContext_.c_str(), cp1x, cp1y, cp2x, cp2y, x, y);
}

void WebCanvasContext::Arc(float x, float y, float r, float sAngle, float eAngle, bool antiClockwise) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').arc($1, $2, $3, $4, $5, $6); }, canvasContext_.c_str(), x,
           y, r, sAngle, eAngle, antiClockwise);
}

void WebCanvasContext::ArcTo(float x1, float y1, float x2, float y2, float r) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').arcTo($1, $2, $3, $4, $5); }, canvasContext_.c_str(),
           x1, y1, x2, y2, r);
}

void WebCanvasContext::Scale(float x, float y) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').scale($1, $2); }, canvasContext_.c_str(), x, y);
}

void WebCanvasContext::Rotate(float angle) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').rotate($1); }, canvasContext_.c_str(), angle);
}

void WebCanvasContext::Translate(float x, float y) {
    EM_ASM({ document.getElementById(UTF8ToString($0)).getContext('2d').translate($1, $2); }, canvasContext_.c_str(), x, y);
}

void WebCanvasContext::DrawImage(CanvasImage *image, float dx, float dy) {}

void WebCanvasContext::DrawImage(CanvasImage *image, float dx, float dy, float width, float height) {}