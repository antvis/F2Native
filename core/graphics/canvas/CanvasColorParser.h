//
// Created by ruize on 2022/1/7.
//

#ifndef XG_GRAPHICS_CANVAS_CANVASCOLORPARSER_H
#define XG_GRAPHICS_CANVAS_CANVASCOLORPARSER_H

#include <mutex>
#include <stdio.h>
#include <string>
#include <unordered_map>

namespace xg {

namespace canvas {

struct CanvasColor {
    float r = 0;
    float g = 0;
    float b = 0;
    float a = 0;
};

class CanvasColorParser {

  public:
    /*
     *入口解析函数
     *支持解析#RRGGBBAA,#RGB,字符串（white,red),rgba(1, 1, 1, 1), rgb(1, 1, 1), hsl(0, 100%, 50%)，hsla(120,100%,50%,0.3
     */
    static bool Parse(const std::string &str, CanvasColor &output);

    /*
     * 解析成安卓平台的ARGB格式
     */
    static int RGBAToHex(const CanvasColor &color);
  private:
    static void InitColorMapIfEmpty();

  private:
    static std::unordered_map<std::string, CanvasColor> colorMap;
    static std::string hex_digit;
};

} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_CANVAS_CANVASCOLORPARSER_H
