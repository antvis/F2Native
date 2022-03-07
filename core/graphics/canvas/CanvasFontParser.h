#ifndef XG_GRAPHICS_CANVAS_CANVASFONTSTYLE_H
#define XG_GRAPHICS_CANVAS_CANVASFONTSTYLE_H

#include <string>

namespace xg {
namespace canvas {

struct CanvasFont {
    
    //NORMAL,ITALIC的枚举与android对齐 不要轻易改动
    enum class Style { INVALID = -0x0001, NORMAL = 0x0000, ITALIC = 0x0001, OBLIQUE = 0x0004 };
    
    enum class Variant { INVALID = -0x0001, NORMAL = 0x0008, SMALL_CAPS = 0x0010 };
    
    enum class Weight {
        INVALID = -0x0001,
        LIGHTER = 50,
        THIN = 100,        // 100
        EXTRA_LIGHT = 200, // 200
        LIGHT = 300,       // 300
        NORMAL = 400,      // 400
        MEDIUM = 500,      // 500
        SEMI_BOLD = 600,   // 600
        BOLD = 700,        // 700
        BOLDER = 750,
        EXTRA_BOLD = 800,   // 800
        BLACK = 900,        // 900
        EXTRA_BLACK = 1000, // 1000
    };
    
    //外部需要访问
    std::string fontFamily;
    float fontSize;
    Style fontStyle;
    Variant fontVariant;
    Weight fontWeight;
    
    CanvasFont():
    fontSize(10),
    fontFamily("sans-serif"),
    fontStyle(Style::NORMAL),
    fontVariant(Variant::NORMAL),
    fontWeight(Weight::NORMAL) {};
};

class CanvasFontParser {
public:
    //入口解析函数
    // font 格式为 normal normal normal 26.086958px sans-serif
    static bool Parse(const std::string &font, CanvasFont &fontStyle);
    
    CanvasFontParser()
    : fontName("10px sans-serif"){}
private:
    static bool ParseFields(const std::string &font,
                            CanvasFont::Style &style,
                            CanvasFont::Variant &variant,
                            CanvasFont::Weight &weight,
                            float &fontStyleSize,
                            std::string &outFontFamily);
    
    
private:
    std::string fontName;
};
} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_CANVAS_CANVASFONTSTYLE_H
