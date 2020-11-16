#include "TooltipController.h"

#include "graphics/XChart.h"
#include "graphics/global.h"
#include "utils/xtime.h"

using namespace xg;

namespace xg {
namespace tooltip {

static scale::AbstractScale &_GetToolTipValueScale(XChart *chart, geom::AbstractGeom *geom) {
    auto &attr = geom->GetAttr(attr::AttrType::Color);
    if(attr.get() != nullptr) {
        auto &scale = chart->GetScale(attr->GetFields()[0]);
        if(scale::IsLinear(scale.GetType())) {
            return scale;
        }
    }

    return chart->GetScale(geom->GetYScaleField());
}

static scale::AbstractScale &_GetToolTipGroupScale(XChart *chart, geom::AbstractGeom *geom) {
    auto &attr = geom->GetAttr(attr::AttrType::Color);
    if(attr.get() != nullptr) {
        auto &scale = chart->GetScale(attr->GetFields()[0]);
        return scale;
    }

    return _GetToolTipValueScale(chart, geom);
}
} // namespace tooltip
} // namespace xg

tooltip::ToolTipController::ToolTipController(XChart *chart) : chart_(chart) {
    chart_->eventController_->AddCallback("pressstart", XG_MEMBER_CALLBACK_1(tooltip::ToolTipController::OnPressStart));
    chart_->eventController_->AddCallback("press", XG_MEMBER_CALLBACK_1(tooltip::ToolTipController::OnPress));
    chart_->eventController_->AddCallback("pressend", XG_MEMBER_CALLBACK_1(tooltip::ToolTipController::OnPressEnd));

    chart_->AddMonitor(ACTION_CHART_AFTER_GEOM_DRAW, XG_MEMBER_CALLBACK(tooltip::ToolTipController::OnRender));
    chart_->AddMonitor(ACTION_CHART_CLEAR_INNER, XG_MEMBER_CALLBACK(tooltip::ToolTipController::OnClearInner));
}

tooltip::ToolTipController::~ToolTipController() { actionListeners_.clear(); }

