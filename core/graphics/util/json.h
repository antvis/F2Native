//
//  json.h
//  AFWFinanceChart
//
//  Created by XiaLuo on 2020/12/14.
//  Copyright © 2020 Alipay. All rights reserved.
//

#include <nlohmann/json.hpp>
#include <vector>

#ifndef XG_GRAPHICS_JSON_H
#define XG_GRAPHICS_JSON_H

namespace xg {
namespace json {

nlohmann::json ParseString(const std::string &json);

std::vector<float> ParseDashArray(const nlohmann::json &json, float ratio = 1.0f, std::vector<float> defVal = {10, 10});

void ParseRoundings(const nlohmann::json &data, float *rst, float ratio = 1.0f);

} // namespace json
} // namespace xg

#endif /* XG_GRAPHICS_JSON_H */
