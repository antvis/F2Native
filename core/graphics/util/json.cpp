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

nlohmann::json ParseString(const std::string &json) { return nlohmann::json::parse(json, nullptr, false); }

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

void ParseRoundings(const nlohmann::json &data, float *rst, float ratio) {
    if(data.is_array() && data.size() >= 4) {
        *rst = data[0].get<float>() * ratio;
        *(rst + 1) = data[1].get<float>() * ratio;
        *(rst + 2) = data[2].get<float>() * ratio;
        *(rst + 3) = data[3].get<float>() * ratio;
    }
}

nlohmann::json Get(const nlohmann::json &obj, const std::string &key) {
    if(obj.is_object() && obj.contains(key)) {
        return obj[key];
    } else {
        return nlohmann::json();
    }
}

const std::string GetString(const nlohmann::json &obj, const std::string &key, const std::string &def) {
    if(obj.is_object() && obj.contains(key) && obj[key].is_string()) {
        return obj[key].get<std::string>();
    } else {
        return def;
    }
}

const double GetNumber(const nlohmann::json &obj, const std::string &key, const double def) {
    if(obj.is_object() && obj.contains(key) && obj[key].is_number()) {
        return obj[key].get<double>();
    } else {
        return def;
    }
}

const int GetIntNumber(const nlohmann::json &obj, const std::string &key, const int def) {
    if(obj.is_object() && obj.contains(key) && obj[key].is_number_integer()) {
        return obj[key].get<int>();
    } else {
        return def;
    }
}

const float GetFloatNumber(const nlohmann::json &obj, const std::string &key, const float def) {
    if(obj.is_object() && obj.contains(key) && obj[key].is_number()) {
        return obj[key].get<float>();
    } else {
        return def;
    }
}

const nlohmann::json &GetArray(const nlohmann::json &obj, const std::string &key, const nlohmann::json &defVal) {
    if(obj.is_object() && obj.contains(key) && obj[key].is_array()) {
        return obj[key];
    } else {
        return defVal;
    }
}

const bool GetBool(const nlohmann::json &obj, const std::string &key, const bool defVal) {
    if(obj.is_object() && obj.contains(key) && obj[key].is_boolean()) {
        return obj[key].get<bool>();
    } else {
        return defVal;
    }
}

const nlohmann::json &GetObject(const nlohmann::json &obj, const std::string &key, const nlohmann::json &defVal) {
    if(obj.is_object() && obj.contains(key) && obj[key].is_object()) {
        return obj[key];
    } else {
        return defVal;
    }
}

} // namespace json
} // namespace xg
