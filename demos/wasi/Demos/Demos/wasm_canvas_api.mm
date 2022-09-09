//
//  wasm_canvas_api.c
//  Demos
//
//  Created by weiqing.twq on 9/8/22.
//

#include "wasm_canvas_api.h"
#include <Wasm3Lib/wasm3.h>
#include <Wasm3Lib/m3_exception.h>

static
M3Result SuppressLookupFailure(M3Result i_result)
{
    if (i_result == m3Err_functionLookupFailed)
        return m3Err_none;
    else
        return i_result;
}

m3ApiRawFunction(clearRect) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, width)
    m3ApiGetArg   (float, height)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->ClearRect(x, y, width, height);
    m3ApiSuccess();
}

m3ApiRawFunction(lineTo) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->LineTo(x, y);
    m3ApiSuccess();
}

m3ApiRawFunction(moveTo) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->MoveTo(x, y);
    m3ApiSuccess();
}

m3ApiRawFunction(setFillStyle) {
    m3ApiGetArgMem   (const char *, color)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetFillStyle(color);
    m3ApiSuccess();
}

m3ApiRawFunction(setStrokeStyle) {
    m3ApiGetArgMem   (const char *, color)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetStrokeStyle(color);
    m3ApiSuccess();
}

m3ApiRawFunction(setLineWidth) {
    m3ApiGetArg   (float, lineWidth)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetLineWidth(lineWidth);
    m3ApiSuccess();
}

m3ApiRawFunction(globalAlpha) {
    m3ApiReturnType  (float)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    m3ApiReturn(context->GlobalAlpha());
}

m3ApiRawFunction(setFont) {
    m3ApiGetArgMem   (const char *, font)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetFont(font);
    m3ApiSuccess();
}

m3ApiRawFunction(fillText) {
    m3ApiGetArgMem   (const char *, font)
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, maxWith)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->FillText(font, x, y, maxWith);
    m3ApiSuccess();
}

m3ApiRawFunction(strokeText) {
    m3ApiGetArgMem   (const char *, font)
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, maxWith)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->StrokeText(font, x, y, maxWith);
    m3ApiSuccess();
}

m3ApiRawFunction(setGlobalAlpha) {
    m3ApiGetArg   (float, alpha)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetGlobalAlpha(alpha);
    m3ApiSuccess();
}

m3ApiRawFunction(textAlign) {
    m3ApiReturnType  (const char *)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    m3ApiReturn(context->TextAlign().c_str());
}

m3ApiRawFunction(setTextAlign) {
    m3ApiGetArgMem   (const char *, align)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetTextAlign(align);
    m3ApiSuccess();
}

m3ApiRawFunction(textBaseline) {
    m3ApiReturnType  (const char *)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    m3ApiReturn(context->TextBaseline().c_str());
}

m3ApiRawFunction(setTextBaseline) {
    m3ApiGetArgMem   (const char *, baseline)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetTextBaseline(baseline);
    m3ApiSuccess();
}

m3ApiRawFunction(strokeRect) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, width)
    m3ApiGetArg   (float, height)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->StrokeRect(x, y, width, height);
    m3ApiSuccess();
}

m3ApiRawFunction(fillRect) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, width)
    m3ApiGetArg   (float, height)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->FillRect(x, y, width, height);
    m3ApiSuccess();
}

m3ApiRawFunction(rect) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, width)
    m3ApiGetArg   (float, height)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Rect(x, y, width, height);
    m3ApiSuccess();
}

m3ApiRawFunction(save) {
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Save();
    m3ApiSuccess();
}

m3ApiRawFunction(restore) {
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Restore();
    m3ApiSuccess();
}

m3ApiRawFunction(fill) {
    m3ApiGetArgMem   (const char *, fillRule)
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Fill(fillRule);
    m3ApiSuccess();
}

m3ApiRawFunction(stroke) {
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Stroke();
    m3ApiSuccess();
}

m3ApiRawFunction(beginPath) {
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->BeginPath();
    m3ApiSuccess();
}

m3ApiRawFunction(closePath) {
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->ClosePath();
    m3ApiSuccess();
}

m3ApiRawFunction(quadraticCurveTo) {
    m3ApiGetArg   (float, cpx)
    m3ApiGetArg   (float, cpy)
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->QuadraticCurveTo(cpx, cpy, x, y);
    m3ApiSuccess();
}

