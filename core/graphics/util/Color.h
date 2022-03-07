#include "graphics/canvas/CanvasFillStrokeStyle.h"
#include "graphics/global.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#ifndef XG_GRAPHICS_UTIL_COLOR_H
#define XG_GRAPHICS_UTIL_COLOR_H

namespace xg {
namespace util {

canvas::CanvasFillStrokeStyle ColorParser(const nlohmann::json &color);
canvas::CanvasFillStrokeStyle ColorParser(const nlohmann::json &data, const std::string &key);
float OpacityParserString(const std::string &color);
float OpacityParser(const nlohmann::json &color);
float OpacityParser(const nlohmann::json &data, const std::string &key);

} // namespace util
} // namespace xg

#endif // XG_GRAPHICS_UTIL_COLOR_H
