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
        nlohmann::json data;
        double _x, _y;
        std::vector<double> _y0;
        size_t xField, yField;
        nlohmann::json _style;
        std::string _color, _shape, _adjust;
        double _size;
    };

using XDataArray = std::vector<XData>;
using XDataGroup = std::vector<std::vector<XData>>;
}
}


#endif /* XG_GRAPHICS_UTIL_JSON_DATA */
