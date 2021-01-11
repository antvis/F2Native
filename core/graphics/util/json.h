//
//  json.h
//  AFWFinanceChart
//
//  Created by XiaLuo on 2020/12/14.
//  Copyright © 2020 Alipay. All rights reserved.
//

#include <nlohmann/json.hpp>

#ifndef XG_GRAPHICS_JSON_H
#define XG_GRAPHICS_JSON_H

namespace xg {
namespace json {

nlohmann::json ParseString(const std::string &json);


} // namespace json
} // namespace xg


#endif /* XG_GRAPHICS_JSON_H */
