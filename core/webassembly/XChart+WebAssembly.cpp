//
//  XChart+WebAssembly.cpp
//  F2
//
//  Created by weiqing.twq on 2022/2/21.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#if defined(__EMSCRIPTEN__)
#include "graphics/XChart.h"
#include "graphics/geom/Geom.h"
#include "graphics/geom/Interval.h"
#include "graphics/geom/Line.h"
#include "graphics/shape/Group.h"
#include "utils/Tracer.h"
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

using namespace emscripten;
using namespace xg;
using namespace utils;

//该文件主要的作用是绑定XChart的API到Webassmebly

////////////////////////////////////////////////////////////////////////
// graphics/XChart.h
EMSCRIPTEN_BINDINGS(xchart) {
    class_<XChart>("F2Chart")
        .constructor<std::string, double, double, double, bool>()
        .function("source", &XChart::SourceWasm, allow_raw_pointers()) //普通类成员函数
        .function("padding", &XChart::PaddingWasm, allow_raw_pointers())
        .function("margin", &XChart::MarginWasm, allow_raw_pointers())
        // .function("parse", &XChart::Parse)
        .function("render", &XChart::Render)
        .function("clear", &XChart::Clear)
        .function("repaint", &XChart::Repaint)
        .function("onTouchEvent", &XChart::OnTouchEvent)
        .function("callback", &XChart::SetInvokeFunction, allow_raw_pointers())
        .function("axis", &XChart::AxisWasm, allow_raw_pointers())
        .function("canvas", &XChart::SetCanvasContextWasm, allow_raw_pointers())
        .function("scale", &XChart::ScaleWasm, allow_raw_pointers())
        .function("renderDuration", &XChart::GetRenderDurationMM)
        .function("line", &XChart::LineWasm, allow_raw_pointers())
        .function("interval", &XChart::IntervalWasm, allow_raw_pointers())
        .function("area", &XChart::AreaWasm, allow_raw_pointers())
        .function("point", &XChart::PointWasm, allow_raw_pointers())
        .function("candle", &XChart::CandleWasm, allow_raw_pointers())
        .function("interaction", &XChart::InteractionWasm, allow_raw_pointers())
        .function("guide", &XChart::GuideWasm, allow_raw_pointers());
}
////////////////////////////////////////////////////////////////////////
// graphics/geom/geom.h
EMSCRIPTEN_BINDINGS(geom) {
    class_<geom::AbstractGeom>("Geom")
        .function("position", &geom::AbstractGeom::PositionWasm, allow_raw_pointers())
        .function("color", select_overload<geom::AbstractGeom *(const std::string &, const vector<string> &)>(&geom::AbstractGeom::ColorWasm),
                  allow_raw_pointers())
        .function("color", select_overload<geom::AbstractGeom *(const std::string &)>(&geom::AbstractGeom::ColorWasm), allow_raw_pointers())
        .function("size", select_overload<geom::AbstractGeom *(const std::string &, const vector<float> &)>(&geom::AbstractGeom::SizeWasm),
                  allow_raw_pointers())
        .function("size", select_overload<geom::AbstractGeom *(const float)>(&geom::AbstractGeom::SizeWasm), allow_raw_pointers())
        .function("shape", select_overload<geom::AbstractGeom *(const std::string &, const vector<std::string> &)>(&geom::AbstractGeom::ShapeWasm),
                  allow_raw_pointers())
        .function("shape", select_overload<geom::AbstractGeom *(const std::string &)>(&geom::AbstractGeom::ShapeWasm), allow_raw_pointers())
        .function("adjust", &geom::AbstractGeom::AdjustWasm, allow_raw_pointers())
        .function("style", &geom::AbstractGeom::StyleWasm, allow_raw_pointers());
    class_<geom::Line, base<geom::AbstractGeom>>("Line").constructor<shape::Group *, utils::Tracer *>();
    class_<geom::Interval, base<geom::AbstractGeom>>("Interval").constructor<shape::Group *, utils::Tracer *>();
    class_<geom::Area, base<geom::AbstractGeom>>("Area").constructor<shape::Group *, utils::Tracer *>();
    class_<geom::Point, base<geom::AbstractGeom>>("Point").constructor<shape::Group *, utils::Tracer *>();
    class_<geom::Candle, base<geom::AbstractGeom>>("Candle").constructor<shape::Group *, utils::Tracer *>();
}
////////////////////////////////////////////////////////////////////////
EMSCRIPTEN_BINDINGS(guide_flagcfg) {
    value_object<guide::FlagCfg>("GuideFlagCfg")
            .field("color", &guide::FlagCfg::color)
            .field("textSize", &guide::FlagCfg::textSize) 
            .field("textColor", &guide::FlagCfg::textColor)
            .field("textAlign", &guide::FlagCfg::textAlign)
            .field("textBaseline", &guide::FlagCfg::textBaseline)
            .field("top", &guide::FlagCfg::top)
            .field("position", &guide::FlagCfg::position)
            .field("content", &guide::FlagCfg::content)
            .field("radius", &guide::FlagCfg::radius)
            .field("padding", &guide::FlagCfg::padding)
            .field("rounding", &guide::FlagCfg::rounding)
            .field("lineWidth", &guide::FlagCfg::lineWidth)
            .field("backgroundColor", &guide::FlagCfg::backgroundColor);
}

