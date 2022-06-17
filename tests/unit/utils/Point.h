//
//  Point.h
//  F2Tests
//
//  Created by weiqing.twq on 2021/11/18.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#include "../../../core/graphics/util/Point.h"

using namespace xg;
using namespace std;

namespace unit {

class PointTest {
public:
    static bool Reset() {
        auto point = util::Point(0, 0);
        point.Reset(1, 2);
        return point.x == 1 && point.y == 2;
    }

    static bool Max() {
        return true;
    }

    static bool Min() {
        return true;
    }

    static bool Add() {
        return true;
    }

    static bool Sub() {
        return true;
    }

    static bool Scale() {
        return true;
    }

    static bool Distance() {
        return true;
    }
    
};

}
