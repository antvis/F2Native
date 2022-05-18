//
//  CoreGraphicsContext.c
//  F2
//
//  Created by weiqing.twq on 2021/11/29.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#if defined(__APPLE__)

#include "CoreGraphicsContext.h"
#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>
#include <UIKit/UIKit.h>

using namespace xg::canvas;

ScopedCFObject::ScopedCFObject(void *obj) {
  object_ = obj;
  CFRetain(CFTypeRef(object_));
}

ScopedCFObject::~ScopedCFObject() {
  if (object_) {
    CFRelease(CFTypeRef(object_));
    object_ = nil;
  }
}

static const UIColor *GetUIColorFromHexString(const std::string &hexStr, const float alpha) {
  CanvasColor rgba;
  // Convert hex string to an integer
  CanvasColorParser::Parse(hexStr, rgba);

  // Create a color object, specifying alpha as well
  UIColor *color = [UIColor colorWithRed:(rgba.r) / 255
                                   green:(rgba.g) / 255
                                    blue:(rgba.b) / 255
                                   alpha:alpha];
  return color;
}

static UIFont *GetUIFont(const std::string &fontFmaily, float fontSize) {
  UIFont *font = [UIFont fontWithName:[NSString stringWithUTF8String:fontFmaily.c_str()]
                                 size:fontSize];
  if (!font) {
    font = [UIFont systemFontOfSize:fontSize];
  }
  return font;
}

static CTFontRef GetCTFont(const xg::canvas::CanvasFont &fontStyle) {
  UIFont *font = GetUIFont(fontStyle.fontFamily, fontStyle.fontSize);
  assert(font != nullptr);
  CTFontRef fontRef = (__bridge CTFontRef)font;
  return fontRef;
}

static NSAttributedString *GetAttributedString(CTFontRef fontRef, const UIColor *textColor,
                                               const std::string &text) {
  NSString *textStr = [NSString stringWithUTF8String:text.c_str()];
  NSMutableAttributedString *attributedString =
      [[NSMutableAttributedString alloc] initWithString:textStr];
  if (fontRef) {
    [attributedString addAttribute:(id)kCTFontAttributeName
                             value:(__bridge id)fontRef
                             range:NSMakeRange(0, [attributedString length])];
  }

  if (textColor) {
    [attributedString addAttribute:(id)kCTForegroundColorAttributeName
                             value:(id)textColor.CGColor
                             range:NSMakeRange(0, [attributedString length])];
  }

  return attributedString;
}

/**
 sizeWithAttributes测量出的高度不正确，需要使用CoreText的测量方法
 NSString *textStr = [NSString stringWithUTF8String:text.c_str()];
 UIFont *font = GetUIFont(fontStyle_->fontFmaily, fontStyle_->fontSize);
 CGSize size = [textStr sizeWithAttributes:@{NSFontAttributeName:font}];
 */
static std::array<float, 2> MeasureTextSize(CTFontRef fontRef, const std::string &text) {
  //测量高度和宽度的时候颜色随便填写
  CFAttributedStringRef aString =
      (__bridge CFAttributedStringRef)GetAttributedString(fontRef, [UIColor whiteColor], text);
  CTLineRef line = CTLineCreateWithAttributedString(aString);
  CGFloat ascent;
  CGFloat descent;
  CGFloat width = CTLineGetTypographicBounds(line, &ascent, &descent, NULL);
  CGFloat height = ascent + descent;
  CFRelease(line);
  return {(float)width, (float)height};
}

bool CoreGraphicsContext::IsValid() { return (canvasContext_ != nullptr); }

void CoreGraphicsContext::SetFillStyle(const std::string &color) {
  TraceCommand("SetFillStyle: " + color);
  if (color != fillColorCache_ && CanvasColorParser::Parse(color, fillColor_)) {
    fillColorCache_ = color;
  }
  CGFloat components[4] = {(fillColor_.r) / 255, (fillColor_.g) / 255, (fillColor_.b) / 255, 1.0};
  CGContextSetFillColor((CGContextRef)canvasContext_, components);
}

