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

using namespace std;

namespace xg {
namespace util {

// 从原始数组中查找出所有指定的 key 字段值组成新的数组返回
static vector<const Any *> JsonArrayByKey(const XSourceArray &data, const std::string &key) {
    if(data.empty()) {
        return {};
    }

    vector<const Any *> rst;
    std::set<std::size_t> _keysFilters;

    for(std::size_t i = 0; i < data.size(); ++i) {
        const XSourceItem &item = data[i];
        if(!item.count(key)) {
            continue;
        }
        const Any &val = item.at(key);
        if(val.GetType().IsString() || val.GetType().IsNumber()) {
            std::size_t valHash = val.hash();
            if(_keysFilters.find(valHash) != _keysFilters.end()) {
                continue;
            }
            _keysFilters.emplace(valHash);
            rst.push_back(&val);
        } else if(val.GetType().IsArray()) {
            auto valArray = val.Cast<std::vector<Any>>();
            for(size_t i = 0; i < valArray.size(); i++) {
                auto &item = valArray[i];
                std::size_t itemHash = item.hash();
                if(_keysFilters.find(itemHash) != _keysFilters.end()) {
                    continue;
                }
                _keysFilters.emplace(itemHash);

                if(item.GetType().IsNumber() || item.GetType().IsString()) {
                    rst.push_back(&item);
                }
            }
        }
    }
    return rst;
}

static std::array<double, 2> JsonArrayRange(const vector<const Any *> &data) {
    if(data.empty()) {
        return std::array<double, 2>{0, 0};
    }
    double _min = std::numeric_limits<double>::max();
    double _max = std::numeric_limits<double>::min();

    bool checked = false;
    for(size_t i = 0; i < data.size(); ++i) {
        auto item = data[i];
        if(item->GetType().IsNumber()) {
            double t = item->Cast<double>();
            _min = fmin(_min, t);
            _max = fmax(_max, t);
            checked = true;
        } else if(item->GetType().IsArray()) {
            auto &array = item->Cast<vector<Any> &>();
            for(std::size_t index = 0; index < array.size(); ++index) {
                auto subItem = array[index];
                if(subItem.IsNumber()) {
                    double t = subItem.Cast<double>();
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

static std::string GenerateRowUniqueKey(const XSourceItem &row, const std::set<std::string> &fields) {

    std::string unique = "_";

    for(auto it = fields.begin(); it != fields.end(); ++it) {
        const std::string &field = *it;
        if(row.count(field)) {
            // unique += row[field].dump();
            unique += std::to_string(row.at(field).hash());
        }
    }
    return unique;
}

static XDataGroup JsonGroupByFields(const XSourceArray &data, const std::set<std::string> &fields) {
    XDataGroup rst;
    std::map<std::string, std::vector<XData>> group;
    std::set<std::string> rowKeys;
    std::vector<std::string> rowKeysOrder;

    size_t size = data.size();
    for(size_t index = 0; index < size; ++index) {
        auto &row = data[index];

        std::string key = GenerateRowUniqueKey(row, fields);
        if(group.count(key)) {
            group[key].push_back({.data = &row});
        } else {
            std::vector<XData> array;
            array.push_back({.data = &row});
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

static vector<const Any *> JsonArraySlice(const vector<const Any *> &source, std::size_t start, std::size_t end) {
    vector<const Any *> rst;
    if(start > end || end >= source.size()) {
        return rst;
    }
    
    for(std::size_t i = start; i <= end; i++) {
        rst.push_back(source[i]);
    }

    return rst;
}

static bool isEqualsQuick(const vector<const Any *> &data1, const vector<const Any *> &data2) {
    if(data1.size() != data2.size()) {
        return false;
    }

    if(data1.empty() == 0)
        return true;

    std::size_t lastIndex = fmin(data1.size(), data2.size()) - 1;
    return (data1[0]->IsEqual(data2[0]) && data1[lastIndex]->IsEqual(data2[lastIndex]));
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
                    auto &item = groupData[column].data;
                    if(item->count(field)) {
                        auto fieldVal = item->find(field)->second;
                        if(fieldVal.GetType().IsNumber()) {
                            double val = fieldVal.Cast<double>();
                            (*rangeMin) = fmin(val, (*rangeMin));
                            (*rangeMax) = fmax(val, (*rangeMax));
                        } else if(fieldVal.GetType().IsArray()) {
                            auto &arr = fieldVal.Cast<std::vector<double> &>();
                            for(std::size_t i = 0; i < arr.size(); ++i) {
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

static double JsonArrayMax(const vector<const Any *> &dataArray) {
    std::vector<double> array;
    array.reserve(dataArray.size());
    for (auto &val: dataArray) {
        array.push_back(val->Cast<double>());
    }
    
    auto it = std::max_element(array.begin(), array.end());
    return array[std::distance(array.begin(), it)];
}

} // namespace util
} // namespace xg

#endif // XG_GRAPHICS_UTIL_JSON_H
