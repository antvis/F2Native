//
//  ToolTip.cpp
//  F2
//
//  Created by weiqing.twq on 7/25/22.
//

#include "ToolTip.h"

using namespace xg;

void tooltip::from_json(const nlohmann::json &j, CrosshairsStyle &c) {
    CrosshairsStyle d;
    if (j.is_boolean()) {
        c.hidden = true;
    } else if(j.is_object()) {
        c.yCrosshair = j.value("yCrosshair", d.yCrosshair);
        c.stroke = j.value("stroke", d.stroke);
        c.lineWidth = j.value("lineWidth", d.lineWidth);
        c.type = j.value("type", d.type);
        c.dash = j.value("type", d.dash);
        c.hidden = j.value("hidden", d.hidden);
    }else {
        F2ASSERT(false, "CrosshairsStyle from_json error json type");
    }
}

void tooltip::from_json(const nlohmann::json &j, BackgroundCfg &b) {
    BackgroundCfg d;
    b.radius = j.value("radius", d.radius);
    b.fill = j.value("fill", d.fill);
    b.padding = j.value("padding", d.padding);
}

void tooltip::from_json(const nlohmann::json &j, Tip &t) {
    Tip d;
    if (j.is_boolean()) {
        t.hidden = true;
    } else if(j.is_object()) {
        t.inner = j.value("inner", d.inner);
        t.fontSize = j.value("fontSize", d.fontSize);
        t.fill = j.value("fill", d.fill);
        t.textAlign = j.value("textAlign", d.textAlign);
        t.textBaseline = j.value("textBaseline", d.textBaseline);
        t.hidden = j.value("hidden", d.hidden);
    } else {
        F2ASSERT(false, "Tip from_json error json type");
    }
}

void tooltip::to_json(nlohmann::json &j, const Tip &t) {
    j = nlohmann::json{{"inner", t.inner}, {"fontSize", t.fontSize}, {"fill", t.fill}, {"textAlign", t.textAlign}, {"textBaseline", t.textBaseline}};
}

void tooltip::from_json(const nlohmann::json &j, ToolTipCfg &t) {
    ToolTipCfg d;
    if (j.is_boolean()) {
        t.hidden = true;
    }else if(j.is_object()){
        t.onPressStart = j.value("onPressStart", d.onPressStart);
        t.onPress = j.value("onPress", d.onPress);
        t.onPressEnd = j.value("onPressEnd", d.onPressEnd);
        t.crosshairsStyle = j.value("crosshairsStyle", d.crosshairsStyle);
        t.background = j.value("background", d.background);
        t.xTip = j.value("xTip", d.xTip);
        t.yTip = j.value("yTip", d.yTip);
    }else {
        F2ASSERT(false, "ToolTipCfg from_json error json type");
    }
}

void tooltip::from_json(const nlohmann::json &j, ToolTipItem &t) {
    ToolTipItem d;
    t.x = j.value("x", d.x);
    t.y = j.value("y", d.y);
    t.color = j.value("color", d.color);
    t.name = j.value("name", d.name);
    t.touchX = j.value("touchX", d.touchX);
    t.touchY = j.value("touchY", d.touchY);
    t.title = j.value("title", d.title);
    t.xTip = j.value("xTip", d.xTip);
    t.yTip = j.value("yTip", d.yTip);
}

void tooltip::to_json(nlohmann::json &j, const ToolTipItem &t) {
    j = nlohmann::json{{"x", t.x}, {"y", t.y}, {"color", t.color}, {"name", t.name}, {"touchX", t.touchX}, {"touchY", t.touchY}, {"title", t.title}, {"xTip", t.xTip}, {"yTip", t.yTip}};
}

void tooltip::from_json(const nlohmann::json &j, ToolTipItemList &t) {
    ToolTipItemList d;
    t.items = j.value("tooltip", d.items);
}

void tooltip::to_json(nlohmann::json &j, const ToolTipItemList &t) {
    j = nlohmann::json{{"tooltip", t.items}};
}
