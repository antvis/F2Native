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

namespace xg {
namespace util {
    struct XData final {
        nlohmann::json::const_pointer data;
        double _x = NAN, _y = NAN;
        std::vector<double> _y0;
        nlohmann::json _style;
        std::string _color, _shape, _adjust;
        double _size = NAN;
        
        //for interval and candle
        nlohmann::json _points, _tag, _rect, _line, _state;
        bool _beforeMapped = false;
        
        //for adjust
        std::vector<double> adjust;
        
        //for dodge
        //index 0 for xfiled, index 1 for yfield
        std::vector<double> dodge;
        
        //area
        std::vector<util::Point> points;
    };

using XDataArray = std::vector<XData>;
using XDataGroup = std::vector<std::vector<XData>>;
}
}


#endif /* XG_GRAPHICS_UTIL_JSON_DATA */
