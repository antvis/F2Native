//
//  json_data.h
//  F2
//
//  Created by weiqing.twq on 2022/6/1.
//

#ifndef XG_GRAPHICS_UTIL_JSON_DATA
#define XG_GRAPHICS_UTIL_JSON_DATA

#include <string>
#include <vector>
#include <array>
#include "Point.h"
#include "../global.h"
#include "../canvas/CanvasFillStrokeStyle.h"
#include "../../reflection/reflection.h"

namespace xg {
namespace util {
    struct TagCfg {
        float offset = -5; // 距离柱子顶部的偏移量
        std::string textAlign = "center";
        std::string textBaseline = "bottom";
        std::string fill = "#808080";
        size_t textSize = DEFAULT_FONTSIZE;
        std::string content = "";
        bool hiden = true;
        
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(TagCfg)
        FIELDS(FIELD(&TagCfg::offset),
               FIELD(&TagCfg::textAlign),
               FIELD(&TagCfg::textBaseline),
               FIELD(&TagCfg::fill),
               FIELD(&TagCfg::textSize),
               FIELD(&TagCfg::content),
               FIELD(&TagCfg::hiden))
        CTORS(DEFAULT_CTOR(TagCfg))
    END_TYPE
#endif
    };

    struct XData final {
        //源数据
        const std::unordered_map<std::string, Any> *data;
        
        //扩展数据
        double _x = NAN, _y = NAN;
        std::vector<double> _y0;
        std::string _shape, _adjust;
        canvas::CanvasFillStrokeStyle _color;
        double _size = NAN;
        double _opacity = NAN;
        
        //for interval and candle
        TagCfg _tag;
        bool _beforeMapped = false;
        
        //for adjust
        std::vector<double> adjust;
        
        //for dodge
        //index 0 for xfiled, index 1 for yfield
        std::vector<double> dodge;
        
        //area interval
        std::vector<util::Point> points;
        
        //candle
        std::array<util::Point, 4> rect;
        std::array<double, 2> line;
        std::size_t state = 0; // 0 - 平, 1 涨， -1 跌
    };

using XSourceItem = std::unordered_map<std::string, Any>;
using XSourceArray = std::vector<std::unordered_map<std::string, Any>>;

using XDataArray = std::vector<XData>;
using XDataGroup = std::vector<std::vector<XData>>;
}
}


#endif /* XG_GRAPHICS_UTIL_JSON_DATA */
