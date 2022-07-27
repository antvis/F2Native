#ifndef XG_GRAPHICS_UTIL_JSON_H
#define XG_GRAPHICS_UTIL_JSON_H

#include <algorithm>
#include <array>
#include <limits>
#include <math.h>
#include <set>
#include <string>
#include <unordered_map>
#include "json_data.h"
#include "../../nlohmann/json.hpp"

using namespace std;

namespace xg {
namespace util {

// 从原始数组中查找出所有指定的 key 字段值组成新的数组返回
static nlohmann::json JsonArrayByKey(const nlohmann::json &data, const std::string &key) {
    if(!data.is_array() || data.size() <= 0) {
        return nlohmann::json();
    }

    nlohmann::json rst;
    std::set<std::size_t> _keysFilters;

    for(std::size_t i = 0; i < data.size(); ++i) {
        auto &item = data[i];
        if(item == key || !item.is_object() || !item.contains(key)) {
            continue;
        }
        const nlohmann::json &val = item[key];
        if(val.is_string() || val.is_number()) {
            std::size_t valHash = nlohmann::detail::hash(val);
            if(_keysFilters.find(valHash) != _keysFilters.end()) {
                continue;
            }
            _keysFilters.emplace(valHash);
            rst.push_back(val);
        } else if(val.is_array()) {
            for(size_t i = 0; i < val.size(); i++) {
                const nlohmann::json &item = val[i];
                std::size_t itemHash = nlohmann::detail::hash(item);
                if(_keysFilters.find(itemHash) != _keysFilters.end()) {
                    continue;
                }
                _keysFilters.emplace(itemHash);

                if(item.is_number() || item.is_string()) {
                    rst.push_back(item);
                }
            }
        }
    }
    return rst;
}

static std::array<double, 2> JsonArrayRange(const nlohmann::json &data) {
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
        } else if(item.is_array()) {
            for(std::size_t index = 0; index < item.size(); ++index) {
                auto &subItem = item[index];
                if(subItem.is_number()) {
                    double t = subItem;
                    _min = fmin(_min, t);
                    _max = fmax(_max, t);
                    checked = true;
                }
            }
        }
    }

    if(!checked) {
        _min = 0;
        _max = 0;
    }
    return std::array<double, 2>{_min, _max};
}

static std::string GenerateRowUniqueKey(const nlohmann::json &row, const std::set<std::string> &fields) {

    std::string unique = "_";

    for(auto it = fields.begin(); it != fields.end(); ++it) {
        std::string field = *it;
        if(row.contains(field)) {
            // unique += row[field].dump();
            unique += std::to_string(nlohmann::detail::hash(row[field]));
        }
    }
    return unique;
}

static XDataGroup JsonGroupByFields(const nlohmann::json &data, const std::set<std::string> &fields) {
    XDataGroup rst;
    std::map<std::string, std::vector<XData>> group;
    std::set<std::string> rowKeys;
    std::vector<std::string> rowKeysOrder;

    size_t size = data.size();
    for(size_t index = 0; index < size; ++index) {
        const nlohmann::json &row = data[index];

        std::string key = GenerateRowUniqueKey(row, fields);
        if(group.count(key)) {
            group[key].push_back({&row});
        } else {
            std::vector<XData> array;
            array.push_back({&row});
            group[key] = std::move(array);
        }

        if(std::find(rowKeys.begin(), rowKeys.end(), key) == rowKeys.end()) {
            rowKeysOrder.emplace_back(key);
        }
        rowKeys.insert(key);
    }

    for(auto it = rowKeysOrder.begin(); it != rowKeysOrder.end(); ++it) {
        rst.push_back(group[*it]);
    }
    return rst;
}

static nlohmann::json JsonArraySlice(const nlohmann::json &source, std::size_t start, std::size_t end) {
    nlohmann::json rst;
    if(start > end || end >= source.size()) {
        return rst;
    }

    for(std::size_t i = start; i <= end; i++) {
        rst.push_back(source[i]);
    }

    return rst;
}

static bool isEqualsQuick(nlohmann::json &data1, nlohmann::json &data2) {
    if(data1.type() != data2.type()) {
        return false;
    }

    if(data1.size() != data2.size()) {
        return false;
    }

    if(data1.empty() == 0)
        return true;

    std::size_t lastIndex = fmin(data1.size(), data2.size()) - 1;
    return (data1[0] == data2[0] && data1[lastIndex] == data2[lastIndex]);
}

static void JsonRangeInGeomDataArray(const XDataGroup &geomDataArray,
                                     const std::string &field,
                                     std::size_t start,
                                     std::size_t end,
                                     double *rangeMin,
                                     double *rangeMax) {
    if(geomDataArray.size() > 0) {
        for(std::size_t index = 0; index < geomDataArray.size(); ++index) {
            auto &groupData = geomDataArray[index];

            std::size_t _end = fmin(end, groupData.size() - 1);
            if(_end > start) {
                for(std::size_t column = start; column <= _end; ++column) {
                    auto &item = (*groupData[column].data);
                    if(item.contains(field)) {
                        if(item[field].is_number()) {
                            double val = item[field];
                            (*rangeMin) = fmin(val, (*rangeMin));
                            (*rangeMax) = fmax(val, (*rangeMax));
                        } else if(item[field].is_array()) {
                            const nlohmann::json &arr = item[field];
                            for(std::size_t i = 0; i < arr.size(); ++i) {
                                if(arr[i].is_number()) {
                                    double val = arr[i];
                                    (*rangeMin) = fmin(val, (*rangeMin));
                                    (*rangeMax) = fmax(val, (*rangeMax));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

static double JsonArrayMax(const nlohmann::json &dataArray) {
    std::vector<double> array = dataArray;
    auto it = std::max_element(array.begin(), array.end());
    return array[std::distance(array.begin(), it)];
}

} // namespace util
} // namespace xg

#endif // XG_GRAPHICS_UTIL_JSON_H