void CoreGraphicsContext::SetFillStyle(const CanvasFillStrokeStyle &style) {
  switch (style.type) {
    case CanvasFillStrokeStyleType::kColor: {
      gradient_ = Gradient::None;
      this->SetFillStyle(style.color);
      break;
    }
    case CanvasFillStrokeStyleType::kLinearGradient: {
      gradient_ = Gradient::Linear;
      gradientStyle_ = style;
      break;
    }
    case CanvasFillStrokeStyleType::kRadialGradient: {
      gradient_ = Gradient::Radial;
      gradientStyle_ = style;
      break;
    }
    default:
      gradient_ = Gradient::None;
      break;
  }
}

void CoreGraphicsContext::SetStrokeStyle(const std::string &color) {
  TraceCommand("SetStrokeStyle: " + color);
  if (color != strokeColorCache_ && CanvasColorParser::Parse(color, strokeColor_)) {
    strokeColorCache_ = color;
  }
  CGFloat components[4] = {(strokeColor_.r) / 255, (strokeColor_.g) / 255, (strokeColor_.b) / 255,
                           1.0};
  CGContextSetStrokeColor((CGContextRef)canvasContext_, components);
}

void CoreGraphicsContext::SetStrokeStyle(const canvas::CanvasFillStrokeStyle &style) {
  switch (style.type) {
    case CanvasFillStrokeStyleType::kColor: {
      gradient_ = Gradient::None;
      this->SetStrokeStyle(style.color);
      break;
    }
    case CanvasFillStrokeStyleType::kLinearGradient: {
      gradient_ = Gradient::Linear;
      gradientStyle_ = style;
      break;
    }
    case CanvasFillStrokeStyleType::kRadialGradient: {
      gradient_ = Gradient::Radial;
      gradientStyle_ = style;
      break;
    }
    default:
      gradient_ = Gradient::None;
      break;
  }
}

void CoreGraphicsContext::SetLineCap(const std::string &lineCap) {
  TraceCommand("SetLineCap: " + lineCap);
  NSCAssert(NO, @"unsupport SetLineCap");
  // unsupport
}

void CoreGraphicsContext::SetLineJoin(const std::string &lineJoin) {
  TraceCommand("SetLineJoin: " + lineJoin);
  NSCAssert(NO, @"unsupport SetLineJoin");
  // unsupport
}

void CoreGraphicsContext::SetLineWidth(float lineWidth) {
  TraceCommand("SetLineWidth: " + std::to_string(lineWidth));
  CGContextSetLineWidth(CGContextRef(canvasContext_), lineWidth);
}

void CoreGraphicsContext::SetLineDashOffset(float v) {
  TraceCommand("SetLineDashOffset: " + std::to_string(v));
  NSCAssert(NO, @"unsupport SetLineDashOffset");
  // unsupport
}

void CoreGraphicsContext::SetLineDash(const std::vector<float> &params) {
  TraceCommand("SetLineDash: vec{}");
  CGFloat dashes[params.size()];
  for (int i = 0; i < params.size(); ++i) {
    dashes[i] = params[i];
  }

  CGContextSetLineDash(CGContextRef(canvasContext_), 0, dashes, params.size());
}

void CoreGraphicsContext::SetMiterLimit(float limit) {
  TraceCommand("SetMiterLimit: " + std::to_string(limit));
  CGContextSetMiterLimit(CGContextRef(canvasContext_), limit);
}

void CoreGraphicsContext::SetGlobalAlpha(float globalAlpha) {
  TraceCommand("SetGlobalAlpha: " + std::to_string(globalAlpha));
  globalAlpha_ = globalAlpha;
  CGContextSetAlpha(CGContextRef(canvasContext_), globalAlpha);
}

