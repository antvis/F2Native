#ifndef XG_GRAPHICS_GLOBAL_H
#define XG_GRAPHICS_GLOBAL_H

#include <string>
#include <vector>

namespace xg {
//默认颜色
const std::vector<string> GLOBAL_COLORS = {"#1890FF", "#2FC25B", "#FACC14", "#223273",
                                           "#8543E0", "#13C2C2", "#3436C7", "#F04864"};
//虚线间隔
const std::vector<float> GLOBAL_LINE_DASH = {4, 4};

//默认size大小
const vector<float> GLOBAL_SIZES = {4.f, 10.f};
} // namespace xg

#endif // XG_GRAPHICS_GLOBAL_H
