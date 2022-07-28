#ifndef XG_GRAPHICS_LEGEND_CONTROLLER_H
#define XG_GRAPHICS_LEGEND_CONTROLLER_H

#include <string>
#include <unordered_map>
#include "../shape/Group.h"
#include "../shape/Marker.h"
#include "../shape/Text.h"
#include "../../nlohmann/json.hpp"

namespace xg {
class XChart;

namespace legend {

struct LegendStyle {
    float textSize = DEFAULT_FONTSIZE;
    string fill = "#808080";
    string textAlign = "start";
    string textBaseline = "top";
};
extern void from_json(const nlohmann::json &j, LegendStyle &n);

struct LegendCfg {
    size_t horizontalItems = 3;
    size_t itemMarginBottom = 12;
    size_t lineBottom = 5;
    size_t itemGap = 10;
    size_t wordSpace = 6;
    LegendStyle nameStyle;
    string symbol = "circle";
    float radius = 3;
    string position = "top";
    string layout;
    string align;
    string verticalAlign;
    bool hidden = false;
    
    void Reset(const std::string &position_) {        
        if (position_ == "top" || position_ == "bottom") {
            layout = layout.empty() ? "horizontal" : layout;
            align = align.empty() ? "left" : align;
        } else if(position_ == "right" || position_ == "middle" ) {
            layout = layout.empty() ? "vertical" : layout;
            verticalAlign = align.empty() ? "middle" : align;
        }        
    }
};
extern void from_json(const nlohmann::json &j, LegendCfg &n);

//struct LegendCfg {
//    bool hidden = false;
//    LegendLayout layout;
//    LegendLayout top {.position = "top", .layout = "horizontal", .align = "left"};
//    LegendLayout right {.position = "left", .layout = "vertical", .align = "middle"};
//    LegendLayout bottom {.position = "right", .layout = "vertical", .align = "middle"};
//    LegendLayout left {.position = "bottom", .layout = "horizontal", .align = "left"};
//
//    LegendLayout &GetLayoutByPosition() {
//        if (position == "top") {
//            return top;
//        }else if (position == "left") {
//            return left;
//        }else if (position == "right") {
//            return right;
//        }else {
//            return bottom;
//        }
//    }
//};
//extern void from_json(const nlohmann::json &j, LegendCfg &l);

static nlohmann::json DefaultLegendConfig() {
    nlohmann::json defaultCfg = {{"horizontalItems", 3},
                                 {"itemMarginBottom", 12},
                                 {"lineBottom", 5},
                                 {"itemGap", 10},
                                 {"showTitle", false},
                                 {"titleStyle", {{"textSize", DEFAULT_FONTSIZE}, {"fill", "#808080"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
                                 {"nameStyle", {{"textSize", DEFAULT_FONTSIZE}, {"fill", "#808080"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
                                 {"valueStyle", {{"textSize", DEFAULT_FONTSIZE}, {"fill", "#808080"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
                                 {"itemWidth", "auto"},
                                 {"wordSpace", 6},
                                 {"radius", 3},
                                 {"symbol", "circle"}};

    nlohmann::json top = {{"position","top"},
                          {"layout", "horizontal"},
                          {"align", "left"}};
    top.merge_patch(defaultCfg);

    nlohmann::json left = {{
                               "position",
                               "left",
                           },
                           {"layout", "vertical"},
                           {"verticalAlign", "middle"}};
    left.merge_patch(defaultCfg);

    nlohmann::json right = {{
                                "position",
                                "right",
                            },
                            {"layout", "vertical"},
                            {"verticalAlign", "middle"}};
    right.merge_patch(defaultCfg);

    nlohmann::json bottom = {{
                                 "position",
                                 "bottom",
                             },
                             {"layout", "horizontal"},
                             {"align", "left"}};
    bottom.merge_patch(defaultCfg);

    nlohmann::json config = {{"common", defaultCfg}, {"top", top}, {"right", right}, {"bottom", bottom}, {"left", left}};

    return config;
}

struct LegendRange {
    double left = 0;
    double top = 0;
    double right = 0;
    double bottom = 0;
};

struct LegendMarker {
    std::string fill;
    float radius = 3;
    std::string symbol = "circle";
    std::string stroke = "#ffffff";
};

struct LegendItem {
    std::string name;
    std::string field;
    LegendMarker marker;
    std::string value;
};

class LegendController;
class Legend {
    friend LegendController;

  public:
    Legend(const std::string &field, const LegendCfg &cfg, const std::vector<legend::LegendItem> &legendItems)
        : field_(field), cfg_(cfg), legendItems_(legendItems) {}

    void CreateShape(XChart &chart, shape::Group *container, const util::Point &originPoint);

    util::Point AlignLegend(XChart &chart, const string &position, const float legendHeight, const float legendWidth);

    float CalLegendWidth(XChart &chart);
    float CalLegendHeight(XChart &chart);

  private:
    std::string field_;
    LegendCfg cfg_;
    std::vector<legend::LegendItem> legendItems_;
    float width_ = 0.f;
    float height_ = 0.f;
};

class LegendController {
  public:
    LegendController(shape::Group *_container) : container_(_container) {}

    ~LegendController() { container_ = nullptr; }

    void SetFieldConfig(const std::string &field, const LegendCfg &cfg);

    void Render(XChart &chart);

    inline LegendRange GetRange() const noexcept { return legendRange_; }

    void OnToolTipMarkerItemsChanged();

    void Redraw(XChart &chart);

    void ClearInner();

  private:
    void AddLegend(XChart &chart, const std::string &field, const std::vector<LegendItem> &fieldItems);

  private:
    unordered_map<string, LegendCfg> legendCfg_;
    std::string position_ = "top";
    LegendRange legendRange_{};
    std::unordered_map<std::string, std::vector<Legend>> legends_;
    float legendWidth_ = 0.f;
    float legendHeight_ = 0.f;
//    nlohmann::json markerItems_;
    shape::Group *container_ = nullptr;
    bool hidden_ = false;
};
} // namespace legend
} // namespace xg

#endif // XG_GRAPHICS_LEGEND_CONTROLLER_H
