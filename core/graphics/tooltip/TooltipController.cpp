#include "TooltipController.h"

#include "graphics/XChart.h"
#include "graphics/func/Func.h"
#include "graphics/global.h"
#include "utils/xtime.h"

using namespace xg;

namespace xg {
namespace tooltip {

static scale::AbstractScale &_GetToolTipValueScale(XChart *chart, std::unique_ptr<geom::AbstractGeom> &geom) {
    auto &attr = geom->GetAttr(attr::AttrType::Color);
    if(attr.get() != nullptr) {
        auto &scale = chart->GetScale(attr->GetFields()[0]);
        if(scale::IsLinear(scale.GetType())) {
            return scale;
        }
    }

    return chart->GetScale(geom->GetYScaleField());
}

static scale::AbstractScale &_GetToolTipGroupScale(XChart *chart, std::unique_ptr<geom::AbstractGeom> &geom) {
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

    container_ = chart_->frontLayout_->AddGroup();
}

tooltip::ToolTipController::~ToolTipController() {
    actionListeners_.clear();
    container_ = nullptr;
}

void tooltip::ToolTipController::Init(const nlohmann::json &cfg) {
    nlohmann::json defaultConfig = {
        {"alwaysShow", false},
        {"showTitle", false},
        {"showCrosshairs", false},
        {"crosshairsStyle", {{"stroke", "#1890FF"}, {"lineWidth", 1}, {"type", "dash"}, {"dash", {4, 4}}}},
        {"showTooltipMarker", true},
        {"background", {{"radius", 1}, {"fill", "#1890FF"}, {"padding", {3, 3}}}},
        {"xTip", {{"fontSize", 10}, {"fill", "#ffffff"}, {"textAlign", "center"}, {"textBaseline", "bottom"}}},
        {"yTip", {{"fontSize", 10}, {"fill", "#ffffff"}, {"textAlign", "center"}, {"textBaseline", "bottom"}}},
        {"titleStyle", {{"fontSize", 12}, {"fill", "#ffffff"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
        {"nameStyle", {{"fontSize", 12}, {"fill", "#ffffffa6"}, {"textAlign", "start"}, {"textBaseline", "top"}}},
        {"valueStyle", {{"fontSize", 12}, {"fill", "#ffffff"}, {"textAlign", "start"}, {"textBaseline", "middle"}}},
        {"showItemMarker", true},
        {"itemMarkerStyle", {{"radius", 3}, {"symbol", "circle"}, {"lineWidth", 1}, {"stroke", "#ffffff"}}},
        {"layout", "horizontal"},
        {"onPress", false},
        {"snap", false}};

    this->config_.merge_patch(defaultConfig);
    if(cfg.is_object()) {
        this->config_.merge_patch(cfg);
    }
}

bool tooltip::ToolTipController::OnPressStart(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPressStart");
    return false;
}

bool tooltip::ToolTipController::OnPress(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPress");
    if(this->toolTip_.get() == nullptr)
        return false;

    util::Point point = event.points[0];
    bool alwaysShow = config_["alwaysShow"];
    if(!chart_->coord_->IsContains(point.x, point.y)) {
        if(!alwaysShow) {
            return this->HideToolTip();
        }
        return false;
    }

    long currentTime = xg::CurrentTimestampAtMM();
    if(currentTime - lastShowTimeStamp_ > 16) {
        bool ret = this->ShowToolTip(point);
        this->lastShowTimeStamp_ = currentTime;
        return ret;
    }
    return false;
}
bool tooltip::ToolTipController::OnPressEnd(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPressend");
    if(this->toolTip_.get() == nullptr) {
        return false;
    }

    return this->HideToolTip();
}

void tooltip::ToolTipController::OnRender() {
    if(this->toolTip_.get() != nullptr)
        return;

    this->config_["maxLength"] = chart_->coord_->GetWidth();
    // TODO contains [line, area, path, point]
    config_["showCrosshairs"] = true;
    this->toolTip_ = std::make_unique<ToolTip>(container_, config_);
}

void tooltip::ToolTipController::OnClearInner() { this->container_->Clear(); }

bool tooltip::ToolTipController::ShowToolTip(const util::Point &point) {

    util::Point _point = point;

    auto timestampStart = xg::CurrentTimestampAtMM();
    nlohmann::json tooltipMarkerItems;
    std::for_each(chart_->geoms_.begin(), chart_->geoms_.end(), [&](auto &geom) -> void {
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

                _point.x = record["_x"];
            }
        }
    });

    if(tooltipMarkerItems.size() == 0) {
        return false;
    }

    this->container_->Clear();

    nlohmann::json &first = tooltipMarkerItems[0];
    if(chart_->GetCoord().IsTransposed()) {

    } else {
        util::Point xTipPoint = _point;
        xTipPoint.y = chart_->GetCoord().GetYAxis().x;
        toolTip_->SetXTipContent(*chart_->canvasContext_, first["title"], xTipPoint);
    }

    this->toolTip_->SetPosition(chart_->GetCoord(), *chart_->canvasContext_, _point, tooltipMarkerItems, InvertYTip(_point));

    this->toolTip_->Show(*chart_->canvasContext_);

    std::for_each(actionListeners_.begin(), actionListeners_.end(),
                  [&](ToolTipMarkerItemsCallback callback) -> void { callback(tooltipMarkerItems); });

    chart_->Redraw();

    if(config_.contains("onPress") && config_["onPress"].is_string()) {
        auto onPressCallback = func::FunctionManager::GetInstance().Find(config_["onPress"]);
        if(onPressCallback != nullptr) {
            onPressCallback->Execute(tooltipMarkerItems);
        }
    }

    chart_->GetLogTracer()->trace("ShowToolTip duration: %lums", (xg::CurrentTimestampAtMM() - timestampStart));
    return true;
}

bool tooltip::ToolTipController::HideToolTip() {
    nlohmann::json emptyData;
    std::for_each(actionListeners_.begin(), actionListeners_.end(),
                  [&](ToolTipMarkerItemsCallback callback) -> void { callback(emptyData); });
    container_->Clear();
    chart_->Redraw();
    return true;
}

std::string tooltip::ToolTipController::InvertYTip(const util::Point &p) {
    util::Point invertPoint = chart_->GetCoord().InvertPoint(p);
    auto &yScale = chart_->GetScale(chart_->getYScaleFields()[0]);
    nlohmann::json yVal = yScale.Invert(invertPoint.y);
    return yScale.GetTickText(yVal);
}
