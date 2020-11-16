#ifndef XG_GRAPHICS_UTIL_JSON_H
#define XG_GRAPHICS_UTIL_JSON_H

#include <array>
#include <iostream>
#include <limits>
#include <math.h>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace xg {
namespace util {

// 从原始数组中查找出所有指定的 key 字段值组成新的数组返回
static nlohmann::json JsonArrayByKey(const nlohmann::json &data, const std::string &key) {
    if(!data.is_array() || data.size() <= 0) {
        return nlohmann::json();
    }

    nlohmann::json rst;
    unordered_map<string, string> stringMap;

    for(int i = 0; i < data.size(); ++i) {
        auto &item = data[i];
        if(item == key || !item.is_object() || !item.contains(key)) {
            continue;
        }
        const nlohmann::json &val = item[key];
        if(val.is_string()) {
            std::string _val = val.get<std::string>();
            if(stringMap.find(_val) != stringMap.end()) {
                continue;
            }
            stringMap.insert(pair<string, string>(_val, "tmp"));
            rst.push_back(val);
        } else if(val.is_number()) {
            double _val = val;
            std::string _key = std::to_string(_val);
            if(stringMap.find(_key) != stringMap.end()) {
                continue;
            }
            stringMap.insert(pair<string, string>(_key, "tmp"));
            rst.push_back(val);
        } else {
            // val is array
        }
    }
    return rst;
}

static std::array<double, 2> JsonArrayRange(nlohmann::json &data) {
    if(!data.is_array() || data.size() <= 0) {
        return std::array<double, 2>{0, 0};
    }
    double _min = std::numeric_limits<double>::max();
    double _max = std::numeric_limits<double>::min();

    bool checked = false;
    for(size_t i = 0; i < data.size(); ++i) {
        auto &item = data[i];
        if(item.is_number()) {
            double t = item;
            _min = fmin(_min, t);
            _max = fmax(_max, t);
            checked = true;
        }
    }
    if(!checked) {
        _min = 0;
        _max = 0;
    }
    return std::array<double, 2>{_min, _max};
}

static std::string GenerateRowUniqueKey(nlohmann::json &row, std::set<std::string> &fields) {

    std::string unique = "_";

    for(auto it = fields.begin(); it != fields.end(); ++it) {
        std::string field = *it;
        if(row.contains(field)) {
            unique += row[field].dump();
        }
    }
    return unique;
}

static nlohmann::json JsonGroupByFields(const nlohmann::json &data, std::set<std::string> fields) {
    if(fields.empty()) {
        nlohmann::json rst;
        rst.push_back(data);
        return rst;
    }

    nlohmann::json group;
    std::set<std::string> rowKeys;

    size_t size = data.size();
    for(size_t index = 0; index < size; ++index) {
        nlohmann::json row = data[index];

        std::string key = GenerateRowUniqueKey(row, fields);
        if(group.contains(key)) {
            group[key].push_back(row);
        } else {
            nlohmann::json array;
            array.push_back(row);
            group[key] = array;
        }
        rowKeys.insert(key);
    }

    nlohmann::json rst;
    if(group.is_object() && group.size() > 0) {
        for(auto it = rowKeys.begin(); it != rowKeys.end(); ++it) {
            rst.push_back(group[*it]);
        }
    } else {
        rst.push_back(data);
    }
    return rst;
}

static nlohmann::json JsonArraySlice(nlohmann::json &source, int start, int end) {
    nlohmann::json rst;
    for(int i = start; i <= end; i++) {
        if(i < 0 || i >= source.size()) {
            continue;
        }

        rst.push_back(source[i]);
    }

    return rst;
}

} // namespace util
} // namespace xg

#endif // XG_GRAPHICS_UTIL_JSON_H
