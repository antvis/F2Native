//
//  Linear.cpp
//  F2
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#include "Linear.h"
#include "graphics/XChart.h"

using namespace xg::scale;

std::string Linear::GetTickText(const nlohmann::json &item, XChart *chart) {
    if(!this->tickCallbackId.empty() && chart) {
        nlohmann::json content{{"content", item.dump()}};
        auto rst = xg::json::ParseString((chart->InvokeFunction(this->tickCallbackId, content.dump())));
        if(rst.is_object() && rst.contains("content")) {
            return rst["content"];
        }
    }
    
    // 处理 TickText 数值精度
    if(item.is_string()) {
        return item.get<std::string>();
    } else if(item.is_number_integer()) {
        return std::to_string(item.get<int>());
    } else if(item.is_number_float()) {
        float val = item.get<float>();
        if(fabs(val) < XG_EPS) {
            return "0";
        }
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << val;
        return ss.str();
    } else {
        return ""; // TODO get Tick text from callback
    }
}
