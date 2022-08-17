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

Any FromMapByObj(const unordered_map<string, Any> &data, const Any &obj) {
    const auto &type = obj.GetType().GetType();
    for (const auto &i : data) {
        auto field = type->GetField(i.first);
        if (!field) {
            continue;
        }     
        field->Set(obj, i.second);
    }
    return obj;
}

Any FromMapByType(const unordered_map<string, Any> &data, const Type* type) {
    Any obj = type->GetConstructor()->Invoke();
    FromMapByObj(data, obj);
    return obj;
}


std::vector<float> ScaleDash(const std::vector<float> &dash, float scale) {
    std::vector<float> rst;
    for(std::size_t i = 0; i < dash.size(); ++i) {
            rst.push_back(dash[i] * scale);
    }

    return rst;
}

std::vector<float> ScaleRoundings(const std::vector<float> &roundings, float scale) {
    std::vector<float> rst {roundings[0] * scale, roundings[1] * scale, roundings[2] * scale, roundings[3] * scale };
    return rst;
}

} // namespace json
} // namespace xg
