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

std::string Linear::GetTickText(const Any &item, XChart *chart) {
    if(!config.tick.empty() && chart) {
        std::string content = "{\"content\"," +  item.Cast<string>() + "}";
        auto rst = chart->InvokeFunction(config.tick, content);
        if(rst.count("content")) {
            return rst["content"].Cast<string>();
        }
    }
    
    // 处理 TickText 数值精度
    if(item.GetType().IsString()) {
        return StringUtil::ToFixed(stod(item.Cast<std::string>()), config.precision);
    } else if(item.GetType().IsFloatingNumber()) {
        return StringUtil::ToFixed(item.Cast<float>(), config.precision);
    } else if(item.GetType().IsNumber()) {
        return std::to_string(item.Cast<int>());
    } else {
        return ""; // TODO get Tick text from callback
    }
}
