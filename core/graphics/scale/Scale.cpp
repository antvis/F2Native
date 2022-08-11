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

void xg::scale::from_json(const nlohmann::json &j, ScaleCfg &s) {
    if (!j.is_object()) { return; }
    ScaleCfg d;
    s.type = j.value("type", d.type);
    s.max = j.value("max", d.max);
    s.min = j.value("min", d.min);
    s.range = j.value("range", d.range);
    s.tick = j.value("tick", d.tick);
    s.ticks = j.value("ticks", d.ticks);
    s.nice = j.value("nice", d.nice);
    s.tickCount = j.value("tickCount", d.tickCount);
    s.precision = j.value("precision", d.precision);
    s.domain = j.value("domain", d.domain);
    s.timeRange = j.value("timeRange", d.timeRange);
    s.dateFormate = j.value("dateFormate", d.dateFormate);
}

std::string AbstractScale::GetTickText(const Any &item, XChart *chart) {
    if(!config.tick.empty() && chart) {
        nlohmann::json content{{"content", item.Cast<string>()}};
        auto rst = xg::json::ParseString((chart->InvokeFunction(config.tick, content.dump())));
        if(rst.is_object() && rst.contains("content")) {
            return rst["content"];
        }
    }
    
    if(item.GetType().IsString()) {
        return item.Cast<std::string>();
    } else if(item.GetType().IsNumber()) {
        return std::to_string(item.Cast<int>());
    } else {
        return ""; // TODO get Tick text from callback
    }
}
