//
//  CanvasColorParser.h
//  F2Tests
//
//  Created by weiqing.twq on 2022/1/10.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#include "../../../core/graphics/canvas/CanvasColorParser.h"

using namespace xg;
using namespace xg::canvas;
using namespace std;

namespace unit {

class CanvasColorParserTest {
public:
    
    static bool Parse() {
        //不同红色的解析
        CanvasColor color1;
        bool ret1 = CanvasColorParser::Parse("#ff0000", color1) && color1.r == 255 && color1.g == 0 && color1.b == 0 && color1.a == 255;
        
        CanvasColor color2;
        bool ret2 = CanvasColorParser::Parse("#f00", color2) && color2.r == 255 && color2.g == 0 && color2.b == 0 && color2.a == 255;
        
        CanvasColor color3;
        bool ret3 = CanvasColorParser::Parse("red", color3) && color3.r == 255 && color3.g == 0 && color3.b == 0 && color3.a == 255;
        
        CanvasColor color4;
        bool ret4 = CanvasColorParser::Parse("rgb(255, 0, 0)", color4) && color4.r == 255 && color4.g == 0 && color4.b == 0 && color4.a == 255;
        
        CanvasColor color5;
        bool ret5 = CanvasColorParser::Parse("rgba(255, 0, 0, 1)", color5) && color5.r == 255 && color5.g == 0 && color5.b == 0 && color5.a == 255;
        
        CanvasColor color6;
        bool ret6 = CanvasColorParser::Parse("#f00f", color6) && color6.r == 255 && color6.g == 0 && color6.b == 0 && color6.a == 255;
        
        CanvasColor color7;
        bool ret7 = CanvasColorParser::Parse("#ff0000ff", color7) && color7.r == 255 && color7.g == 0 && color7.b == 0 && color7.a == 255;
        
        CanvasColor color8;
        bool ret8 = CanvasColorParser::Parse("rgb(100%, 0%, 0%)", color8) && color8.r == 255 && color8.g == 0 && color8.b == 0 && color8.a == 255;
        
        CanvasColor color9;
        bool ret9 = CanvasColorParser::Parse("rgba(100%, 0%, 0%, 100%)", color9) && color9.r == 255 && color9.g == 0 && color9.b == 0 && color9.a == 255;
        return ret1 && ret2 &&ret3 && ret4 && ret5 && ret6 && ret7 && ret8 && ret9;
    }
    
    //错误的颜色解析
    static bool Error() {
        //错误的颜色长度
        CanvasColor color1;
        bool ret1 = !CanvasColorParser::Parse("#f00fa", color1) && color1.r == 0 && color1.g == 0 && color1.b == 0 && color1.a == 255;
        
        //不支持的hsl格式
        CanvasColor color2;
        bool ret2 = !CanvasColorParser::Parse("hsl(120,100%,50%)", color2) && color2.r == 0 && color2.g == 0 && color2.b == 0 && color2.a == 255;
        
        //不是合法的字符串
        CanvasColor color3;
        bool ret3 = !CanvasColorParser::Parse("#zzz", color3) && color3.r == 0 && color3.g == 0 && color3.b == 0 && color3.a == 255;
        
        //空字符串
        CanvasColor color4;
        bool ret4 = !CanvasColorParser::Parse("", color4) && color4.r == 0 && color4.g == 0 && color4.b == 0 && color4.a == 255;
        
        //不合法的rgb 内容不正确
        CanvasColor color5;
        bool ret5 = !CanvasColorParser::Parse("rgb(a, b, c, d)", color5) && color5.r == 0 && color5.g == 0 && color5.b == 0 && color5.a == 255;
        
        //不合法的括号
        CanvasColor color6;
        bool ret6 = !CanvasColorParser::Parse("aaaargb(a, b, c, d)", color6) && color6.r == 0 && color6.g == 0 && color6.b == 0 && color6.a == 255;
        
        //不合法的rgb 少括号 但却正确解析了？解析判断不严格
        CanvasColor color7;
        bool ret7 = CanvasColorParser::Parse("rgb(255, 0, 0, 255", color7) && color7.r == 255 && color7.g == 0 && color7.b == 0 && color7.a == 255;
        
        //不合法的rgb 逗号后面少数字
        CanvasColor color8;
        bool ret8 = !CanvasColorParser::Parse("rgb(255, 0, 0,）", color8) && color8.r == 0 && color8.g == 0 && color8.b == 0 && color8.a == 255;
        
        //不合法的rgb 前面是空格
        CanvasColor color9;
        bool ret9 = !CanvasColorParser::Parse("rgb(, 0, 0,）", color9) && color9.r == 0 && color9.g == 0 && color9.b == 0 && color9.a == 255;
        return ret1 && ret2 && ret3 && ret4 && ret5 && ret6 && ret7 && ret8 && ret9;
    }
};

} //unit
