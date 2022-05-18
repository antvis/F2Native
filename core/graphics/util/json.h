//
//  json.h
//  AFWFinanceChart
//
//  Created by XiaLuo on 2020/12/14.
//  Copyright © 2020 Alipay. All rights reserved.
//

#ifndef XG_GRAPHICS_JSON_H
#define XG_GRAPHICS_JSON_H

#include <vector>
#include "../../nlohmann/json.hpp"

namespace xg {
namespace json {

nlohmann::json ParseString(const std::string &json);

std::vector<float> ParseDashArray(const nlohmann::json &json, float ratio = 1.0f, const std::vector<float> &defVal = {10, 10});

void ParseRoundings(const nlohmann::json &data, float *rst, float ratio = 1.0f);

nlohmann::json Get(const nlohmann::json &obj, const std::string &key);

const std::string GetString(const nlohmann::json &obj, const std::string &key, const std::string &defVal = "");

//默认值defVal给NAN更合理
const double GetNumber(const nlohmann::json &obj, const std::string &key, const double defVal = 0);
const int GetIntNumber(const nlohmann::json &obj, const std::string &key, const int def = NAN) ;
const float GetFloatNumber(const nlohmann::json &obj, const std::string &key, const float def = NAN);

//使用&会地址不正确
const nlohmann::json GetArray(const nlohmann::json &obj, const std::string &key, const nlohmann::json &defVal = nlohmann::json::array());

const bool GetBool(const nlohmann::json &obj, const std::string &key, const bool defVal = false);

//使用&会地址不正确
const nlohmann::json GetObject(const nlohmann::json &obj, const std::string &key, const nlohmann::json &defVal = nlohmann::json::object());


} // namespace json
} // namespace xg

#endif /* XG_GRAPHICS_JSON_H */
