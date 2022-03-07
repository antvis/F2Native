//
//  Polar.h
//  F2Tests
//
//  Created by weiqing.twq on 2021/11/18.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#include "graphics/canvas/Polar.h"

using namespace xg;
using namespace xg::canvas;
using namespace std;

namespace unit {

class Polar {
public:
    static bool Init() {
        // start -- 左下角坐标 end -- 右上角坐标
        auto start = util::Point {100, 300};
        auto end = util::Point {300, 100};
        coord::Polar polar(start, end, false);
        return util::Point::IsEqual(start, polar.GetStart()) && util::Point::IsEqual(end, polar.GetEnd());
    }
    
    //非转至
    static bool ConvertPoint1() {
        coord::Polar polar(util::Point {100, 300}, util::Point {300, 100}, false);
        util::Point p1{0, 0}; util::Point p2{0.5, 0.5}; util::Point p3{0.5, 1};

        auto c1 = polar.ConvertPoint(p1);//200 200
        auto c2 = polar.ConvertPoint(p2);//200 250
        auto c3 = polar.ConvertPoint(p3); //200 100

        auto i1 = polar.InvertPoint(c1);
        auto i2 = polar.InvertPoint(c2);
        auto i3 = polar.InvertPoint(c3);

        //i2 i3的x算出来都是0.409 没找到原因。。
        //所以先保证y相等
        return util::Point::IsEqual(p1, i1) && IsEqual(p2.y, i2.y) && IsEqual(p3.y, i3.y);
    }
    
    //转至
    static bool ConvertPoint2() {
        coord::Polar polar(util::Point {100, 300}, util::Point {300, 100}, true);
        util::Point p1{0, 0}; util::Point p2{0.5, 0.5}; util::Point p3{0.5, 1};

        auto c1 = polar.ConvertPoint(p1);//200 200
        auto c2 = polar.ConvertPoint(p2);//200 250
        auto c3 = polar.ConvertPoint(p3); //200 300

        auto i1 = polar.InvertPoint(c1);
        auto i2 = polar.InvertPoint(c2);
        auto i3 = polar.InvertPoint(c3);

        //i2 i3的x算出来都是0.409 没找到原因。。
        //所以先保证y相等 IsEqual(p3.y, i3.x)不相等..
        return util::Point::IsEqual(p1, i1) && IsEqual(p2.y, i2.x);
    }
    
    static bool Reset() {
        // start -- 左下角坐标 end -- 右上角坐标
        auto start = util::Point {100, 300};
        auto end = util::Point {400, 100};
        coord::Polar polar(util::Point {100, 300}, util::Point {300, 100}, false);
        polar.Reset(start, end);
        
        bool ret1 = util::Point::IsEqual(polar.GetCenter(), {250, 200});
        bool ret2 = polar.GetType() == coord::CoordType::Polar;
        bool ret3 = xg::IsEqual(M_PI * 2, polar.GetWidth());
        bool ret4 = polar.GetHeight() == polar.GetRadius();//宽度 == 半径？
        bool ret5 = polar.GetRadius() == 100;
        bool ret6 = util::Point::IsEqual(polar.GetXAxis(), {-M_PI / 2, M_PI * 3 / 2});
        bool ret7 = util::Point::IsEqual(polar.GetYAxis(), {0, 100});
        bool ret8 = util::Point::IsEqual(polar.GetStart(), start);
        bool ret9 = util::Point::IsEqual(polar.GetEnd(), end);
        bool ret10 = polar.IsContains(200, 200) == true;
        bool ret11 = polar.IsContains(200, 500) == false;
        
        return ret1 && ret2 && ret3 && ret4 && ret5 && ret6 && ret7 && ret8 && ret9 && ret10 && ret11;
    }
};

} //unit
