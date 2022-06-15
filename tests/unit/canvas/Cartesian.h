//
//  Cartesian.h
//  F2Tests
//
//  Created by weiqing.twq on 2021/11/24.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#include "../../../core/graphics/canvas/Cartesian.h"

using namespace xg;
using namespace xg::canvas;
using namespace std;

namespace unit {

class Cartesian {
public:
    static bool Init() {
        // start -- 左下角坐标 end -- 右上角坐标
        auto start = util::Point {100, 300};
        auto end = util::Point {300, 100};
        coord::Cartesian cart(start, end, false);
        return util::Point::IsEqual(start, cart.GetStart()) && util::Point::IsEqual(end, cart.GetEnd());
    }
    
    //非转至
    static bool ConvertPoint1() {
        coord::Cartesian cart(util::Point {100, 300}, util::Point {300, 100}, false);
        util::Point p1{0, 0}; util::Point p2{0.5, 0.5}; util::Point p3{0.5, 1};

        auto c1 = cart.ConvertPoint(p1);//200 200
        auto c2 = cart.ConvertPoint(p2);//200 250
        auto c3 = cart.ConvertPoint(p3); //200 100

        auto i1 = cart.InvertPoint(c1);
        auto i2 = cart.InvertPoint(c2);
        auto i3 = cart.InvertPoint(c3);

        return util::Point::IsEqual(p1, i1) && util::Point::IsEqual(p2, i2) && util::Point::IsEqual(p3, i3);
    }
    
    //转至
    static bool ConvertPoint2() {
        coord::Cartesian cart(util::Point {100, 300}, util::Point {300, 100}, true);
        util::Point p1{0, 0}; util::Point p2{0.5, 0.5}; util::Point p3{0.5, 1};

        auto c1 = cart.ConvertPoint(p1);//200 200
        auto c2 = cart.ConvertPoint(p2);//200 250
        auto c3 = cart.ConvertPoint(p3); //200 100

        auto i1 = cart.InvertPoint(c1);
        auto i2 = cart.InvertPoint(c2);
        auto i3 = cart.InvertPoint(c3);

        return util::Point::IsEqual(p1, i1) && util::Point::IsEqual(p2, i2) && util::Point::IsEqual(p3, i3);
    }
    
    static bool Reset() {
        // start -- 左下角坐标 end -- 右上角坐标
        auto start = util::Point {100, 300};
        auto end = util::Point {300, 100};
        coord::Cartesian cart({0, 0}, {0, 0}, false);
        cart.Reset(start, end);
        
        return util::Point::IsEqual(cart.GetCenter(), {200, 200}) &&
        cart.GetType() == coord::CoordType::Cartesian &&
        cart.GetWidth() == 200 &&
        cart.GetHeight() == 200 &&
        cart.GetRadius() == 0 &&
        util::Point::IsEqual(cart.GetXAxis(), {100, 300}) &&
        util::Point::IsEqual(cart.GetYAxis(), {300, 100}) &&
        util::Point::IsEqual(cart.GetStart(), start) &&
        util::Point::IsEqual(cart.GetEnd(), end) &&
        cart.IsContains(200, 200) == true &&
        cart.IsContains(200, 500) == false;
    }
};

}
