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

std::string AbstractScale::GetTickText(const nlohmann::json &item, XChart *chart) {
    if(!config.tick.empty() && chart) {
        nlohmann::json content{{"content", item}};
        auto rst = xg::json::ParseString((chart->InvokeFunction(config.tick, content.dump())));
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

//void AbstractScale::InitConfig(const nlohmann::json &cfg) {
//    if (!cfg.is_object()) {
//        return;
//    }
//    tickCount = json::GetIntNumber(cfg, "tickCount", int(tickCount));
//    tickCount = fmax(2, tickCount);
//
//    containMin = cfg.contains("min");
//    containMax = cfg.contains("max");
//    min = json::GetNumber(cfg, "min", min);
//    max = json::GetNumber(cfg, "max", max);
//    auto &range = json::GetArray(cfg, "range");
//    if (range.size() >= 2) {
//        rangeMin = range[0];
//        rangeMax = range[1];
//        containRange = true;
//    }
//    
//    tickCallbackId = xg::json::GetString(cfg, "tick");
//    
//    ticks = json::GetArray(cfg, "ticks");
//    containTicks = !ticks.empty();
//}
