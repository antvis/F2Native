//
//  XCubeBridgeWidget.c
//  AFWFinanceChart
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 Alipay. All rights reserved.
//

#include "ChartBridge.h"

using namespace xg;
using namespace xg::bridge;

ChartBridge::ChartBridge(double width, double height, double ratio) {
    chart_ = xg::make_unique<XChart>("", width, height, ratio);
}

bool ChartBridge::InvokeMethod(const std::string &methodName,
                               const std::string &params,
                               BridgeCallback callback) {
    if (!callback) {
        //没有callback 打印一个日志
        chart_->GetLogTracer()->trace("callback is nullptr");
        return false;
    }

    if (!railing_) {
        CallbackError(methodName, callback, "railing is nullptr");
        return false;
    }

    commonFunction_ = BridgeChannel::Callback([callback, this](const std::string &functionId,
                                                               const std::string &params) -> const std::string {
        //回调渲染动画
        if (functionId == chart_->GetRequestFrameFuncId()) {
            railing_->PlayAnimation(params);
            return params;
        }
        //内部回调方法
        else if (callbackFunctions_.find(functionId) != callbackFunctions_.end()) {
            auto &function = callbackFunctions_[functionId];
            return function.Execute(functionId, params);
        }
        //直接通过bridge回调出去
        else {
            nlohmann::json ret = {{"method","render"}, {"renderCallback", {{"functionId", functionId}, {"param", params}}}};
            callback(ret.dump());
            return params;
        }
    });
    chart_->SetCanvasContext(railing_->GetCanvasContext());

    if (methodName == "render") {
        auto chartConfig = json::ParseString(params);
        if (!chartConfig.is_object()) {
            chart_->GetLogTracer()->trace("dsl is not object %s " , params.c_str());
            CallbackError(methodName, callback, "dsl is not object");
            return false;
        }

        if (!chartConfig.contains("chartConfig")) {
            chart_->GetLogTracer()->trace("dsl is not contains chartConfig %s " , params.c_str());
            CallbackError(methodName, callback, "dsl is not contains chartConfig key");
            return false;
        }

        auto position = json::GetObject(chartConfig, "getPosition");
        if (chartConfig["chartConfig"].is_object()) {
            return InvokeRender(json::GetObject(chartConfig, "chartConfig"),
                                position,
                                callback);
        } else {
            const auto &dsl = json::ParseString(json::GetString(chartConfig, "chartConfig"));
            return InvokeRender(dsl, position, callback);
        }
    } else if (methodName == "destroy") {
        //do nothing
    } else if (methodName == "clear") {
        chart_->Clear();
    } else if (methodName == "getPosition") {
        auto config = json::ParseString(params);
        const auto &itemData = json::GetObject(config, "itemData");
        callback(std::move(GetPosition(itemData)));
    } else if (methodName == "getPositions") {
        auto config = json::ParseString(params);
        const auto &itemDatas = json::GetArray(config, "itemDatas");
        const auto &positions = GetPositions(itemDatas);
        callback(positions);
    } else if (methodName == "getScaleTicks") {
        auto config = json::ParseString(params);
        const auto &field = json::GetString(config, "field");
        const auto &ticks = chart_->GetScaleTicks(field);
        callback(nlohmann::json({{"method","getScaleTicks"}, {"ticks",xg::json::ParseString(ticks)}}).dump());
    } else {
        CallbackError(methodName, callback, "unknow method name");
        return false;
    }
    return true;
}

std::string ChartBridge::GetPositions(const nlohmann::json &itemDatas) {
    nlohmann::json array = {};
    if (chart_ == nullptr || itemDatas.is_null() || !itemDatas.is_array() || itemDatas.empty()) {
        return array.dump();
    }
    auto ratio = chart_->GetCanvasContext().GetDevicePixelRatio();
    if (ratio <= 0) {
        ratio = 1;
    }

    for (auto it = itemDatas.begin(); it != itemDatas.end(); ++it) {
        const auto &position = chart_->GetPosition(*it);
        array.push_back({position.x / ratio / pixelRatio_,
                         position.y / ratio / pixelRatio_});
    }
    return nlohmann::json({{"method","getPositions"}, {"positions",array}}).dump();
}

