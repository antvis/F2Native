//
//  CanvasFillStrokeStyle.h
//  F2
//
//  Created by weiqing.twq on 2022/1/19.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#ifndef XG_GRAPHICS_CANVAS_CANVASFILLSTROKESTYLE_H
#define XG_GRAPHICS_CANVAS_CANVASFILLSTROKESTYLE_H

#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cmath>

namespace xg {
namespace canvas {

struct ColorStop {
    float offset;
    std::string color;

    ColorStop() {}

    ColorStop(float value_offset, const std::string &value_color) {
        offset = value_offset;
        color = value_offset;
    }
};

class CanvasGradient {

  public:
    CanvasGradient() = default;

    CanvasGradient(const CanvasGradient &g) {
        for(auto i = 0; i < g.colorStops.size(); i++) {
            const ColorStop &cs = g.colorStops[i];
            this->addColorStop(cs.offset, cs.color);
        }
    }

    virtual bool isLinearGradient() { return true; };

    /// 标准接口, 添加colorStop
    /// offset
    /// color
    void addColorStop(float offset, const std::string &color) {
        ColorStop cs;
        cs.offset = offset;
        cs.color = color;
        colorStops.push_back(cs);
    }

    std::vector<ColorStop> colorStops;
};

class CanvasLinearGradient : public CanvasGradient {
  public:
    CanvasLinearGradient() = default;

    CanvasLinearGradient(const CanvasLinearGradient &g) : CanvasGradient(g) {
        start = g.start;
        end = g.end;
    }

    std::array<float, 2> start = {0}, end = {0};

    bool isLinearGradient() override { return true; }
};

class CanvasRadialGradient : public CanvasGradient {
  public:
    CanvasRadialGradient() = default;

    CanvasRadialGradient(const CanvasRadialGradient &g) : CanvasGradient(g) {
        start = g.start;
        end = g.end;
    }

    std::array<float, 3> start = {0}, end = {0};

    bool isLinearGradient() override { return false; }
};

enum class CanvasFillStrokeStyleType { kNone, kColor, kLinearGradient, kRadialGradient };

struct CanvasFillStrokeStyle {
    CanvasFillStrokeStyle() = default;

    CanvasFillStrokeStyle(const CanvasFillStrokeStyle &style)
        : type(style.type), color(style.color), linearGradient(style.linearGradient), radialGradient(style.radialGradient) {}

    CanvasFillStrokeStyle(const std::string &color) {
        type = CanvasFillStrokeStyleType::kColor;
        this->color = color;
    }

    CanvasFillStrokeStyle(const CanvasLinearGradient &g) {
        type = CanvasFillStrokeStyleType::kLinearGradient;
        this->linearGradient = g;
    }

    CanvasFillStrokeStyle(const CanvasRadialGradient &g) {
        type = CanvasFillStrokeStyleType::kRadialGradient;
        this->radialGradient = g;
    }

    CanvasFillStrokeStyleType type = CanvasFillStrokeStyleType::kNone;

    std::string color = "";

    // ref to pattern / linear gradient / radial gradient
    CanvasLinearGradient linearGradient;
    CanvasRadialGradient radialGradient;
    
    inline bool Empty() const { return type == CanvasFillStrokeStyleType::kNone;}
};

}//canvas
}//xg
#endif /* XG_GRAPHICS_CANVAS_CANVASFILLSTROKESTYLE_H */
