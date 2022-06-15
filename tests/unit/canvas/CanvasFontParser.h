//
//  CanvasFontStyle.h
//  F2
//
//  Created by weiqing.twq on 2022/1/10.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#include "../../../core/graphics/canvas/CanvasFontParser.h"
#include "../../../core/utils/common.h"

using namespace xg;
using namespace xg::canvas;
using namespace std;

namespace unit {

class CanvasFontParserTest {
public:
    static bool Parse() {
        CanvasFont style;
        bool ret1 = CanvasFontParser::Parse("normal normal normal 26.086958px sans-serif", style) &&
        style.fontStyle == CanvasFont::Style::NORMAL &&
        style.fontVariant == CanvasFont::Variant::NORMAL &&
        style.fontWeight == CanvasFont::Weight::NORMAL &&
        xg::IsEqualDeviation(style.fontSize, 26.086958, 0.0001) &&
        style.fontFamily == "sans-serif";
        
        CanvasFont style2;
        bool ret2 = CanvasFontParser::Parse("bold 48px serif", style2) &&
        style2.fontStyle == CanvasFont::Style::NORMAL &&
        style2.fontVariant == CanvasFont::Variant::NORMAL &&
        style2.fontWeight == CanvasFont::Weight::BOLD &&
        xg::IsEqualDeviation(style2.fontSize, 48, 0.0001) &&
        style2.fontFamily == "serif";
        
        CanvasFont style3;
        bool ret3 = CanvasFontParser::Parse("bold 48pt serif", style3) &&
        style3.fontStyle == CanvasFont::Style::NORMAL &&
        style3.fontVariant == CanvasFont::Variant::NORMAL &&
        style3.fontWeight == CanvasFont::Weight::BOLD &&
        xg::IsEqualDeviation(style3.fontSize, 48 * 4 / 3, 0.0001) &&
        style3.fontFamily == "serif";
        
        CanvasFont style4;
        bool ret4 = CanvasFontParser::Parse("oblique small-caps bold 26.086958px sans-serif", style4) &&
        style4.fontStyle == CanvasFont::Style::OBLIQUE &&
        style4.fontVariant == CanvasFont::Variant::SMALL_CAPS &&
        style4.fontWeight == CanvasFont::Weight::BOLD &&
        xg::IsEqualDeviation(style4.fontSize, 26.086958, 0.0001) &&
        style4.fontFamily == "sans-serif";
        
        //带/
        CanvasFont style5;
        bool ret5 = CanvasFontParser::Parse("oblique small-caps bold 26.086958px/ sans-serif", style5) &&
        style5.fontStyle == CanvasFont::Style::OBLIQUE &&
        style5.fontVariant == CanvasFont::Variant::SMALL_CAPS &&
        style5.fontWeight == CanvasFont::Weight::BOLD &&
        xg::IsEqualDeviation(style5.fontSize, 26.086958, 0.0001) &&
        style5.fontFamily == "sans-serif";
        
        CanvasFont style6;
        bool ret6 = CanvasFontParser::Parse("italic small-caps lighter 26.086958px sans-serif", style6) &&
        style6.fontStyle == CanvasFont::Style::ITALIC &&
        style6.fontVariant == CanvasFont::Variant::SMALL_CAPS &&
        style6.fontWeight == CanvasFont::Weight::LIGHTER &&
        xg::IsEqualDeviation(style6.fontSize, 26.086958, 0.0001) &&
        style6.fontFamily == "sans-serif";
        
        //只有大小和自体
        CanvasFont style8;
        bool ret8 = CanvasFontParser::Parse("26.086958px serif", style8) &&
        style8.fontStyle == CanvasFont::Style::NORMAL &&
        style8.fontVariant == CanvasFont::Variant::NORMAL &&
        style8.fontWeight == CanvasFont::Weight::NORMAL &&
        xg::IsEqualDeviation(style8.fontSize, 26.086958, .0001) &&
        style8.fontFamily == "serif";
        
        return ret1 && ret2 && ret3 && ret4 && ret5 && ret6 && ret8;
    }
    
