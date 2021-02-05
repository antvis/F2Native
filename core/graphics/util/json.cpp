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

nlohmann::json ParseString(const std::string &json) { return nlohmann::json::parse(json, nullptr, false); }

std::vector<float> ParseDashArray(const nlohmann::json &json, float ratio, std::vector<float> defVal) {
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
