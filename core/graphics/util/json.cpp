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

nlohmann::json ParseString(const std::string &json) { return nlohmann::json::parse(json, nullptr, false); }

} // namespace json
} // namespace xg
