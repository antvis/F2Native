//
//  XChart.cpp
//  F2Tests
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#include "../../core/graphics/XChart.h"

using namespace xg;
using namespace std;

namespace unit {

class XChart {
public:
    //解析各种空数据的情况
    static bool ParseConfigEmpty() {
        xg::XChart chart("test", 100, 100);
        bool ret1 = chart.Parse("") == false;
        bool ret2 = chart.Parse("{}") == false;
        bool ret3 = chart.Parse("[]") == false;
        bool ret4 = chart.Parse("aaa") == false;
        bool ret5 = chart.Parse("{\"data\":[]}") == false;
        bool ret6 = chart.Parse("{\"source\":[]}") == false;
        bool ret7 = chart.Parse("{\"source\":\"{}}") == false;
        bool ret8 = chart.Parse("{\"source\":\"[\"]}") == false;
        bool ret9 = chart.Render() == false;
        return ret1 && ret2 && ret3 && ret4 && ret5 && ret6 && ret7 && ret8 && ret9;
    }
    
    static bool ParseConfigNoGeoms() {
        xg::XChart chart("test", 100, 100);
        
        //无geoms
        bool ret1 = chart.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10]}") == false;
        bool ret2 = chart.Render() == false;
        
        //geoms为空
        bool ret3 = chart.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"geoms\":[]}") == false;
        bool ret4 = chart.Render() == false;
        
        //geoms类型错误, 类型为object
        bool ret5 = chart.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"geoms\":{}}") == false;
        bool ret6 = chart.Render() == false;
        return ret1 && ret2 && ret3 && ret4 && ret5 && ret6;        
    }
    
    //geoms数组中属性错误
    static bool ParseConfigGeomObjectError() {
        xg::XChart chart("test", 100, 100);
        
        //position不存在
        bool ret1 = chart.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"geoms\":[{\"type\":\"line\"}]}") == true;
        
        //geom type类型不存在
        bool ret2 = chart.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"geoms\":[{\"type\":\"ccc\"}]}") == true;
        
        //geom type类型不存在 position存在
        bool ret3 = chart.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"geoms\":[{\"type\":\"error\",\"position\":\"date*value\"}]}") == true;
        return ret1 && ret2 && ret3;
    }
    
    //geoms各种type覆盖
    static bool ParseConfigGeomObject() {
        
        //5种类型的geom
        xg::XChart chart1("test", 100, 100);
        bool ret1 = chart1.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"geoms\":[{\"type\":\"line\",\"position\":\"date*value\",\"color\":{\"field\":\"value\",\"attrs\":[\"#ff0000\"]},\"size\":{\"field\":\"value\",\"attrs\":[1,2]},\"shape\":{\"field\":\"value\",\"attrs\":[\"smooth\"]}},{\"type\":\"candle\",\"position\":\"date*value\"},{\"type\":\"area\",\"position\":\"date*value\"},{\"type\":\"interval\",\"position\":\"date*value\"},{\"type\":\"point\",\"position\":\"date*value\"}]}") == true;
        ret1 &= chart1.GetGeoms().size() == 5;
        
        xg::XChart chart2("test", 100, 100);
        bool ret2 = chart2.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"geoms\":[{\"type\":\"line\",\"position\":\"date*value\",\"color\":{\"field\":\"value\",\"attrs\":{}},\"size\":{\"field\":\"value\",\"attrs\":{}},\"shape\":{\"field\":\"value\",\"attrs\":{}}},{\"type\":\"candle\",\"position\":\"date*value\"},{\"type\":\"area\",\"position\":\"date*value\"},{\"type\":\"interval\",\"position\":\"date*value\"},{\"type\":\"point\",\"position\":\"date*value\"}]}") == true;
        ret2 &= chart2.GetGeoms().size() == 5;
        
        //interval中设置错误的adjust, style, attrs
        xg::XChart chart3("test", 100, 100);
        bool ret3 = chart3.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"geoms\":[{\"type\":\"line\",\"position\":\"date*value\",\"color\":{\"field\":\"value\",\"attrs\":{}},\"size\":{\"field\":\"value\",\"attrs\":{}},\"shape\":{\"field\":\"value\",\"attrs\":{}}},{\"type\":\"candle\",\"position\":\"date*value\"},{\"type\":\"area\",\"position\":\"date*value\"},{\"type\":\"interval\",\"position\":\"date*value\",\"adjust\":\"a\",\"style\":{},\"attrs\":{},\"fixedShape\":\"a\"},{\"type\":\"point\",\"position\":\"date*value\"}]}") == true;
        
        return ret1 && ret2 && ret3;
    }
    
    //guides的各种异常情况
    static bool ParseConfigGuideObjectError() {
        xg::XChart chart1("test", 100, 100);
        //guides类型不正确，应该为[],实际为{}
        auto ret1 = chart1.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"guides\":{},\"geoms\":[{\"type\":\"line\",\"position\":\"date*value\"}]}") == true;
        
        xg::XChart chart2("test", 100, 100);
        //guides数组为0
        auto ret2 = chart2.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"guides\":[],\"geoms\":[{\"type\":\"line\",\"position\":\"date*value\"}]}") == true;
        return ret1 && ret2;
    }
    
    //guide各种type覆盖
    static bool ParseConfigGuideObject() {
        xg::XChart chart1("test", 100, 100);
        auto ret1 = chart1.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"guides\":[{},{\"type\":\"text\"},{\"type\":\"flag\"},{\"type\":\"line\"},{\"type\":\"background\"},{\"type\":\"image\"}],\"geoms\":[{\"type\":\"line\",\"position\":\"date*value\"}]}") == true;
        return ret1;
    }
    
    //interactions的各种异常情况
    static bool ParseConfigInteractionObjectError() {
        xg::XChart chart1("test", 100, 100);
        //interactions类型不正确，应该为[],实际为{}
        auto ret1 = chart1.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"interactions\":{},\"guides\":[{},{\"type\":\"text\"},{\"type\":\"flag\"},{\"type\":\"line\"},{\"type\":\"background\"},{\"type\":\"image\"}],\"geoms\":[{\"type\":\"line\",\"position\":\"date*value\"}]}") == true;
        
        xg::XChart chart2("test", 100, 100);
        //guides数组为0
        auto ret2 = chart2.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"interactions\":[],\"guides\":[{},{\"type\":\"text\"},{\"type\":\"flag\"},{\"type\":\"line\"},{\"type\":\"background\"},{\"type\":\"image\"}],\"geoms\":[{\"type\":\"line\",\"position\":\"date*value\"}]}") == true;
        return ret1 && ret2;
    }
    
    //interactions各种type覆盖
    static bool ParseConfigInteractionObject() {
        xg::XChart chart1("test", 100, 100);
        auto ret1 = chart1.Parse("{\"source\":[{\"date\":\"2017-06-05\",\"value\":141},{\"date\":\"2017-06-06\",\"value\":129}],\"margin\":[10,10,10,10],\"padding\":[10,10,10,10],\"interactions\":[{\"type\":\"pan\"},{\"type\":\"pinch\"}],\"guides\":[{},{\"type\":\"text\"},{\"type\":\"flag\"},{\"type\":\"line\"},{\"type\":\"background\"},{\"type\":\"image\"}],\"geoms\":[{\"type\":\"line\",\"position\":\"date*value\"}]}") == true;
        return ret1;
    }
};
}
