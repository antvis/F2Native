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

/// 创建一个空的对象，方便返回空对象的时候使用
/// @return nlohmann::json  type is null
const nlohmann::json &NullObject();

/// 创建一个空的数组，方便返回空对象的时候使用
/// /// @return nlohmann::json  type is array
const nlohmann::json &ArrayObject();

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
//@return 如果找不到key，会返回一个空数组对象  可以使用obj.is_array() && obj.size() == 0 来判断
const nlohmann::json &GetArray(const nlohmann::json &obj, const std::string &key, const nlohmann::json &defVal = ArrayObject());

const bool GetBool(const nlohmann::json &obj, const std::string &key, const bool defVal = false);

//使用&会地址不正确
//@return 如果找不到key，会返回一个null object,  可以使用obj.is_null()来判断
const nlohmann::json &GetObject(const nlohmann::json &obj, const std::string &key, const nlohmann::json &defVal = NullObject());
} // namespace json
} // namespace xg

#endif /* XG_GRAPHICS_JSON_H */
