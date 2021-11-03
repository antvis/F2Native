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
    if(attr.get() != nullptr && !attr->GetFields().empty()) {
        auto &scale = chart->GetScale(attr->GetFields()[0]);
        if(scale::IsLinear(scale.GetType())) {
            return scale;
        }
    }

    return chart->GetScale(geom->GetYScaleField());
}

static scale::AbstractScale &_GetToolTipGroupScale(XChart *chart, std::unique_ptr<geom::AbstractGeom> &geom) {
    auto &attr = geom->GetAttr(attr::AttrType::Color);
    if(attr.get() != nullptr && !attr->GetFields().empty()) {
        auto &scale = chart->GetScale(attr->GetFields()[0]);
        return scale;
    }

    return _GetToolTipValueScale(chart, geom);
}

static const std::string &_GetToolTipGroupNameField(XChart *chart, std::unique_ptr<geom::AbstractGeom> &geom) {
    auto &attr = geom->GetAttr(attr::AttrType::Color);
    if(attr.get() != nullptr && !attr->GetFields().empty()) {
        auto &scale = chart->GetScale(attr->GetFields()[0]);
        return scale.field;
    }
    return geom->GetYScaleField();
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
    if(cfg.is_object()) {
        this->config_.merge_patch(cfg);
    }
    chart_->GetLogTracer()->trace("ToolTip Config: %s", config_.dump().data());
}

bool tooltip::ToolTipController::OnPressStart(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPressStart");
    if(config_.contains("onPressStart") && config_["onPressStart"].is_string()) {
        func::InvokeFunction(config_["onPressStart"], {});
    }
    return false;
}

bool tooltip::ToolTipController::OnPress(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPress");
    if(this->toolTip_.get() == nullptr)
        return false;

    util::Point point = event.points[0];
    point.x = fmax(chart_->GetCoord().GetXAxis().x, fmin(chart_->GetCoord().GetXAxis().y, point.x));
    point.y = fmax(chart_->GetCoord().GetYAxis().y, fmin(chart_->GetCoord().GetYAxis().x, point.y));
    //    bool alwaysShow = config_["alwaysShow"];
    //    if(!chart_->coord_->IsContains(point.x, point.y)) {
    //        chart_->GetLogTracer()->trace("%s", "#ToolTipController onPress interrupted.");
    //        if(!alwaysShow) {
    //            return this->HideToolTip();
    //        }
    //        return false;
    //    }

    auto currentTime = xg::CurrentTimestampAtMM();
    auto deltaTime = currentTime - lastShowTimeStamp_;
    if(deltaTime > 32) {
        chart_->GetLogTracer()->trace("%s delta: %lu", "#ToolTipController onPress startShowTooltip", deltaTime);
        bool ret = this->ShowToolTip(point);
        this->lastShowTimeStamp_ = currentTime;
        return ret;
    }
    return false;
}

bool tooltip::ToolTipController::OnPressEnd(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPressend");

    if(config_.contains("onPressEnd") && config_["onPressEnd"].is_string()) {
        func::InvokeFunction(config_["onPressEnd"], {});
    }

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
    
    //限制point.x的坐标为数据点的最后一个坐标
    double maxPointX = FLT_MIN;
    double minPointX = FLT_MAX;
    std::for_each(chart_->geoms_.begin(), chart_->geoms_.end(), [&](auto &geom) -> void {
        const nlohmann::json &lastRecords = geom->GetLastSnapRecord(chart_);
        const nlohmann::json &firstRecords = geom->GetFirstSnapRecord(chart_);
        for(std::size_t index = 0; index < lastRecords.size(); ++index) {
            const nlohmann::json &lastRecord = lastRecords[index];
            const nlohmann::json &firstRecord = firstRecords[index];
            //在各分组中取最大的
            if (lastRecord.contains("_x")) {
                maxPointX = fmax(maxPointX, lastRecord["_x"].get<double>());
                minPointX = fmin(minPointX, lastRecord["_x"].get<double>());
            }
            
            if (firstRecord.contains("_x")) {
                maxPointX = fmax(maxPointX, firstRecord["_x"].get<double>());
                minPointX = fmin(minPointX, firstRecord["_x"].get<double>());
            }
        }
    });
    _point.x = fmax(fmin(_point.x, maxPointX), minPointX);
    
    auto timestampStart = xg::CurrentTimestampAtMM();
    nlohmann::json tooltipMarkerItems;
    std::for_each(chart_->geoms_.begin(), chart_->geoms_.end(), [&](auto &geom) -> void {
        // TODO geom is visible
        nlohmann::json records = geom->GetSnapRecords(chart_, _point);
        for(std::size_t index = 0; index < records.size(); ++index) {
            nlohmann::json &record = records[index];
            if(record.contains("_x") && record.contains("_y")) {
                nlohmann::json tooltipItem;
                tooltipItem["x"] = record["_x"];
                tooltipItem["y"] = record["_y"];
                tooltipItem["color"] = record.contains("_color") ? record["_color"].get<std::string>() : GLOBAL_COLORS[0];
                tooltipItem["xTip"] = config_["xTip"];
                tooltipItem["yTip"] = config_["yTip"];

                auto &nameField = tooltip::_GetToolTipGroupNameField(chart_, geom);
                auto &yScale = chart_->GetScale(nameField);
                tooltipItem["name"] = nameField;

                _point.x = record["_x"];
                tooltipItem["value"] = InvertYTip(_point, yScale);
                tooltipItem["title"] = chart_->GetScale(geom->GetXScaleField()).GetTickText(record[geom->GetXScaleField()]);
                
                tooltipItem["touchX"] = _point.x;
                tooltipItem["touchY"] = _point.y;
                tooltipMarkerItems.push_back(tooltipItem);
            }
        }
    });

    if(tooltipMarkerItems.size() == 0) {
        chart_->GetLogTracer()->trace("%s", "#ToolTipController ShowToolTip interrupted with tooltipMarkerItems is empty.");
        return false;
    }

    this->container_->Clear();

    if(config_.contains("onPress") && config_["onPress"].is_string()) {
        auto rst = func::InvokeFunction(config_["onPress"], tooltipMarkerItems);
        if(rst.is_array() && rst.size() > 0) {
            tooltipMarkerItems = rst;
        }
    }

    if(config_["hidden"] == false) {
        const nlohmann::json &first = tooltipMarkerItems[0];
        if(chart_->GetCoord().IsTransposed()) {
            // TODO
        } else {
            util::Point xTipPoint = _point;
            xTipPoint.y = chart_->GetCoord().GetYAxis().x;
            toolTip_->SetXTipContent(*chart_->canvasContext_, first, xTipPoint, chart_->GetCoord().GetXAxis());
        }

        this->toolTip_->SetPosition(chart_->GetCoord(), *chart_->canvasContext_, _point, first);

        chart_->GetLogTracer()->trace("%s", "#ToolTipController ShowToolTip show tooltip.");
        this->toolTip_->Show(*chart_->canvasContext_, chart_->GetLogTracer());

        std::for_each(actionListeners_.begin(), actionListeners_.end(),
                      [&](ToolTipMarkerItemsCallback callback) -> void { callback(tooltipMarkerItems); });

        chart_->Redraw();
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

std::string tooltip::ToolTipController::InvertYTip(const util::Point &p, xg::scale::AbstractScale &yScale) {
    util::Point invertPoint = chart_->GetCoord().InvertPoint(p);
    nlohmann::json yVal = yScale.Invert(invertPoint.y);
    return yScale.GetTickText(yVal);
}
