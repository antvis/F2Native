#ifndef XG_GRAPHICS_UTIL_COLOR_H
#define XG_GRAPHICS_UTIL_COLOR_H

#include <string>
#include <vector>
#include "../canvas/CanvasFillStrokeStyle.h"
#include "../global.h"

namespace xg {
namespace util {

canvas::CanvasFillStrokeStyle ColorParser(const std::string &color);
float OpacityParserString(const std::string &color);
float OpacityParser(const std::string &color);

} // namespace util
} // namespace xg

#endif // XG_GRAPHICS_UTIL_COLOR_H