EMSCRIPTEN_BINDINGS(guide_textcfg) {
    value_object<guide::TextCfg>("GuideTextCfg")
            .field("content", &guide::TextCfg::content)
            .field("textColor", &guide::TextCfg::textColor) 
            .field("textSize", &guide::TextCfg::textSize)
            .field("textAlign", &guide::TextCfg::textAlign)
            .field("textBaseline", &guide::TextCfg::textBaseline)
            .field("top", &guide::TextCfg::top)
            .field("position", &guide::TextCfg::position)
            .field("margin", &guide::TextCfg::margin);
}

EMSCRIPTEN_BINDINGS(guide_linecfg) {
    value_object<guide::LineCfg>("GuideLineCfg")
            .field("color", &guide::LineCfg::color)
            .field("top", &guide::LineCfg::top)
            .field("position", &guide::LineCfg::position)
            .field("orientation", &guide::LineCfg::orientation)
            .field("dash", &guide::LineCfg::dash)
            .field("lineWidth", &guide::LineCfg::lineWidth);
}

EMSCRIPTEN_BINDINGS(guide_backgroundcfg) {
    value_object<guide::BackgroundCfg>("GuideBackgroundCfg")
            .field("color", &guide::BackgroundCfg::color)
            .field("top", &guide::BackgroundCfg::top)
            .field("leftBottom", &guide::BackgroundCfg::leftBottom)
            .field("rightTop", &guide::BackgroundCfg::rightTop);
}

EMSCRIPTEN_BINDINGS(guide_imagecfg) {
    value_object<guide::ImageCfg>("GuideImageCfg")
            .field("margin", &guide::ImageCfg::margin)
            .field("top", &guide::ImageCfg::top)
            .field("width", &guide::ImageCfg::width)
            .field("height", &guide::ImageCfg::height)
            .field("src", &guide::ImageCfg::src);
}


// graphics/guide/GuideController.h
EMSCRIPTEN_BINDINGS(guide) {
    class_<guide::GuideController>("Guide")
        .function("flag", &guide::GuideController::FlagObject)
        .function("text", &guide::GuideController::TextObject)
        .function("line", &guide::GuideController::LineObject)
        .function("background", &guide::GuideController::BackgroundObject)
        .function("image", &guide::GuideController::ImageObject);
}

////////////////////////////////////////////////////////////////////////
// vector
EMSCRIPTEN_BINDINGS(module) {
    emscripten::register_vector<std::string>("StringVector");
    emscripten::register_vector<float>("NumberVector");
}

#endif
