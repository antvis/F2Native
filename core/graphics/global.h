#ifndef XG_GRAPHICS_GLOBAL_H
#define XG_GRAPHICS_GLOBAL_H

#include <string>
#include <vector>
#include <cmath>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

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
//默认刻度数量
constexpr int DEFAULT_COUNT = 5;

//默认透明度
constexpr float DEFAULT_OPACITY = NAN;

#if TARGET_OS_MACCATALYST == 1
constexpr float DEFAULT_FONTSIZE = 17.f;
#else
constexpr float DEFAULT_FONTSIZE = 10.f;
#endif

static std::string CreateFontStyle(float fontSize = DEFAULT_FONTSIZE,
                                const std::string &fontStyle = "normal",
                                const std::string &fontVariant = "normal",
                                const std::string &fontWeight = "normal",
                                const std::string &fontFamily = "sans-serif")  {
    return fontStyle + " " + fontVariant + " " + fontWeight + " " + std::to_string(fontSize) + "px " + fontFamily;
}
} // namespace xg

#endif // XG_GRAPHICS_GLOBAL_H