m3ApiRawFunction(bezierCurveTo) {
    m3ApiGetArg   (float, cp1x)
    m3ApiGetArg   (float, cp1y)
    m3ApiGetArg   (float, cp2x)
    m3ApiGetArg   (float, cp2y)
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->BezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
    m3ApiSuccess();
}

m3ApiRawFunction(arc) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, r)
    m3ApiGetArg   (float, sAngle)
    m3ApiGetArg   (float, eAngle)
    m3ApiGetArg   (int, antiClockwise)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Arc(x, y, r, sAngle, eAngle, antiClockwise);
    m3ApiSuccess();
}

m3ApiRawFunction(arcTo) {
    m3ApiGetArg   (float, x1)
    m3ApiGetArg   (float, y1)
    m3ApiGetArg   (float, x2)
    m3ApiGetArg   (float, y2)
    m3ApiGetArg   (float, r)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->ArcTo(x1, y1, x2, y2, r);
    m3ApiSuccess();
}

m3ApiRawFunction(scale) {
    m3ApiGetArg   (float, scale_width)
    m3ApiGetArg   (float, scale_height)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Scale(scale_width, scale_height);
    m3ApiSuccess();
}

m3ApiRawFunction(rotate) {
    m3ApiGetArg   (float, angle)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Rotate(angle);
    m3ApiSuccess();
}

m3ApiRawFunction(translate) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Translate(x, y);
    m3ApiSuccess();
}

m3ApiRawFunction(measureTextWidth) {
    m3ApiReturnType  (float)
    m3ApiGetArgMem   (const char *, text)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    m3ApiReturn(context->MeasureTextWidth(text));
}

m3ApiRawFunction(transform) {
    m3ApiGetArg   (float, a)
    m3ApiGetArg   (float, b)
    m3ApiGetArg   (float, c)
    m3ApiGetArg   (float, d)
    m3ApiGetArg   (float, e)
    m3ApiGetArg   (float, f)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->Transform(a, b, c, d, e, f);
    m3ApiSuccess();
}

m3ApiRawFunction(setTransform) {
    m3ApiGetArg   (float, a)
    m3ApiGetArg   (float, b)
    m3ApiGetArg   (float, c)
    m3ApiGetArg   (float, d)
    m3ApiGetArg   (float, e)
    m3ApiGetArg   (float, f)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetTransform(a, b, c, d, e, f);
    m3ApiSuccess();
}

M3Result m3_LinkCanvas(IM3Module module, CanvasContext *context) {
    M3Result result = m3Err_none;

    const char* canvas = "*";

_   (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "clearRect", "v(ffff)", &clearRect, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "lineTo", "v(ff)", &lineTo, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "moveTo", "v(ff)", &moveTo, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setFillStyle", "v(*)", &setFillStyle, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setStrokeStyle", "v(*)", &setStrokeStyle, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setLineWidth", "v(f)", &setLineWidth, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setFont", "v(*)", &setFont, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "globalAlpha", "f()", &globalAlpha, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "fillText", "v(*fff)", &fillText, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "strokeText", "v(*fff)", &strokeText, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setGlobalAlpha", "v(f)", &setGlobalAlpha, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "textAlign", "*()", &textAlign, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setTextAlign", "v(*)", &setTextAlign, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "textBaseline", "*()", &textBaseline, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setTextBaseline", "v(*)", &setTextBaseline, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "strokeRect", "v(ffff)", &strokeRect, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "fillRect", "v(ffff)", &fillRect, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "rect", "v(ffff)", &rect, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "save", "v()", &save, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "restore", "v()", &restore, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "fill", "v(*)", &fill, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "stroke", "v()", &stroke, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "beginPath", "v()", &beginPath, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "closePath", "v()", &closePath, context)));
    
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "quadraticCurveTo", "v(ffff)", &quadraticCurveTo, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "bezierCurveTo", "v(ffffff)", &bezierCurveTo, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "arc", "v(fffffi)", &arc, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "arcTo", "v(fffff)", &arcTo, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "scale", "v(ff)", &scale, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "rotate", "v(f)", &scale, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "translate", "v(ff)", &translate, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "measureTextWidth", "f(*)", &measureTextWidth, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "transform", "v(ffffff)", &transform, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setTransform", "v(ffffff)", &setTransform, context)));
_catch:
    return result;
}
