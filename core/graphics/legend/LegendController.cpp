#include <map>
#include <math.h>
#include "LegendController.h"
#include "../XChart.h"
#include "../scale/Scale.h"

using namespace xg;

void legend::Legend::CreateShape(XChart &chart, shape::Group *container, const util::Point &originPoint) {
    // draw
    util::Point currPoint = Point(originPoint.x, originPoint.y);
    nlohmann::json &nameStyle = cfg_["nameStyle"];
    float ratio = chart.GetCanvasContext().GetDevicePixelRatio();
    float wordSpace = cfg_["wordSpace"];
    float itemGap = cfg_["itemGap"];
    float textSize = nameStyle["textSize"];
    float r_itemGap = itemGap * ratio;
    int horizontalItems = cfg_["horizontalItems"];
    float step = CalLegendStep(chart);
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
                chart.GetWidth() - legendWidth - chart.GetMargin()[2] - chart.GetPadding()[2] + itemMarginBottom * chart.GetCanvasContext().GetDevicePixelRatio();
        y = (chart.GetHeight() - legendHeight) / 2;
        if(verticalAlign == "top") {
            y = chart.GetMargin()[1] + chart.GetPadding()[1];
        } else if(verticalAlign == "bottom") {
            y = chart.GetHeight() - legendHeight;
        }
    } else {
        y = (position == "top") ? chart.GetPadding()[1] : chart.GetPadding()[1] + chart.GetHeight() - legendHeight;
        if (align == "left") {
            x = chart.GetMargin()[0] + chart.GetPadding()[0];
        } else if(align == "right") {
            x = chart.GetWidth() - legendWidth - chart.GetMargin()[2] - chart.GetPadding()[2];
        } else { //center
            x = chart.GetMargin()[0] + chart.GetPadding()[0] + legendWidth/2;
        }
    }
    return Point(x, y);
}

float legend::Legend::CalLegendWidth(XChart &chart) {

    float width = 0;
    float ratio = chart.GetCanvasContext().GetDevicePixelRatio();
    nlohmann::json &nameStyle = cfg_["nameStyle"];
    float wordSpace = cfg_["wordSpace"];
    float textSize = nameStyle["textSize"];
    float itemMarginBottom = cfg_["itemMarginBottom"];
    auto step = CalLegendStep(chart);

    auto cal = [&](LegendItem &item) {
        float r_radius = item.marker.radius * ratio;
        auto text = xg::make_unique<shape::Text>(item.name, util::Point(0, 0), textSize * ratio, "", "");
        float t_width = text->GetTextWidth(chart.GetCanvasContext());
        float i_width = r_radius * 2 + wordSpace * ratio + t_width;
        width = fmax(i_width, width);
    };
    std::for_each(legendItems_.begin(), legendItems_.end(), cal);
    if (cfg_["layout"] == "vertical") {
        width += itemMarginBottom * ratio;
    } else {
        width += (legendItems_.size() - 1) * step;
    }

    width_ = width;
    return width_;
}

float legend::Legend::CalLegendHeight(XChart &chart) {
    float height = 0.f;
    float ratio = chart.GetCanvasContext().GetDevicePixelRatio();
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

float legend::Legend::CalLegendStep(XChart &chart) {
    int horizontalItems = cfg_["horizontalItems"];
    auto width = chart.GetWidth() - chart.GetPadding()[0] - chart.GetMargin()[0] - chart.GetPadding()[2] - chart.GetMargin()[2];
    float step = width / horizontalItems;
    return step;
}

void legend::LegendController::AddLegend(XChart &chart, const std::string &field, const std::vector<legend::LegendItem> &fieldItems) {

    nlohmann::json &fieldLegendCfg = legendCfg_;

    if(this->legendCfg_.contains(field)) {
        fieldLegendCfg = legendCfg_[field];
    }

    if(fieldLegendCfg.is_boolean() && fieldLegendCfg == false || !fieldLegendCfg.is_object()) {
        return;
    }
    // if custom { // TODO 待实现 }
    // else if category
    if(scale::IsCategory(chart.GetScale(field).GetType())) {
        nlohmann::json lastCfg = DefaultLegendConfig()[position_];
        lastCfg.merge_patch(fieldLegendCfg);
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
    if(cfg.contains("position") && cfg["position"].is_string()) {
        position_ = cfg["position"];
    }
    if(cfg.contains("enable") && cfg["enable"].is_boolean()) {
        enable_ = cfg["enable"];
    }
    legendCfg_[field] = cfg;
}

void legend::LegendController::Render(XChart &chart) {
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
        legend.CreateShape(chart, container_, originPoint);
    }

    // calculate legend range
    float ratio = chart.GetCanvasContext().GetDevicePixelRatio();
    float left = 0;
    float top = 0;
    float right = 0;
    float bottom = 0;

    std::array<double, 4> &userPadding = chart.padding_;

    auto width = chart.GetWidth() - chart.GetPadding()[0] - chart.GetMargin()[0] - chart.GetPadding()[2] - chart.GetMargin()[2];
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

    std::vector<Legend> &legends = legends_[position_];

    for(size_t i = 0; i < legends.size(); i++) {
        Legend &legend = legends[i];
        if(markerItems_.empty() || markerItems_.size() != legend.legendItems_.size()) {
            for(std::size_t index = 0; index < legend.legendItems_.size(); ++index) {
                legend.legendItems_[index].value = "";
            }
            continue;
        }

        for(std::size_t index = 0; index < legend.legendItems_.size(); ++index) {
            nlohmann::json &item = markerItems_[index];
            LegendItem &lengedItem = legend.legendItems_[index];
            if(item.contains("name") && item.contains("value") && lengedItem.name == item["name"]) {
                lengedItem.value = item["value"];
            }
        }
    }
}

void legend::LegendController::Redraw(XChart &chart) {
    container_->Clear();

    // draw
    std::vector<Legend> &legends = legends_[position_];
    for(size_t i = 0; i < legends.size(); i++) {
        Legend &legend = legends[i];
        Point originPoint = legend.AlignLegend(chart, position_, legendHeight_, legendWidth_);
        legend.CreateShape(chart, container_, originPoint);
    }
}

void legend::LegendController::ClearInner() {
    container_->Clear();
    legends_.clear();
}