nlohmann::json ChartBridge::GetPosition(const nlohmann::json &itemData) {
    if (chart_ == nullptr || itemData.is_null()) {
        return {};
    }
    const auto &position = chart_->GetPosition(itemData);
    auto ratio = chart_->GetCanvasContext().GetDevicePixelRatio();
    return nlohmann::json({{"method","getPosition"}, {"position", {position.x / ratio / pixelRatio_,
                                                                    position.y / ratio /
                                                                    pixelRatio_}}}).dump();
}

bool ChartBridge::InvokeRender(const nlohmann::json &dsl, const nlohmann::json &itemPosition,
                               BridgeCallback callback) {
    chart_->Clear();
    chart_->SetRequestFrameFuncId(requestFrameFuncId_);
    chart_->SetInvokeFunction(&commonFunction_);
    if (!chart_->ParseObject(dsl)) {
        CallbackError("render", callback, "parse dsl error");
        return false;
    }

    //业务需要的额外字段解析，会覆盖之前的设置
    CompatScales(dsl);
    CompatToolTip(callback);

    //渲染
    if (chart_->Render()) {
        railing_->Swap();

        CallbackSuccess("render", callback, GetPosition(itemPosition), chart_->GetRenderCmdCount(),
                        chart_->GetRenderDurationMM());
        return true;
    } else {
        CallbackError("render", callback, "render error");
        return false;
    }
}

void ChartBridge::CompatToolTip(BridgeCallback callback) {
    if (!needToolTip_) {
        return;
    }

    nlohmann::json tooltip;
    auto onPress = BridgeChannel::Callback([callback, this](const std::string &functionId, const std::string &params)-> const std::string {
        auto config = json::ParseString(params);
        auto &tooltip = json::GetArray(config, "tooltip");
        auto &tooltips = json::GetArray(config, "tooltips");
        callback(nlohmann::json({{"method","render"}, {"tooltip", tooltip.dump()}, {"tooltips", tooltips.dump()}}).dump());
        return params;
    });

    auto onPressEnd = BridgeChannel::Callback([callback, this](const std::string &functionId, const std::string &params)-> const std::string {
        auto config = json::ParseString(params);
        auto &tooltip = json::GetArray(config, "tooltip");
        callback(nlohmann::json({{"method","render"}, {"tooltipEnd", tooltip.dump()}}).dump());
        return params;
    });

    tooltip["onPress"] = onPress.functionId;
    tooltip["onPressEnd"] = onPressEnd.functionId;
    callbackFunctions_[onPress.functionId] = std::move(onPress);
    callbackFunctions_[onPressEnd.functionId] = std::move(onPressEnd);

    chart_->PatchTooltipsObject(tooltip);
}

void ChartBridge::CompatScales(const nlohmann::json &dsl) {
    //解析scales
    const auto &scales = json::GetArray(dsl, "scales");
    for (size_t i = 0; i < scales.size(); ++i) {
        auto &scaleConfig = json::GetObject(scales[i], "config");
        auto &scaleField = json::GetString(scales[i], "field");
        if (scaleConfig.is_object()) {
            CompatScale(scaleField, scaleConfig);
        } else {
            CompatScale(scaleField, scales[i]);
        }
    }
}

