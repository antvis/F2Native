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
#include <unordered_map>
#include "../../reflection/reflection.h"

using namespace std;

namespace xg {
namespace json {

Any FromMapByType(const unordered_map<string, Any> &data, const Type* type);
Any FromMapByObj(const unordered_map<string, Any> &data, const Any &obj);

//template <class T> Any FromMap(const unordered_map<string, Any> &data) {
//    return FromMap(data, typeof(T));
//}

//template <class T> Any FromMap(const unordered_map<string, Any> &data, Any Obj) {
//    return FromMap(data, Obj);
//}

std::vector<float> ScaleDash(const std::vector<float> &dash, float scale);
std::vector<float> ScaleRoundings(const std::vector<float> roundings, float scale);

} // namespace json
} // namespace xg

#endif /* XG_GRAPHICS_JSON_H */
