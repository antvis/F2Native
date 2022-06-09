//
//  Scale.cpp
//  F2
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#include "Scale.h"
#include "../XChart.h"

using namespace xg::scale;

std::string AbstractScale::GetTickText(const nlohmann::json &item, XChart *chart) {
    if(!this->tickCallbackId.empty() && chart) {
        nlohmann::json content{{"content", item}};
        auto rst = xg::json::ParseString((chart->InvokeFunction(this->tickCallbackId, content.dump())));
        if(rst.is_object() && rst.contains("content")) {
            return rst["content"];
        }
    }
    
    if(item.is_string()) {
        return item.get<std::string>();
    } else if(item.is_number()) {
        return std::to_string(item.get<int>());
    } else {
        return ""; // TODO get Tick text from callback
    }
}
