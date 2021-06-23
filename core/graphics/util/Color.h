#include "Point.h"
#include "graphics/global.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#ifndef XG_GRAPHICS_UTIL_COLOR_H
#define XG_GRAPHICS_UTIL_COLOR_H

namespace xg {
namespace util {
struct ColorStop {
    float offset;
    std::string color;

    ColorStop() {}

    ColorStop(float value_offset, const std::string &value_color) {
        offset = value_offset;
        color = value_offset;
    }
};

class CanvasGradient {

  public:
    CanvasGradient() = default;

    CanvasGradient(const CanvasGradient &g) {
        for(auto i = 0; i < g.colorStops.size(); i++) {
            const ColorStop &cs = g.colorStops[i];
            this->addColorStop(cs.offset, cs.color);
        }
    }

    virtual bool isLinearGradient() { return true; };

    /// 标准接口, 添加colorStop
    /// offset
    /// color
    void addColorStop(float offset, std::string color) {
        ColorStop cs;
        cs.offset = offset;
        cs.color = color;
        colorStops.push_back(cs);
    }

    std::vector<ColorStop> colorStops;
};

class CanvasLinearGradient : public CanvasGradient {
  public:
    CanvasLinearGradient() = default;

    CanvasLinearGradient(const CanvasLinearGradient &g) : CanvasGradient(g) {
        start = g.start;
        end = g.end;
    }

    Point start, end;

    bool isLinearGradient() override { return true; }
};

class CanvasRadialGradient : public CanvasGradient {
  public:
    CanvasRadialGradient() = default;

    CanvasRadialGradient(const CanvasRadialGradient &g) : CanvasGradient(g) {
        memcpy(start, g.start, sizeof(float) * 3);
        memcpy(end, g.end, sizeof(float) * 3);
    }

    float start[3] = {0}, end[3] = {0};

    bool isLinearGradient() override { return false; }
};

enum class CanvasFillStrokeStyleType { kNone, kColor, kLinearGradient, kRadialGradient };

struct CanvasFillStrokeStyle {
    CanvasFillStrokeStyle() = default;

    CanvasFillStrokeStyle(const CanvasFillStrokeStyle &style)
        : type(style.type), color(style.color), linearGradient(style.linearGradient), radialGradient(style.radialGradient) {}

    CanvasFillStrokeStyle(const std::string &color) {
        type = CanvasFillStrokeStyleType::kColor;
        this->color = color;
    }

    CanvasFillStrokeStyle(const CanvasLinearGradient &g) {
        type = CanvasFillStrokeStyleType::kLinearGradient;
        this->linearGradient = g;
    }

    CanvasFillStrokeStyle(const CanvasRadialGradient &g) {
        type = CanvasFillStrokeStyleType::kRadialGradient;
        this->radialGradient = g;
    }

    CanvasFillStrokeStyleType type = CanvasFillStrokeStyleType::kNone;

    std::string color;

    // ref to pattern / linear gradient / radial gradient
    CanvasLinearGradient linearGradient;
    CanvasRadialGradient radialGradient;
};

static CanvasFillStrokeStyle ColorParser(const nlohmann::json &color) {
    if(color.is_string()) {
        std::string c = color.get<std::string>();
        if(c.length() > 1 && c.substr(0, 1) == "#") {
            //截取超长的color 如#ff0000ff
            c = c.length() > 7 ? c.substr(0, 7) : c;
            return CanvasFillStrokeStyle(c);
        }
        nlohmann::json _color = nlohmann::json::parse(c, nullptr, false);
        if(_color.is_object() && _color.contains("colorStops") && _color["colorStops"].is_array()) {
            bool isLinear = _color.contains("isLinear") ? _color["isLinear"].get<bool>() : true;
            if(isLinear) {
                CanvasLinearGradient gradient;
                nlohmann::json &colorStops = _color["colorStops"];
                for(std::size_t i = 0; i < colorStops.size(); ++i) {
                    nlohmann::json &stop = colorStops[i];
                    gradient.addColorStop(stop["offset"], stop["color"]);
                }

                if(_color.contains("position") && _color["position"].size() >= 4) {
                    nlohmann::json &position = _color["position"];
                    gradient.start = util::Point{position[0], position[1]};
                    gradient.end = util::Point{position[2], position[3]};
                }
                return gradient;
            } else {
                CanvasRadialGradient gradient;

                nlohmann::json &colorStops = _color["colorStops"];
                for(std::size_t i = 0; i < colorStops.size(); ++i) {
                    nlohmann::json &stop = colorStops[i];
                    gradient.addColorStop(stop["offset"], stop["color"]);
                }

                if(_color.contains("position") && _color["position"].size() >= 6) {
                    nlohmann::json &position = _color["position"];
                    gradient.start[0] = position[0];
                    gradient.start[1] = position[1];
                    gradient.start[2] = position[2];

                    gradient.end[0] = position[3];
                    gradient.end[1] = position[4];
                    gradient.end[2] = position[5];
                }
                return gradient;
            }
        }
    }
    return CanvasFillStrokeStyle(GLOBAL_COLORS[0]);
}

static CanvasFillStrokeStyle ColorParser(const nlohmann::json &data, const std::string &key) {
    if(data.is_object() && data.contains(key)) {
        return ColorParser(data[key]);
    }
    return CanvasFillStrokeStyle(GLOBAL_COLORS[0]);
}

static float OpacityParser(const nlohmann::json &color) {
    if(color.is_string()) {
        std::string c = color.get<std::string>();
        if(c.length() > 7 && c.substr(0, 1) == "#") {
            //截取超长的color 如#ff0000ff
            c =  c.substr(7,  c.length());
            long n = strtol(c.c_str(), NULL, 16);
            return n / 255.0f;
        }
    }
    return DEFAULT_OPACITY;
}

static float OpacityParser(const nlohmann::json &data, const std::string &key) {
    if(data.is_object() && data.contains(key)) {
        return OpacityParser(data[key]);
    }
    return DEFAULT_OPACITY;
}

} // namespace util
} // namespace xg

#endif // XG_GRAPHICS_UTIL_COLOR_H
