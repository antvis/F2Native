#include <map>

#include "LegendController.h"
#include "graphics/XChart.h"
#include "graphics/scale/Scale.h"
#include <math.h>

using namespace xg;

void legend::Legend::CreateShape(XChart &chart, shape::Group *container, const util::Point &originPoint) {
    // draw
    util::Point currPoint = Point(originPoint.x, originPoint.y);
    nlohmann::json &nameStyle = cfg_["nameStyle"];
    float ratio = chart.GetRatio();
    float wordSpace = cfg_["wordSpace"];
    float itemGap = cfg_["itemGap"];
    float textSize = nameStyle["textSize"];
    float r_itemGap = itemGap * ratio;
    int horizontalItems = cfg_["horizontalItems"];
    float step = width_ / horizontalItems;
    float lineBottom = cfg_["lineBottom"];
    float radius = cfg_["radius"];
    float r_radius = radius * ratio;

    for(size_t i = 0; i < legendItems_.size(); i++) {
        LegendItem &item = legendItems_[i];
        // text
        std::unique_ptr<xg::shape::Text> text =
            xg::make_unique<xg::shape::Text>(item.name + "\t" + item.value,
                                             Point(currPoint.x + r_radius * 2 + wordSpace * ratio, currPoint.y),
                                             textSize * ratio, nameStyle["fill"], nameStyle["fill"]);
        text->SetTextAlign(nameStyle["textAlign"]);
        text->SetTextBaseline(nameStyle["textBaseline"]);
        xg::util::BBox textBbox = text->GetBBox(chart.GetCanvasContext());
        container->AddElement(std::move(text));

        // marker
        util::Point center = Point(currPoint.x + r_radius, currPoint.y + (textBbox.height / 2));
        auto marker = xg::make_unique<xg::shape::Marker>(center, r_radius, item.marker.fill, cfg_["symbol"]);
        container->AddElement(std::move(marker));

        if(cfg_["layout"] == "vertical") {
            currPoint.Reset(currPoint.x, textBbox.maxY + r_itemGap);
        } else {
            int y_num = floor((i + 1) / horizontalItems);
            int x_num = (i + 1) % horizontalItems;
            currPoint.Reset(originPoint.x + step * x_num, originPoint.y + y_num * (textBbox.height + lineBottom * ratio));
        }
    }
}

util::Point legend::Legend::AlignLegend(XChart &chart, const string &position, const float legendHeight, const float legendWidth) {
    float x = 0.f;
    float y = 0.f;
    float itemMarginBottom = cfg_["itemMarginBottom"];
    string verticalAlign = cfg_["verticalAlign"].is_string() ? cfg_["verticalAlign"] : "middle";
    string align = cfg_["align"].is_string() ? cfg_["align"] : "center";
    if(position == "left" || position == "right") {
        x = (position == "left") ?
                chart.GetMargin()[0] + chart.GetPadding()[0] :
                chart.GetWidth() - legendWidth - chart.GetMargin()[2] - chart.GetPadding()[2] + itemMarginBottom * chart.GetRatio();
        y = (chart.GetHeight() - legendHeight) / 2;
        if(verticalAlign == "top") {
            y = chart.GetMargin()[1] + chart.GetPadding()[1];
        } else if(verticalAlign == "bottom") {
            y = chart.GetHeight() - legendHeight;
        }
    } else {
        x = chart.GetPadding()[0];
        y = (position == "top") ? chart.GetPadding()[1] : chart.GetPadding()[1] + chart.GetHeight() - legendHeight;
    }
    return Point(x, y);
}

float legend::Legend::CalLegendWidth(XChart &chart) {

    float width = 0;
    float ratio = chart.GetRatio();
    nlohmann::json &nameStyle = cfg_["nameStyle"];
    float wordSpace = cfg_["wordSpace"];
    float textSize = nameStyle["textSize"];
    float itemMarginBottom = cfg_["itemMarginBottom"];

    if(cfg_["layout"] == "vertical") {
        auto cal = [&](LegendItem &item) {
            float r_radius = item.marker.radius * ratio;
            auto text = xg::make_unique<shape::Text>(item.name, util::Point(0, 0), textSize * ratio, "", "");
            float t_width = text->GetTextWidth(chart.GetCanvasContext());
            float i_width = r_radius * 2 + wordSpace * ratio + t_width;
            width = fmax(i_width, width);
        };
        std::for_each(legendItems_.begin(), legendItems_.end(), cal);
        width += itemMarginBottom * ratio;
    } else {
        width = chart.GetWidth() - chart.GetPadding()[0] - chart.GetMargin()[0] - chart.GetPadding()[2] - chart.GetMargin()[2];
    }

    width_ = width;
    return width_;
}

