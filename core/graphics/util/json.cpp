//
//  json.cpp
//  AFWFinanceChart
//
//  Created by XiaLuo on 2020/12/14.
//  Copyright © 2020 Alipay. All rights reserved.
//

#include "json.h"

namespace xg {
namespace json {

const nlohmann::json &NullObject() {
    static nlohmann::json nullobj;
    return nullobj;
}

const nlohmann::json &ArrayObject() {
    static nlohmann::json array = nlohmann::json::array();
    return array;
}

std::vector<float> ParseDashArray(const nlohmann::json &json, float ratio, const std::vector<float> &defVal) {
    if(json.is_array() && json.size() > 0) {
        std::vector<float> rst;
        for(std::size_t i = 0; i < json.size(); ++i) {
            if(json[i].is_number()) {
                rst.push_back(json[i].get<float>() * ratio);
            }
        }

        return rst;
    }
    return defVal;
}

std::vector<float> ScaleDash(const std::vector<float> &dash, float scale) {
    std::vector<float> rst;
    for(std::size_t i = 0; i < dash.size(); ++i) {
            rst.push_back(dash[i] * scale);
    }

    return rst;
}

std::array<float, 4> ScaleRoundings(const std::array<float, 4> roundings, float scale) {
    std::array<float, 4> rst;
    rst[0] = roundings[0] * scale;
    rst[1] = roundings[1] * scale;
    rst[2] = roundings[2] * scale;
    rst[3] = roundings[3] * scale;
    return rst;
}

std::vector<float> ScaleRoundings(const std::vector<float> roundings, float scale) {
    std::vector<float> rst;
    rst[0] = roundings[0] * scale;
    rst[1] = roundings[1] * scale;
    rst[2] = roundings[2] * scale;
    rst[3] = roundings[3] * scale;
    return rst;
}

void ParseRoundings(const nlohmann::json &data, float *rst, float ratio) {
    if(data.is_array() && data.size() >= 4) {
        *rst = data[0].get<float>() * ratio;
        *(rst + 1) = data[1].get<float>() * ratio;
        *(rst + 2) = data[2].get<float>() * ratio;
        *(rst + 3) = data[3].get<float>() * ratio;
    }
}

} // namespace json
} // namespace xg
