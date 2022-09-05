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

bool xg::scale::IsCategory(ScaleType type) {
    return type == ScaleType::Cat || type == ScaleType::TimeCat || type == ScaleType::Kline;
}

bool xg::scale::IsLinear(ScaleType type) { return type == ScaleType::Linear || type == ScaleType::TimeSharingLinear; }

std::string AbstractScale::GetTickText(const Any &item, XChart *chart) {
    printf("GetTickText start \n");
    if(!config.tick.empty() && chart) {
        printf("GetTickText start 0\n");
        std::string content = "{\"content\"," +  item.Cast<string>() + "}";
        auto rst = chart->InvokeFunction(config.tick, content);
        if(rst.count("content")) {
            return rst["content"].Cast<string>();
        }
    }
    
    printf("GetTickText start 1\n");
    if(item.GetType().IsString()) {
        printf("GetTickText start 2\n");
        return item.Cast<std::string>();
    } else if(item.GetType().IsNumber()) {
        return std::to_string(item.Cast<int>());
    } else {
        return ""; // TODO get Tick text from callback
    }
}
