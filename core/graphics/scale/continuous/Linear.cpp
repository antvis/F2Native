//
//  Linear.cpp
//  F2
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#include "Linear.h"
#include "../../XChart.h"

using namespace xg::scale;

std::string Linear::GetTickText(const nlohmann::json &item, XChart *chart) {
    if(!config.tick.empty() && chart) {
        nlohmann::json content{{"content", item.dump()}};
        auto rst = xg::json::ParseString((chart->InvokeFunction(config.tick, content.dump())));
        if(rst.is_object() && rst.contains("content")) {
            return rst["content"];
        }
    }
    
    // 处理 TickText 数值精度
    if(item.is_string()) {
        return StringUtil::ToFixed(stod(item.get<std::string>()), config.precision);
    } else if(item.is_number_integer()) {
        return std::to_string(item.get<int>());
    } else if(item.is_number_float()) {
        return StringUtil::ToFixed(item.get<float>(), config.precision);
    } else {
        return ""; // TODO get Tick text from callback
    }
}