    static bool Error() {
        //自体字符串内容过少
        CanvasFont style;
        bool ret1 = !CanvasFontParser::Parse("normal", style) &&
        style.fontStyle == CanvasFont::Style::NORMAL &&
        style.fontVariant == CanvasFont::Variant::NORMAL &&
        style.fontWeight == CanvasFont::Weight::NORMAL &&
        xg::IsEqualDeviation(style.fontSize, 10, .0001) &&
        style.fontFamily == "sans-serif";
        
        
        //空字符串
        CanvasFont style2;
        bool ret2 = !CanvasFontParser::Parse("", style2) &&
        style2.fontStyle == CanvasFont::Style::NORMAL &&
        style2.fontVariant == CanvasFont::Variant::NORMAL &&
        style2.fontWeight == CanvasFont::Weight::NORMAL &&
        xg::IsEqualDeviation(style2.fontSize, 10, .0001) &&
        style2.fontFamily == "sans-serif";
        
        //错误的字体
        CanvasFont style3;
        bool ret3 = !CanvasFontParser::Parse("normalx normalx normalx 26.086958pp sans-serifxx", style3) &&
        style3.fontStyle == CanvasFont::Style::NORMAL &&
        style3.fontVariant == CanvasFont::Variant::NORMAL &&
        style3.fontWeight == CanvasFont::Weight::NORMAL &&
        xg::IsEqualDeviation(style3.fontSize, 10, 0.0001) &&
        style3.fontFamily == "sans-serif";
        
        //fontfamily 为initial
        CanvasFont style4;
        bool ret4 = !CanvasFontParser::Parse("italic normal bolder 26.086958px initial", style4) &&
        style4.fontStyle == CanvasFont::Style::NORMAL &&
        style4.fontVariant == CanvasFont::Variant::NORMAL &&
        style4.fontWeight == CanvasFont::Weight::NORMAL &&
        xg::IsEqualDeviation(style4.fontSize, 10, 0.0001) &&
        style4.fontFamily == "sans-serif";
        
        //variant错误
        CanvasFont style5;
        bool ret5 = !CanvasFontParser::Parse("italic wrong bolder 26.086958px sans-serif", style5) &&
        style5.fontStyle == CanvasFont::Style::NORMAL &&
        style5.fontVariant == CanvasFont::Variant::NORMAL &&
        style5.fontWeight == CanvasFont::Weight::NORMAL &&
        xg::IsEqualDeviation(style5.fontSize, 10, 0.0001) &&
        style5.fontFamily == "sans-serif";
        
        //style错误
        CanvasFont style6;
        bool ret6 = !CanvasFontParser::Parse("wrong small-caps bolder 26.086958px sans-serif", style6) &&
        style6.fontStyle == CanvasFont::Style::NORMAL &&
        style6.fontVariant == CanvasFont::Variant::NORMAL &&
        style6.fontWeight == CanvasFont::Weight::NORMAL &&
        xg::IsEqualDeviation(style6.fontSize, 10, 0.0001) &&
        style6.fontFamily == "sans-serif";
        
        //bolder错误
        CanvasFont style7;
        bool ret7 = !CanvasFontParser::Parse("italic normal wrong 26.086958px sans-serif", style7) &&
        style7.fontStyle == CanvasFont::Style::NORMAL &&
        style7.fontVariant == CanvasFont::Variant::NORMAL &&
        style7.fontWeight == CanvasFont::Weight::NORMAL &&
        xg::IsEqualDeviation(style7.fontSize, 10, 0.0001) &&
        style7.fontFamily == "sans-serif";
        
        return ret1 && ret2 && ret3 && ret4 && ret5 && ret6 && ret7;
    }
};

}//unit