float legend::Legend::CalLegendHeight(XChart &chart) {
    float height = 0.f;
    float ratio = chart.GetRatio();
    nlohmann::json &nameStyle = cfg_["nameStyle"];
    float wordSpace = cfg_["wordSpace"];
    float textSize = nameStyle["textSize"];
    float itemMarginBottom = cfg_["itemMarginBottom"];
    int horizontalItems = cfg_["horizontalItems"];
    float lineBottom = cfg_["lineBottom"];

    if(cfg_["layout"] == "vertical") {
        auto cal = [&](LegendItem &item) {
            auto text = xg::make_unique<shape::Text>(item.name, util::Point(0, 0), textSize * ratio, "", "");
            float i_height = text->GetTextHeight() + wordSpace * ratio;
            height += i_height;
        };
        std::for_each(legendItems_.begin(), legendItems_.end(), cal);
        height += (itemMarginBottom - wordSpace) * ratio;
    } else {
        if(legendItems_.size() > 0) {
            LegendItem &item = legendItems_[0];
            auto text = xg::make_unique<shape::Text>(item.name, util::Point(0, 0), textSize * ratio, "", "");
            xg::util::BBox textBbox = text->GetBBox(chart.GetCanvasContext());
            int line_num = ceil(static_cast<float>(legendItems_.size()) / static_cast<float>(horizontalItems));
            height += (textBbox.height + lineBottom * ratio) * line_num + (itemMarginBottom - lineBottom) * ratio;
        }
    }
    height_ = height;
    return height_;
}

void legend::LegendController::AddLegend(XChart &chart, const std::string &field, const std::vector<legend::LegendItem> &fieldItems) {

    nlohmann::json &fieldLegendCfg = legendCfg_;

    if(this->legendCfg_.contains(field)) {
        fieldLegendCfg = legendCfg_[field];
    }

    if(fieldLegendCfg.is_boolean() && fieldLegendCfg == false) {
        return;
    }
    // if custom { // TODO 待实现 }
    // else if category
    if(scale::IsCategory(chart.GetScale(field).GetType())) {
        nlohmann::json lastCfg;
        lastCfg.merge_patch(DefaultLegendConfig()[position_]);
        if(fieldLegendCfg.is_object()) {
            lastCfg.merge_patch(fieldLegendCfg);
        }
        double maxLength = (position_ == "right" || position_ == "left") ? chart.GetHeight() : chart.GetWidth();
        lastCfg["maxLength"] = maxLength;

        Legend legend(field, std::move(lastCfg), fieldItems);

        if(legends_.find(position_) == legends_.end()) {
            legends_[position_] = std::vector<Legend>{};
        }
        legends_[position_].push_back(std::move(legend));
    }
    // cal Size
    std::vector<Legend> &legends = legends_[position_];
    for(size_t i = 0; i < legends.size(); i++) {
        Legend &legend = legends[i];
        float width_i = legend.CalLegendWidth(chart);
        float height_i = legend.CalLegendHeight(chart);
        legendWidth_ = fmax(legendWidth_, width_i);
        legendHeight_ = fmax(legendHeight_, height_i);
    }
}

void legend::LegendController::SetFieldConfig(std::string field, nlohmann::json cfg) {
    if(cfg["position"].is_string()) {
        position_ = cfg["position"];
    }
    if(cfg["enable"].is_boolean()) {
        enable_ = cfg["enable"];
    }
    legendCfg_[field] = cfg;
}

void legend::LegendController::Render(XChart &chart, shape::Group *container) {
    if(!this->enable_)
        return;

    if(chart.GetCoord().GetType() == xg::canvas::coord::CoordType::Polar) {
        position_ = "right";
    }

    // only support category

    std::map<std::string, std::vector<legend::LegendItem>> legendItems = chart.GetLegendItems();
    if(legendItems.empty())
        return;

    // init legend

    for(auto it = legendItems.begin(); it != legendItems.end(); ++it) {
        std::string field = it->first;
        std::vector<legend::LegendItem> &fieldItems = it->second;

        std::for_each(fieldItems.begin(), fieldItems.end(), [&](legend::LegendItem &legendItem) -> void {
            if(markerItems_.is_array() && markerItems_.size() > 0) {
                for(std::size_t index = 0; index < markerItems_.size(); ++index) {
                    nlohmann::json &item = markerItems_[index];
                    if(item.contains("name") && item.contains("value") && legendItem.name == item["name"]) {
                        legendItem.value = item["value"];
                        break;
                    }
                }
            }
        });

        this->AddLegend(chart, field, fieldItems);
    }

    // draw
    std::vector<Legend> &legends = legends_[position_];
    for(size_t i = 0; i < legends.size(); i++) {
        Legend &legend = legends[i];
        Point originPoint = legend.AlignLegend(chart, position_, legendHeight_, legendWidth_);
        legend.CreateShape(chart, container, originPoint);
    }

    // calculate legend range
    float ratio = chart.GetRatio();
    float left = 0;
    float top = 0;
    float right = 0;
    float bottom = 0;

    std::array<double, 4> &userPadding = chart.padding_;

    if(position_ == "top") {
        top = legendHeight_;
    } else if(position_ == "bottom") {
        bottom = legendHeight_;
    } else if(position_ == "left") {
        left = legendWidth_;
    } else {
        right = legendWidth_;
    }

    chart.UpdateLayout({userPadding[0] + left, userPadding[1] + top, userPadding[2] + right, userPadding[3] + bottom});
}

void legend::LegendController::OnToolTipMarkerItemsChanged(nlohmann::json &items) {
    if(!this->enable_)
        return;

    markerItems_ = items;
}

void legend::LegendController::ClearInner() { legends_.clear(); }
