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

    nlohmann::json top = {{
                              "position",
                              "right",
                          },
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
    Legend(const std::string &field, const nlohmann::json &cfg, const std::vector<legend::LegendItem> &legendItems)
        : field_(field), cfg_(cfg), legendItems_(legendItems) {}

    void CreateShape(XChart &chart, shape::Group *container, const util::Point &originPoint);

    util::Point AlignLegend(XChart &chart, const string &position, const float legendHeight, const float legendWidth);

    float CalLegendWidth(XChart &chart);
    float CalLegendHeight(XChart &chart);
    float CalLegendStep(XChart &chart);

  private:
    std::string field_;
    nlohmann::json cfg_;
    std::vector<legend::LegendItem> legendItems_;
    float width_ = 0.f;
    float height_ = 0.f;
};

class LegendController {
  public:
    LegendController(shape::Group *_container) : container_(_container) {}

    ~LegendController() { container_ = nullptr; }

    void SetFieldConfig(std::string field, nlohmann::json cfg = {});

    void Render(XChart &chart);

    inline LegendRange GetRange() const noexcept { return legendRange_; }

    void OnToolTipMarkerItemsChanged(nlohmann::json &items);

    void Redraw(XChart &chart);

    void ClearInner();

  private:
    void AddLegend(XChart &chart, const std::string &field, const std::vector<LegendItem> &fieldItems);

  private:
    nlohmann::json legendCfg_;
    bool enable_ = true;
    std::string position_ = "";
    LegendRange legendRange_{};
    std::unordered_map<std::string, std::vector<Legend>> legends_;
    float legendWidth_ = 0.f;
    float legendHeight_ = 0.f;
    nlohmann::json markerItems_;
    shape::Group *container_ = nullptr;
};
} // namespace legend
} // namespace xg

#endif // XG_GRAPHICS_LEGEND_CONTROLLER_H
