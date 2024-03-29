#ifndef XG_GRAPHICS_UTIL_COLOR_H
#define XG_GRAPHICS_UTIL_COLOR_H

#include <string>
#include <vector>
#include "../../nlohmann/json.hpp"
#include "../canvas/CanvasFillStrokeStyle.h"
#include "../global.h"

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