float CoreGraphicsContext::GlobalAlpha() { return globalAlpha_; }

void CoreGraphicsContext::SetFont(const std::string &font) {
  TraceCommand("SetFont: " + font);
  if (font != fontStyleCache_ && CanvasFontParser::Parse(font, fontStyle_)) {
    fontStyleCache_ = font;
  }
}

/**
 在支付宝上使用下面的渲染方法文字可能渲染不出来
    [textStr drawAtPoint:CGPointMake(x, y) withAttributes:@ {
    NSFontAttributeName: font,
    NSForegroundColorAttributeName: textColor
    }];
 */
void CoreGraphicsContext::DrawText(const std::string &text, const CanvasColor &color, float x,
                                   float y, float maxWidth) {
  //把浮点型转换为size_t 避免用float作为key
  size_t fontSize = fontStyle_.fontSize;
  if (cfObjects_.find(fontSize) == cfObjects_.end()) {
    cfObjects_[fontSize] = std::make_unique<ScopedCFObject>((void *)(GetCTFont(fontStyle_)));
  }

  CTFontRef fontRef = CTFontRef(cfObjects_[fontSize]->object_);

  std::array<float, 2> size = MeasureTextSize(fontRef, text);

  // coretext中下面的baseline设置不起作用，手动计算对齐方式
  //[attributedString addAttribute:(id)kCTBaselineClassAttributeName
  // value:(__bridge id)kCTBaselineClassIdeographicCentered
  // range:NSMakeRange(0, [attributedString length])];
  // todo 需要补齐其它的对齐方式
  if (textAlgin_ == "start") {
    // do nothing
  } else if (textAlgin_ == "end") {
    x -= size[0];
  } else if (textAlgin_ == "center") {
    x -= size[0] / 2;
  } else if (textAlgin_ == "left") {
    // do nothing
  } else if (textAlgin_ == "right") {
    x -= size[0];
  }

  if (textBaseline_ == "top") {
    // do nothing
  } else if (textBaseline_ == "bottom") {
    y -= size[1];
  } else if (textBaseline_ == "middle") {
    y -= size[1] / 2;
  } else {
    // unsupported
  }

  UIColor *textColor = [UIColor colorWithRed:(color.r) / 255
                                       green:(color.g) / 255
                                        blue:(color.b) / 255
                                       alpha:1.0];
  NSAttributedString *attributedString =
      GetAttributedString(GetCTFont(fontStyle_), textColor, text);

  //加上Save 避免矩阵转换影响上下文
  Save();
  // CoreText的0,0在左下角，通过scale翻转为左上角0,0
  CGContextSetTextMatrix(CGContextRef(canvasContext_), CGAffineTransformIdentity);
  CGContextTranslateCTM(CGContextRef(canvasContext_), x, height_ + y);
  CGContextScaleCTM(CGContextRef(canvasContext_), 1.0, -1.0);

  CGMutablePathRef path = CGPathCreateMutable();
  CGPathAddRect(path, NULL, CGRectMake(0, 0, width_, height_));
  CTFramesetterRef framesetter =
      CTFramesetterCreateWithAttributedString((CFAttributedStringRef)attributedString);
  CTFrameRef frame =
      CTFramesetterCreateFrame(framesetter, CFRangeMake(0, [attributedString length]), path, NULL);

  // draw text
  CTFrameDraw(frame, CGContextRef(canvasContext_));
  Restore();

  // realese
  CFRelease(frame);
  CFRelease(path);
  CFRelease(framesetter);
}

void CoreGraphicsContext::FillText(const std::string &text, float x, float y, float maxWidth) {
  TraceCommand("FillText: " + text);
  CGContextSetTextDrawingMode((CGContextRef)canvasContext_, kCGTextFill);
  DrawText(text, fillColor_, x, y);
}

