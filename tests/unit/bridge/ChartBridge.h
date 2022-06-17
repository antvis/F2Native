//
//  ChartBridge.h
//  F2Tests
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//


#include <fstream>
#include <iostream>
#include "../../../core/bridge/ChartBridge.h"
#include "../../../core/ios/BridgeRailingIOS.h"

using namespace xg;
using namespace xg::canvas;
using namespace std;

namespace unit {

class ChartBridge {
public:
    //错误的初始化方法
    static bool Init() {
        //提前初始化下
        auto callback = [](const std::string &json) -> void {
        };
        bridge::BridgeRailingIOS railing(nullptr);
        
        //未设置railing
        bridge::ChartBridge bridge1(100, 100, 1.);
        bool ret1 = bridge1.InvokeMethod("render", "{}", callback) == false;
        
        //未设置callback
        bridge::ChartBridge bridge2(100, 100, 1.);
        bool ret2 = bridge2.InvokeMethod("render", "{}", nullptr) == false;
        
        //param中缺少chartConfig参数
        bridge::ChartBridge bridge3(100, 100, 1.);
        bridge3.SetRailing(&railing);
        bool ret3 = bridge3.InvokeMethod("render", "{}", callback) == false;
        
        //param是非json格式
        bridge::ChartBridge bridge4(100, 100, 1.);
        bridge4.SetRailing(&railing);
        bool ret4 = bridge4.InvokeMethod("render", "", callback) == false;
        
        //method是空
        bridge::ChartBridge bridge5(100, 100, 1.);
        bridge5.SetRailing(&railing);
        bool ret5 = bridge5.InvokeMethod("", "{}", callback) == false;
        return ret1 && ret2 && ret3 && ret4 && ret5;
    }
    
    static bool Render() {
        //提前初始化下
        auto callback = [](const std::string &json) -> void {
        };
        bridge::BridgeRailingIOS railing(nullptr);
        
        //chartConfig中data数据为空
        bridge::ChartBridge bridge1(100, 100, 1.);
        bridge1.SetRailing(&railing);
        bool ret1 = bridge1.InvokeMethod("render", "{\"chartConfig\":{\"data\":\"\"}}", callback) == false;
        
        //chartConfig中data数据为object
        bridge::ChartBridge bridge2(100, 100, 1.);
        bridge2.SetRailing(&railing);
        bool ret2 = bridge2.InvokeMethod("render", "{\"chartConfig\":{\"data\":\"{}\"}}", callback) == false;
        
        //chartConfig中data数据为空数组
        bridge::ChartBridge bridge3(100, 100, 1.);
        bridge3.SetRailing(&railing);
        bool ret3 = bridge3.InvokeMethod("render", "{\"chartConfig\":{\"data\":\"[]\"}}", callback) == false;
        return ret1 && ret2 && ret3;
    }
    
    static bool Render(const std::string &path, void *context, double width, double height, double ratio) {
        //提前初始化下
        auto callback = [](const std::string &json) -> void {
        };
        std::ifstream jsonFile(path);
        std::string json((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());
        
        bridge::BridgeRailingIOS railing(nullptr);
        railing.SetCanvasContext(context);
        bridge::ChartBridge bridge1(width, height, ratio);
        bridge1.SetRailing(&railing);
        return bridge1.InvokeMethod("render", json, callback) ;
    }
    
};
}
