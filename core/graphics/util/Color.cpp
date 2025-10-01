//
//  Color.c
//  AFWFinanceChart
//
//  Created by weiqing.twq on 2021/12/1.
//  Copyright © 2021 Alipay. All rights reserved.
//

#include "Color.h"
#include "graphics/util/json.h"

using namespace xg::canvas;

CanvasFillStrokeStyle xg::util::ColorParser(const nlohmann::json &color, xg::util::BBox *bbox) {
    if(color.is_string()) {
        const std::string &c = color.get<std::string>();

        //不是渐变
        if(c.find("colorStops") == std::string::npos) {
            //解析"#1890FF0F"格式传递到canvas后会引起颜色解析问题
            if(c.length() > 1 && c.substr(0, 1) == "#") {
                //截取超长的color 如#ff0000ff, 截取成#ff0000
                return CanvasFillStrokeStyle(c.length() > 7 ? c.substr(0, 7) : c);
            }else {
                return CanvasFillStrokeStyle(c);
            }
        }
        nlohmann::json _color = nlohmann::json::parse(c, nullptr, false);
        if(_color.is_object() && _color.contains("colorStops") && _color["colorStops"].is_array()) {
            const auto &gradientStyle = json::GetString(_color, "gradient");
            bool isLinear = _color.contains("isLinear") ? _color["isLinear"].get<bool>() : true;
            if(gradientStyle == "conic") {
                CanvasConicGradient gradient;
                nlohmann::json &colorStops = _color["colorStops"];
                for(std::size_t i = 0; i < colorStops.size(); ++i) {
                    nlohmann::json &stop = colorStops[i];
                    gradient.addColorStop(stop["offset"], stop["color"]);
                }
                gradient.start = {bbox->minX, bbox->minY};
                gradient.end = {bbox->maxX, bbox->maxY};
                gradient.gradientColor = gradientStyle;
                return gradient;
            } else if(isLinear || gradientStyle == "linear") {
                CanvasLinearGradient gradient;
                nlohmann::json &colorStops = _color["colorStops"];
                for(std::size_t i = 0; i < colorStops.size(); ++i) {
                    nlohmann::json &stop = colorStops[i];
                    gradient.addColorStop(stop["offset"], stop["color"]);
                }

                if(_color.contains("position") && _color["position"].size() >= 4) {
                    nlohmann::json &position = _color["position"];
                    gradient.start = {position[0], position[1]};
                    gradient.end = {position[2], position[3]};
                } else if (bbox != nullptr) {
                    gradient.start = {bbox->minX, bbox->maxY};
                    gradient.end = {bbox->minX, bbox->minY};
                    if (_color.contains("angle")) {
                        double angle = xg::json::GetNumber(_color, "angle", 0);
                        // 0 从上到下，1 从右到左，2 从下到上，3 从左到右
                        int direction = angle / 90;
                        if (direction == 1) {
                            gradient.start = {bbox->maxX, bbox->minY};
                            gradient.end = {bbox->minX, bbox->minY};
                        } else if (direction == 2) {
                            gradient.start = {bbox->minX, bbox->minY};
                            gradient.end = {bbox->minX, bbox->maxY};
                        } else if (direction == 3) {
                            gradient.start = {bbox->minX, bbox->minY};
                            gradient.end = {bbox->maxX, bbox->minY};
                        }
                    }
                }
                gradient.gradientColor = gradientStyle;
                return gradient;
            } else {
                CanvasRadialGradient gradient;

                nlohmann::json &colorStops = _color["colorStops"];
                for(std::size_t i = 0; i < colorStops.size(); ++i) {
                    nlohmann::json &stop = colorStops[i];
                    gradient.addColorStop(stop["offset"], stop["color"]);
                }

                if(_color.contains("position") && _color["position"].size() >= 6) {
                    nlohmann::json &position = _color["position"];
                    gradient.start[0] = position[0];
                    gradient.start[1] = position[1];
                    gradient.start[2] = position[2];

                    gradient.end[0] = position[3];
                    gradient.end[1] = position[4];
                    gradient.end[2] = position[5];
                }
                gradient.gradientColor = gradientStyle;
                return gradient;
            }
        }
    }
    return CanvasFillStrokeStyle(GLOBAL_COLORS[0]);
}

CanvasFillStrokeStyle
xg::util::ColorParser(const nlohmann::json &data, const std::string &key, xg::util::BBox *bbox) {
    if(data.is_object() && data.contains(key)) {
        return ColorParser(data[key], bbox);
    }
    return CanvasFillStrokeStyle(GLOBAL_COLORS[0]);
}

float xg::util::OpacityParserString(const std::string &color) {
    if(color.length() > 7 && color.substr(0, 1) == "#") {
        //截取超长的color 如#ff0000ff
        const std::string c =  color.substr(7,  color.length());
        long n = strtol(c.c_str(), NULL, 16);
        return n / 255.0f;
    };
    return DEFAULT_OPACITY;
}

float xg::util::OpacityParser(const nlohmann::json &color) {
    if(color.is_string()) {
        return OpacityParserString(color.get<std::string>());
    }
    return DEFAULT_OPACITY;
}

float xg::util::OpacityParser(const nlohmann::json &data, const std::string &key) {
    if(data.is_object() && data.contains(key)) {
        return OpacityParser(data[key]);
    }
    return DEFAULT_OPACITY;
}