void CoreGraphicsContext::StrokeText(const std::string &text, float x, float y, float maxWidth) {
  TraceCommand("StrokeText: " + text);
  CGContextSetTextDrawingMode((CGContextRef)canvasContext_, kCGTextStroke);
  DrawText(text, strokeColor_, x, y);
}

std::string CoreGraphicsContext::TextAlign() const { return textAlgin_; }

void CoreGraphicsContext::SetTextAlign(const std::string &textAlign) {
  TraceCommand("SetTextAlign " + textAlign);
  textAlgin_ = textAlign;
}

std::string CoreGraphicsContext::TextBaseline() const { return textBaseline_; }

void CoreGraphicsContext::SetTextBaseline(const std::string &textBaseline) {
  TraceCommand("SetTextBaseline" + textBaseline);
  textBaseline_ = textBaseline;
}

void CoreGraphicsContext::StrokeRect(float x, float y, float width, float height) {
  TraceCommand("StrokeRect");
  CGContextStrokeRect(CGContextRef(canvasContext_), CGRectMake(x, y, width, height));
}

void CoreGraphicsContext::Save() {
  TraceCommand("Save");
  CGContextSaveGState(CGContextRef(canvasContext_));
}

void CoreGraphicsContext::Restore() {
  TraceCommand("Restore");
  CGContextRestoreGState(CGContextRef(canvasContext_));
}

bool CoreGraphicsContext::HasClip() {
  TraceCommand("HasClip");
  NSCAssert(NO, @"unsupport HasClip");
  // unsupport
  return false;
}

void CoreGraphicsContext::SetShadowOffsetX(float v) {
  TraceCommand("SetShadowOffsetX: " + std::to_string(v));
  NSCAssert(NO, @"unsupport SetShadowOffsetX");
  // unsupport
}

void CoreGraphicsContext::SetShadowOffsetY(float v) {
  TraceCommand("SetShadowOffsetY: " + std::to_string(v));
  NSCAssert(NO, @"unsupport SetShadowOffsetY");
  // unsupport
}

void CoreGraphicsContext::SetShadowColor(const char *v) {
  TraceCommand("SetShadowColor " + std::string(v));
  NSCAssert(NO, @"unsupport SetShadowColor");
  // unsupport
}

void CoreGraphicsContext::SetShadowBlur(float v) {
  TraceCommand("SetShadowBlur: " + std::to_string(v));
  NSCAssert(NO, @"unsupport SetShadowBlur");
  // unsupport
}

float CoreGraphicsContext::MeasureTextWidth(const std::string &text) {
  //把浮点型转换为size_t 避免用float作为key
  size_t fontSize = fontStyle_.fontSize;
  if (cfObjects_.find(fontStyle_.fontSize) == cfObjects_.end()) {
    cfObjects_[fontSize] = std::make_unique<ScopedCFObject>((void *)(GetCTFont(fontStyle_)));
  }

  CTFontRef fontRef = CTFontRef(cfObjects_[fontSize]->object_);

  float width = MeasureTextSize(fontRef, text)[0];
  TraceCommand("MeasureTextWidth: " + text + " result: " + std::to_string(width));
  return width;
}

void CoreGraphicsContext::Transform(float a, float b, float c, float d, float e, float f) {
  TraceCommand("Transform ");
  // Transform 和 SetTransform有啥区别？
  CGAffineTransform t = CGAffineTransformMake(a, b, c, d, e, f);
  CGContextConcatCTM(CGContextRef(canvasContext_), t);
}

void CoreGraphicsContext::SetTransform(float a, float b, float c, float d, float e, float f) {
  TraceCommand("SetTransform ");
  CGAffineTransform t = CGAffineTransformMake(a, b, c, d, e, f);
  CGContextConcatCTM(CGContextRef(canvasContext_), t);
}

void CoreGraphicsContext::Rect(float x, float y, float width, float height) {
  TraceCommand("Rect ");
  CGContextAddRect(CGContextRef(canvasContext_), CGRectMake(x, y, width, height));
}

