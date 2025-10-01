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
    float ratio = chart.GetCanvasContext().GetDevicePixelRatio();
    float wordSpace = cfg_["wordSpace"];
    float itemGap = cfg_["itemGap"];
    float textSize = nameStyle["textSize"];
    float r_itemGap = itemGap * ratio;
    int horizontalItems = cfg_["horizontalItems"];
    float step = width_ / horizontalItems;
    float lineBottom = cfg_["lineBottom"];
    float radius = cfg_["radius"];
    float r_radius = radius * ratio;
    std::string symbol_ =  cfg_["symbol"];
    float width_ = json::GetNumber(cfg_, "width", 0) * ratio;
    float height_ = json::GetNumber(cfg_, "height", 0) * ratio;
    bool showRectSymbol = symbol_ == "rect"  && width_ > 0 && height_ > 0;

    for(size_t i = 0; i < legendItems_.size(); i++) {
        LegendItem &item = legendItems_[i];
        double textPointX;
        double markerPointX;
        if(showRectSymbol){
            textPointX = currPoint.x + width_ + wordSpace * ratio;
            markerPointX = currPoint.x + width_ / 2;
        } else {
            if(symbol_ == "rect"){
                symbol_ = "square";
            }
            textPointX = currPoint.x + r_radius * 2 + wordSpace * ratio;
            markerPointX = currPoint.x + r_radius;
        }
        // text
        std::unique_ptr<xg::shape::Text> text =
                xg::make_unique<xg::shape::Text>(item.name + "\t" + item.value,
                                                 Point(textPointX, currPoint.y) ,
                                                 textSize * ratio, nameStyle["fill"], nameStyle["fill"]);
        if (nameStyle.contains("font")){
            nlohmann::json font = json::GetObject(nameStyle, "font");
            if (!font.is_null()){
                std::string fontStyle = json::GetString(font, "fontStyle", "");
                std::string fontVariant = json::GetString(font, "fontVariant", "");
                std::string fontWeight = json::GetString(font, "fontWeight", "");
                std::string fontFamily = json::GetString(font, "fontFamily", "");
                text->SetTextFont(fontStyle, fontVariant, fontWeight, fontFamily);
            }
        }

        text->SetTextAlign(nameStyle["textAlign"]);
        text->SetTextBaseline(nameStyle["textBaseline"]);
        xg::util::BBox textBbox = text->GetBBox(chart.GetCanvasContext());
        container->AddElement(std::move(text));

        // marker
        util::Point center = Point(markerPointX, currPoint.y + (textBbox.height / 2));
        auto marker = xg::make_unique<xg::shape::Marker>(center, r_radius, util::Size{width_, height_}, item.marker.fill, symbol_);
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
    // 在padding内
    float x = 0.f;
    float y = 0.f;
    float itemMarginBottom = cfg_["itemMarginBottom"];
    // 垂直方向，只有left和right的时候才生效。top、bottom、middle/center
    string verticalAlign = cfg_["verticalAlign"].is_string() ? cfg_["verticalAlign"] : "middle";
    // 水平方向，只有top和bottom的时候才生效，left、right、middle/center
//    string align = cfg_["align"].is_string() ? cfg_["align"] : "left";
    if(position == "left" || position == "right") {
        x = (position == "left") ?
                chart.GetMargin()[0] + chart.GetPadding()[0] :
                chart.GetWidth() - legendWidth - chart.GetMargin()[2] - chart.GetPadding()[2] + itemMarginBottom * chart.GetCanvasContext().GetDevicePixelRatio();
        y = (chart.GetHeight() - legendHeight) / 2;
        if(verticalAlign == "top") {
            y = chart.GetMargin()[1] + chart.GetPadding()[1];
        } else if(verticalAlign == "bottom") {
            y = chart.GetHeight() - chart.GetPadding()[3]- legendHeight;
        }
    } else {
        x = chart.GetPadding()[0];
        y = (position == "top") ? chart.GetPadding()[1] : chart.GetHeight() -chart.GetPadding()[3] - legendHeight;
    }
    return Point(x, y);
}

float legend::Legend::CalLegendWidth(XChart &chart) {

    float width = 0;
    float ratio = chart.GetCanvasContext().GetDevicePixelRatio();
    nlohmann::json &nameStyle = cfg_["nameStyle"];
    // 图形与文字之间的距离
    float wordSpace = cfg_["wordSpace"];
    float textSize = nameStyle["textSize"];
    // 图例与图表之间的距离
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
    float ratio = chart.GetCanvasContext().GetDevicePixelRatio();
    nlohmann::json &nameStyle = cfg_["nameStyle"];
    // 图形与文字之间的距离
    float wordSpace = cfg_["wordSpace"];
    float textSize = nameStyle["textSize"];
    // 图例与图表之间的距离
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
//    float ratio = chart.GetCanvasContext().GetDevicePixelRatio();
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
