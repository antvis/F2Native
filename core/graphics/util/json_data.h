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
#include "Point.h"
#include "../global.h"

namespace xg {
namespace util {
    struct TagCfg {
        float offset = -5; // 距离柱子顶部的偏移量
        string textAlign = "center";
        string textBaseline = "bottom";
        string fill = "#808080";
        size_t textSize = DEFAULT_FONTSIZE;
        string content;
        bool hiden = true;
    };
    //impl in interval.cpp
    extern void from_json(const nlohmann::json &j, TagCfg &t);

    struct XData final {
        nlohmann::json::const_pointer data;
        double _x = NAN, _y = NAN;
        std::vector<double> _y0;
        nlohmann::json _style;
        std::string _color, _shape, _adjust;
        double _size = NAN;
        
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
        array<util::Point, 4> rect;
        array<double, 2> line;
        std::size_t state = 0; // 0 - 平, 1 涨， -1 跌
    };

using XDataArray = std::vector<XData>;
using XDataGroup = std::vector<std::vector<XData>>;
}
}


#endif /* XG_GRAPHICS_UTIL_JSON_DATA */
