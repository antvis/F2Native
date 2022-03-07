//
//  json.h
//  F2Tests
//
//  Created by weiqing.twq on 2021/11/17.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#include "graphics/util/json.h"

using namespace xg;
using namespace std;

namespace unit {

class Json {
public:
    static bool ParseString() {
        auto json = json::ParseString("{\"name\":\"f2\"}");
        return json["name"] == "f2";
    }
    
    static bool ParseDashArray() {
        //多个dash
        auto dash0 = json::ParseDashArray({5, 6, 7, 8}, 1.0);
        bool ret0 = dash0.size() == 4 && dash0[0] == 5 && dash0[1] == 6 && dash0[2] == 7 && dash0[3] == 8;
        
        
        //有两个dash
        auto dash1 = json::ParseDashArray({5,5}, 2.0);
        bool ret1 = dash1.size() == 2 && dash1[0] == 10 && dash1[1] == 10;
        
        //只配置了一个dash
        auto dash2 = json::ParseDashArray({5}, 1.0);
        bool ret2 = dash2.size() == 1 && dash2[0] == 5;
        
        //没配置dash
        auto dash3 = json::ParseDashArray({}, 1.0);
        bool ret3 = dash3.size() == 2 && dash3[0] == 10 && dash3[1] == 10;
        return ret0 && ret1 && ret2 && ret3;
    }
    
    static bool ParseRoundings() {
        float roundings0[4] = {0, 0, 0, 0};
        json::ParseRoundings({5, 6, 7, 8}, &roundings0[0], 2);
        bool ret0 = roundings0[0] == 10 && roundings0[1] == 12 &&  roundings0[2] == 14 && roundings0[3] == 16;
        
        //roundings 少数据
        float roundings1[1] = {0};
        json::ParseRoundings({5, 6, 7, 8}, &roundings1[0], 2);
        bool ret1 = roundings1[0] == 10;
        return ret0 && ret1;
    }
    
    static bool Get() {
        auto ret1 = json::Get({{"name","f2"}}, "name");
        auto ret2 = json::Get("", "name");
        return ret1 == "f2" && ret2.type() == nlohmann::json::value_t::null;
    }
    
    static bool GetString() {
        auto ret1 = json::GetString({{"name","f2"}}, "name");
        auto ret2 = json::GetString("", "name");
        auto ret3 = json::GetString("", "name", "f2");
        return ret1 == "f2" && ret2 == "" && ret3 == "f2";
    }
    
    static bool GetNumber() {
        auto ret1 = json::GetNumber({{"value",100}}, "value");
        auto ret2 = json::GetNumber("", "name");
        auto ret3 = json::GetNumber("", "name", 100);
        return ret1 == 100 && ret2 == 0 && ret3 == 100;
    }
    
    static bool GetArray() {
        auto &ret1 = json::GetArray({{"value",{1,2,3}}}, "value");
        auto &ret2 = json::GetArray("", "name");
        auto &ret3 = json::GetArray("", "name", {1,2});
        return ret1.size() == 3 && ret2.type() == nlohmann::json::value_t::array && ret3.size() == 2;
    }
    
    static bool GetBool() {
        auto ret1 = json::GetBool({{"value",true}}, "value");
        auto ret2 = json::GetBool("", "name");
        auto ret3 = json::GetBool("", "name", true);
        return ret1 == true && ret2 == false && ret3 == true;
    }
    
    static bool GetObject() {
        auto obj1 = json::GetObject({{"value",{{"name","f2"}}}}, "value");
        auto obj2 = json::GetObject("", "name");
        auto obj3 = json::GetObject("", "value", {{"value",{{"name","f2"}}}});
        
        auto ret1 = json::GetString(obj1, "name");
        auto ret2 = obj2.type() == nlohmann::json::value_t::object;
        auto ret3 = json::GetString(json::GetObject(obj3, "value"), "name");
        return ret1 == "f2" && ret2 && ret3 == "f2";
    }
};

}