void tooltip::ToolTipController::Init(const nlohmann::json &cfg) {
    nlohmann::json defaultConfig = {
        {"alwaysShow", false},
        {"showTitle", false},
        {"showCrosshairs", false},
        {"crosshairsStyle", {{"stroke", "#00000064"}, {"lineWidth", 1}}},
        {"showTooltipMarker", true},
        {"background", {{"radius", 1}, {"fill", "#000000a6"}, {"padding", {3, 3}}}},
        {"xTip", {{"fontSize", 10}, {"fill", "#ffffff"}, {"textAlign", "center"}, {"textBaseline", "bottom"}}},
        {"yTip", {{"fontSize", 10}, {"fill", "#ffffff"}, {"textAlign", "center"}, {"textBaseline", "bottom"}}},
        {"titleStyle", {{"fontSize", 12}, {"fill", "#ffffff"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
        {"nameStyle", {{"fontSize", 12}, {"fill", "#ffffffa6"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
        {"valueStyle", {{"fontSize", 12}, {"fill", "#ffffff"}, {"textAlign", "start"}, {"textBaseline", "middle"}}},
        {"showItemMarker", true},
        {"itemMarkerStyle", {{"radius", 3}, {"symbol", "circle"}, {"lineWidth", 1}, {"stroke", "#ffffff"}}},
        {"layout", "horizontal"},
        {"snap", false}};

    this->config_.merge_patch(defaultConfig);
    if(cfg.is_object()) {
        this->config_.merge_patch(cfg);
    }
}

void tooltip::ToolTipController::OnPressStart(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPressStart");
}
void tooltip::ToolTipController::OnPress(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPress");
    if(this->toolTip_.get() == nullptr)
        return;

    util::Point point = event.points[0];
    bool alwaysShow = config_["alwaysShow"];
    if(!chart_->coord_->IsContains(point.x, point.y)) {
        if(!alwaysShow) {
            this->HideToolTip();
        }
        return;
    }

    long currentTime = xg::CurrentTimestampAtMM();
    if(currentTime - lastShowTimeStamp_ > 16) {
        this->ShowToolTip(point);
        this->lastShowTimeStamp_ = currentTime;
    }
}
void tooltip::ToolTipController::OnPressEnd(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPressend");
    if(this->toolTip_.get() == nullptr) {
        return;
    }

    this->HideToolTip();
}

void tooltip::ToolTipController::OnRender() {
    if(this->toolTip_.get() != nullptr)
        return;

    this->config_["maxLength"] = chart_->coord_->GetWidth();
    // TODO contains [line, area, path, point]
    config_["showCrosshairs"] = true;
    this->toolTip_ = std::make_unique<ToolTip>(config_);
}

void tooltip::ToolTipController::OnClearInner() {
    if(toolTip_.get() != nullptr) {
        toolTip_->Clear();
    }
}

void tooltip::ToolTipController::ShowToolTip(const util::Point &point) {

    auto timestampStart = xg::CurrentTimestampAtMM();
    nlohmann::json tooltipMarkerItems;
    std::for_each(chart_->geoms_.begin(), chart_->geoms_.end(), [&](geom::AbstractGeom *geom) -> void {
        // TODO geom is visible
        std::string geomType = geom->GetType();
        if(geomType == "interval") {
            return;
        }

        nlohmann::json records = geom->GetSnapRecords(chart_, point);
        for(std::size_t index = 0; index < records.size(); ++index) {
            nlohmann::json &record = records[index];
            if(record.contains("_x") && record.contains("_y")) {
                nlohmann::json tooltipItem;
                tooltipItem["x"] = record["_x"];
                tooltipItem["y"] = record["_y"];
                tooltipItem["color"] = record.contains("_color") ? record["_color"].get<std::string>() : GLOBAL_COLORS[0];

                auto &nameScale = tooltip::_GetToolTipGroupScale(chart_, geom);
                tooltipItem["name"] = nameScale.GetTickText(record[nameScale.field]);
                auto &scale = tooltip::_GetToolTipValueScale(chart_, geom);
                tooltipItem["value"] = scale.GetTickText(record[scale.field]);
                tooltipItem["title"] = chart_->GetScale(geom->GetXScaleField()).GetTickText(record[geom->GetXScaleField()]);

                tooltipMarkerItems.push_back(tooltipItem);
            }
        }
    });

    if(tooltipMarkerItems.size() == 0) {
        return;
    }

    chart_->Repaint();
    auto cmdCount = chart_->canvasContext_->GetRenderCount();

    nlohmann::json &first = tooltipMarkerItems[0];
    if(chart_->GetCoord().IsTransposed()) {

    } else {
        util::Point xTipPoint = point;
        xTipPoint.y = chart_->GetCoord().GetYAxis().x;
        toolTip_->SetXTipContent(*chart_->canvasContext_, first["title"], xTipPoint);
    }

    this->toolTip_->SetPosition(chart_->GetCoord(), *chart_->canvasContext_, point, tooltipMarkerItems);

    this->toolTip_->Show(*chart_->canvasContext_);

    std::for_each(actionListeners_.begin(), actionListeners_.end(),
                  [&](ToolTipMarkerItemsCallback callback) -> void { callback(tooltipMarkerItems); });

    chart_->GetLogTracer()->trace("ToolTip Canvas cmdCount: %ld duration: %ld-ms", (chart_->canvasContext_->GetRenderCount() - cmdCount),
                                  (xg::CurrentTimestampAtMM() - timestampStart));
}

void tooltip::ToolTipController::HideToolTip() {
    nlohmann::json emptyData;
    std::for_each(actionListeners_.begin(), actionListeners_.end(),
                  [&](ToolTipMarkerItemsCallback callback) -> void { callback(emptyData); });
    chart_->Repaint();
}