void ChartBridge::CompatScale(const std::string &field, const nlohmann::json &sConfig) {
    nlohmann::json scaleConfig(sConfig);


    const auto &tickType = json::GetString(scaleConfig, "tickType");
    const auto &timeZoneName = json::GetString(scaleConfig, "timeZone");
    if (!timeZoneName.empty() && railing_) {
        long timeZoneOffset = railing_->GetTimezoneOffset(timeZoneName);
        scaleConfig["timeZoneOffset"] = timeZoneOffset;
    }

    F2ASSERT(scaleConfig.is_object(), "scaleConfig is not a object");

    if (tickType == "price") {
        int precision = json::GetNumber(scaleConfig, "tickTypePricePrecision", 0);
        auto item = BridgeChannel::Callback([precision, this](const std::string &functionId, const std::string &params)-> const std::string {
            auto config = json::ParseString(params);
            auto &content = json::GetString(config, "content");
            config["content"] = FormatPrice(content, precision);
            return config.dump();
        });
        scaleConfig["tick"] = item.functionId;
        callbackFunctions_[item.functionId] = std::move(item);
        chart_->ScaleObject(field, scaleConfig);
    } else if(tickType == "dateTimestamp") {
        const auto &formatter = json::GetString(scaleConfig, "tickTimestampFormatter");
        const auto &timeZoneName = json::GetString(scaleConfig, "timeZone", "Asia/Shanghai");
        auto item = BridgeChannel::Callback([formatter, timeZoneName, this](const std::string &functionId, const std::string &params)-> const std::string {
            auto config = json::ParseString(params);
            auto &content = json::GetString(config, "content");
            config["content"] = FormatTime(content, timeZoneName, formatter);
            return config.dump();
        });
        scaleConfig["tick"] = item.functionId;
        callbackFunctions_[item.functionId] = std::move(item);
        chart_->ScaleObject(field, scaleConfig);
    } else if(tickType == "percent") {
        double divisor = json::GetNumber(scaleConfig, "tickTypePercentDivisor", 1);
        int precision = json::GetNumber(scaleConfig, "tickTypePercentPrecision", 0);
        auto item = BridgeChannel::Callback([divisor, precision, this](const std::string &functionId, const std::string &params)-> const std::string {
            auto config = json::ParseString(params);
            auto &content = json::GetString(config, "content");
            config["content"] = FormatPercent(content, divisor, precision);
            return config.dump();
        });
        scaleConfig["tick"] = item.functionId;
        callbackFunctions_[item.functionId] = std::move(item);
        chart_->ScaleObject(field, scaleConfig);
    } else {
        if (!timeZoneName.empty()) {
            chart_->ScaleObject(field, scaleConfig);
        }
    }
}

std::string ChartBridge::FormatPrice(const std::string &content, int precision) {
    double value = stod(content);
    std::string unit = "";

    if (abs(value) >= 10000000000000000.0) {
        value = value / 10000000000000000.0;
        unit = "亿亿";
    } else if (abs(value) >= 1000000000000.0) {
        value = value / 1000000000000.0;
        unit = "万亿";
    } else if (abs(value) >= 100000000.0) {
        value = value / 100000000.0;
        unit = "亿";
    } else if (abs(value) >= 10000.0) {
        value = value / 10000.0;
        unit = "万";
    } else {
        //do nothing
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str() + unit;
}

std::string ChartBridge::FormatPercent(const std::string &content, double divisor, int precision) {
    double value = stod(content);
    double percent = value / divisor * 100;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << percent;
    return ss.str() + "%";
}

std::string ChartBridge::FormatTime(const std::string &value, const std::string &timezoneName, const std::string &formatter) {
    return railing_->FormatTime(value, timezoneName, formatter);
}

void
ChartBridge::CallbackSuccess(const std::string &methodName, BridgeCallback callback, const nlohmann::json &itemPosition,
                             long renderCmdCount, long renderDurationMM) {
    nlohmann::json ret = {{"method", methodName},{"renderResult", {
        {"state", true},{"position",itemPosition},{"renderCmdCount", renderCmdCount},{"renderDurationMM", renderDurationMM}}
    }};
    callback(ret.dump());
}

void ChartBridge::CallbackError(const std::string &methodName, BridgeCallback callback, const std::string &msg) {
    nlohmann::json ret = {{"method", methodName}, {"error", msg} , {"renderResult", {{"state", false}}}};
    callback(ret.dump());
}

void ChartBridge::SetEnableListConfig(const std::string &params){
    if (chart_ == nullptr || params.empty()) {
        return;
    }
    chart_->GetLogTracer()->trace("SetEnableListConfig %s ", params.c_str());
    auto enableCfg = json::ParseString(params);
    if (!enableCfg.is_object()) {
        return;
    }
    EnableListConfig enableConfig;
    enableConfig.imageGuideEnable = json::GetBool(enableCfg, "imageGuideEnable");
    enableConfig.legendCenterEnable = json::GetBool(enableCfg, "legendCenterEnable");
    enableConfig.candleFirstRenderFixDisable = json::GetBool(enableCfg, "candleFirstRenderFixDisable", false);
    chart_->SetEnableConfig(enableConfig);
}

void ChartBridge::SetDarkModeInfo(bool isDark, std::map<int, int> &colors) {
    if (chart_ == nullptr) {
        return;
    }
    chart_->GetDarkModeManager().SetDarkModeInfo(isDark,colors);
}
