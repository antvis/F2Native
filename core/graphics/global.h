#ifndef XG_GRAPHICS_GLOBAL_H
#define XG_GRAPHICS_GLOBAL_H

#include <string>
#include <vector>
#include <cmath>

#define GLOBAL_SIZES                                                                                                           \
    std::vector<float> { 4.f, 10.f }
#define GLOBAL_LINE_DASH                                                                                                       \
    std::vector<float> { 4, 4 }
#define SNAP_COUNT_ARRAY                                                                                                       \
    std::vector<float> { 1, 1.2, 1.5, 2, 2.2, 2.4, 2.5, 3, 4, 5, 6, 7.5, 8, 10 }

namespace xg {
//默认颜色
constexpr const char *GLOBAL_COLORS[] = {"#1890FF", "#2FC25B", "#FACC14", "#223273",
                                         "#8543E0", "#13C2C2", "#3436C7", "#F04864"};
//虚线间隔
// const std::vector<float> GLOBAL_LINE_DASH = {4, 4};

//默认size大小
// const std::vector<float> GLOBAL_SIZES = {4.f, 10.f};

//刻度间隔
// const std::vector<float> SNAP_COUNT_ARRAY = {1, 1.2, 1.5, 2, 2.2, 2.4, 2.5, 3, 4, 5, 6, 7.5, 8, 10};
//默认刻度数量
constexpr int DEFAULT_COUNT = 5;

constexpr float DEFAULT_OPACITY = NAN;
} // namespace xg

#endif // XG_GRAPHICS_GLOBAL_H