void CoreGraphicsContext::ClearRect(float x, float y, float width, float height) {
  TraceCommand("ClearRect ");
  CGContextClearRect(CGContextRef(canvasContext_), CGRectMake(x, y, width, height));
}

void CoreGraphicsContext::FillRect(float x, float y, float width, float height) {
  TraceCommand("FillRect");
  CGContextFillRect(CGContextRef(canvasContext_), CGRectMake(x, y, width, height));
}

void CoreGraphicsContext::Fill(const std::string &fillRule) {
  TraceCommand("Fill");
  if (gradient_ == Gradient::Linear) {
    CGContextClip(CGContextRef(canvasContext_));
    DrawLinearGradient(gradientStyle_);
  } else if (gradient_ == Gradient::Radial) {
    CGContextClip(CGContextRef(canvasContext_));
    DrawRadialGradinet(gradientStyle_);
  } else {
    CGContextFillPath(CGContextRef(canvasContext_));
  }
}

void CoreGraphicsContext::Stroke() {
  TraceCommand("Stroke");
  if (gradient_ == Gradient::Linear) {
    CGContextClip(CGContextRef(canvasContext_));
    DrawLinearGradient(gradientStyle_);
  } else if (gradient_ == Gradient::Radial) {
    CGContextClip(CGContextRef(canvasContext_));
    DrawRadialGradinet(gradientStyle_);
  } else {
    CGContextStrokePath(CGContextRef(canvasContext_));
  }
}

void CoreGraphicsContext::BeginPath() {
  TraceCommand("BeginPath");
  CGContextBeginPath(CGContextRef(canvasContext_));
}

void CoreGraphicsContext::MoveTo(float x, float y) {
  TraceCommand("MoveTo x: " + std::to_string(x) + ", " + std::to_string(y));
  CGContextMoveToPoint(CGContextRef(canvasContext_), x, y);
}

void CoreGraphicsContext::ClosePath() {
  TraceCommand("ClosePath");
  CGContextClosePath(CGContextRef(canvasContext_));
}

void CoreGraphicsContext::LineTo(float x, float y) {
  TraceCommand("LineTo x: " + std::to_string(x) + ", " + std::to_string(y));
  CGContextAddLineToPoint(CGContextRef(canvasContext_), x, y);
}

void CoreGraphicsContext::Clip(const std::string &fillRule) {
  TraceCommand("Clip");
  CGContextClip(CGContextRef(canvasContext_));
}

void CoreGraphicsContext::QuadraticCurveTo(float cpx, float cpy, float x, float y) {
  CGContextAddQuadCurveToPoint(CGContextRef(canvasContext_), cpx, cpy, x, y);
}

void CoreGraphicsContext::BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x,
                                        float y) {
  TraceCommand("BezierCurveTo");
  CGContextAddCurveToPoint(CGContextRef(canvasContext_), cp1x, cp1y, cp2x, cp2y, x, y);
}

void CoreGraphicsContext::Arc(float x, float y, float r, float sAngle, float eAngle,
                              bool antiClockwise) {
  TraceCommand("Arc");
  CGContextAddArc(CGContextRef(canvasContext_), x, y, r, sAngle, eAngle, antiClockwise);
}

void CoreGraphicsContext::ArcTo(float x1, float y1, float x2, float y2, float r) {
  TraceCommand("ArcTo");
  CGContextAddArcToPoint(CGContextRef(canvasContext_), x1, y1, x2, y2, r);
}

void CoreGraphicsContext::Scale(float sx, float sy) {
  TraceCommand("Scale width: " + std::to_string(sx) + ", height: " + std::to_string(sy));
  CGContextScaleCTM(CGContextRef(canvasContext_), sx, sy);
}

void CoreGraphicsContext::Rotate(float angle) {
  TraceCommand("Rotate angle: " + std::to_string(angle));
  CGContextRotateCTM(CGContextRef(canvasContext_), angle);
}

