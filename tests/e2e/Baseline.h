//
//  Baseline.h
//  F2Tests
//
//  Created by weiqing.twq on 2022/1/18.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#include <fstream>
#include <iostream>
#include "graphics/XChart.h"

using namespace xg;

namespace e2e {

class Baseline {
public:
    
    //基础的线图
    static bool Case1(float width, float height,float ratio, const std::string &path, void *context) {
        XChart chart("Baseline#Test", width, height, ratio);
        std::ifstream jsonFile(path);
        std::string jsonData((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());
        chart.Source(jsonData);
        chart.SetCoreGraphicsContext(context).Padding(20, 10, 20, 0);
        chart.Scale("date", "{\"tickCount\": 3}");
        chart.Scale("value", "{\"nice\": true}");
        chart.Line().Position("date*value");
        chart.Render();
        return true;
    }
};

}