void CoreGraphicsContext::Translate(float x, float y) {
  TraceCommand("Translate x: " + std::to_string(x) + ", y: " + std::to_string(y));
  CGContextTranslateCTM(CGContextRef(canvasContext_), x, y);
}

void CoreGraphicsContext::DrawImage(CanvasImage *image, float dx, float dy) {
  TraceCommand("DrawImage x: " + std::to_string(dx) + ", y: " + std::to_string(dy));
  if (image->GetImage()) {
    CGRect rect = CGRectMake(dx, dy, image->GetWidth(), image->GetHeight());
    CGContextDrawImage(CGContextRef(canvasContext_), rect, (CGImageRef)(image->GetImage()));
  }
}

void CoreGraphicsContext::DrawImage(CanvasImage *image, float dx, float dy, float width,
                                    float height) {
  TraceCommand("DrawImage x: " + std::to_string(dx) + ", y: " + std::to_string(dy) +
               ", width: " + std::to_string(width) + ", height: " + std::to_string(height));
  if (image->GetImage()) {
    CGRect rect = CGRectMake(dx, dy, width, height);
    CGContextDrawImage(CGContextRef(canvasContext_), rect, (CGImageRef)(image->GetImage()));
  }
}

void CoreGraphicsContext::DrawLinearGradient(const canvas::CanvasFillStrokeStyle &style) {
  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  size_t size = style.linearGradient.colorStops.size();
  CGFloat locations[size];
  int index = 0;

  CFMutableArrayRef array = CFArrayCreateMutable(NULL, 0, NULL);
  std::for_each(style.linearGradient.colorStops.begin(), style.linearGradient.colorStops.end(),
                [&](auto &item) -> void {
                  locations[index] = item.offset;
                  CFArrayAppendValue(array, GetUIColorFromHexString(item.color, 1.0).CGColor);
                  ++index;
                });
  CGPoint start = CGPointMake(style.linearGradient.start[0], style.linearGradient.start[1]);
  CGPoint end = CGPointMake(style.linearGradient.end[0], style.linearGradient.end[1]);
  CGGradientRef gradient = CGGradientCreateWithColors(colorSpace, array, locations);
  CGContextDrawLinearGradient(
      CGContextRef(canvasContext_), gradient, start, end,
      kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
  CGColorSpaceRelease(colorSpace);
  CGGradientRelease(gradient);
  CFRelease(array);
}

void CoreGraphicsContext::DrawRadialGradinet(const canvas::CanvasFillStrokeStyle &style) {
  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  size_t size = style.radialGradient.colorStops.size();
  CGFloat locations[size];
  int index = 0;

  CFMutableArrayRef array = CFArrayCreateMutable(NULL, 0, NULL);
  std::for_each(style.radialGradient.colorStops.begin(), style.radialGradient.colorStops.end(),
                [&](auto &item) -> void {
                  locations[index] = item.offset;
                  CFArrayAppendValue(array, GetUIColorFromHexString(item.color, 1.0).CGColor);
                  ++index;
                });
  CGPoint start = CGPointMake(style.radialGradient.start[0], style.radialGradient.start[1]);
  CGFloat startRadius = style.radialGradient.start[2];

  CGPoint end = CGPointMake(style.radialGradient.end[0], style.radialGradient.end[1]);
  CGFloat endRadius = style.radialGradient.end[2];
  CGGradientRef gradient = CGGradientCreateWithColors(colorSpace, array, locations);
  CGContextDrawRadialGradient(
      CGContextRef(canvasContext_), gradient, start, startRadius, end, endRadius,
      kCGGradientDrawsBeforeStartLocation | kCGGradientDrawsAfterEndLocation);
  CGColorSpaceRelease(colorSpace);
  CGGradientRelease(gradient);
  CFRelease(array);
}

#endif  // APPLE
