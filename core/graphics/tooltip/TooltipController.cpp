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
    configs_.clear();
    actionListeners_.clear();
    container_ = nullptr;
}

void tooltip::ToolTipController::AddConfig(const nlohmann::json &cfg) {
    if (cfg.is_object()) {
        nlohmann::json newCfg = config_;
        newCfg.merge_patch(cfg);
        configs_.push_back(newCfg);
    }
    if (configs_.size() > 0) {
//        chart_->GetLogTracer()->trace("ToolTips Config: %s", configs_.back().dump().data());
    }
}

void tooltip::ToolTipController::Init(const nlohmann::json &cfg) {
    if (cfg.is_object()) {
        this->config_.merge_patch(cfg);
    }
    chart_->GetLogTracer()->trace("ToolTip Config: %s", config_.dump().data());
}

bool tooltip::ToolTipController::OnPressStart(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPressStart");
    if (isTooltips()) {
        if (configs_[0].contains("onPressStart") && configs_[0]["onPressStart"].is_string()) {
            chart_->InvokeFunction(configs_[0]["onPressStart"]);
        }
    } else {
        if (config_.contains("onPressStart") && config_["onPressStart"].is_string()) {
            chart_->InvokeFunction(config_["onPressStart"]);
        }
    }
    return false;
}

bool tooltip::ToolTipController::OnPress(event::Event &event) {
    if (isTooltips()) {
        if (this->toolTips_.size() == 0) {
            return false;
        }
        
        util::Point point = event.points[0];
        point.x = fmax(chart_->GetCoord().GetXAxis().x, fmin(chart_->GetCoord().GetXAxis().y, point.x));
        point.y = fmax(chart_->GetCoord().GetYAxis().y, fmin(chart_->GetCoord().GetYAxis().x, point.y));
        
        auto currentTime = xg::CurrentTimestampAtMM();
        auto deltaTime = currentTime - lastShowTimeStamp_;
        if (deltaTime > 32) {
            chart_->GetLogTracer()->trace("%s delta: %lu", "#ToolTipController onPress startShowTooltips", deltaTime);
            bool ret = this->ShowToolTips(point);
            this->lastShowTimeStamp_ = currentTime;
            return ret;
        }
    } else {
        //    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPress");
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
    }
    return false;
}

bool tooltip::ToolTipController::OnPressEnd(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPressend");

    if (isTooltips()) {
        if (configs_[0].contains("onPressEnd") && configs_[0]["onPressEnd"].is_string()) {
            chart_->InvokeFunction(configs_[0]["onPressEnd"]);
        }
        
        bool ret = false;
        for_each(this->toolTips_.begin(), this->toolTips_.end(), [&](auto &tooltip) {
            if (tooltip.get() != nullptr) {
                ret = true;
            }
        });
        if (ret == false) {
            return false;
        }
    } else {
        if (config_.contains("onPressEnd") && config_["onPressEnd"].is_string()) {
            chart_->InvokeFunction(config_["onPressEnd"]);
        }

        if (this->toolTip_.get() == nullptr) {
            return false;
        }
    }

    return this->HideToolTip();
}

void tooltip::ToolTipController::OnRender() {
    if (isTooltips()) {
        if (this->toolTips_.size() == configs_.size()) {
            return;
        }
        for (std::size_t i = 0; i < configs_.size(); i++) {
            nlohmann::json &config = configs_[i];
            config["maxLength"] = chart_->coord_->GetWidth();
            std::unique_ptr<ToolTip> tooltip = std::make_unique<ToolTip>(container_, config);
            this->toolTips_.push_back(std::move(tooltip));
        }
    } else {
        if (this->toolTip_.get() != nullptr) {
            return;
        }
        this->config_["maxLength"] = chart_->coord_->GetWidth();
        // TODO contains [line, area, path, point]
        config_["showCrosshairs"] = true;
        this->toolTip_ = std::make_unique<ToolTip>(container_, config_);
    }
}

void tooltip::ToolTipController::OnClearInner() { this->container_->Clear(); }

bool tooltip::ToolTipController::ShowToolTips(const util::Point &point) {

    this->container_->Clear();
    
    nlohmann::json tooltipMarkerItemsArray;
    auto timestampStart = xg::CurrentTimestampAtMM();
    for (std::size_t tooltipIdx = 0; tooltipIdx < this->configs_.size(); tooltipIdx++) {
        nlohmann::json &config = this->configs_[tooltipIdx];
        if (!config.contains("geometryIndex")) {
            continue;
        }
        auto &tooltip = this->toolTips_[tooltipIdx];
        auto geometryIndex = json::GetIntNumber(config, "geometryIndex");
        if (chart_->geoms_.size() <= geometryIndex) {
            continue;
        }
        auto &geom = chart_->geoms_[geometryIndex];
        
        std::string xFieldPrefix = geom->isAdjustDodge() ? "original_" : "";
        
        util::Point _point = point;
        
        //限制point.x的坐标为数据点的最后一个坐标
        double maxPointX = FLT_MIN;
        double minPointX = FLT_MAX;
        const nlohmann::json &firstRecord = geom->GetFirstSnapRecord(chart_);
        const nlohmann::json &lastRecord = geom->GetLastSnapRecord(chart_);
        //在各分组中取最大的 当scale为timesharing的时候，first和last可能对调，所以min和max判断都需要
        if (lastRecord.contains("_x")) {
            double lastX = json::GetNumber(lastRecord, "_x");
            minPointX = fmin(minPointX, lastX);
            maxPointX = fmax(maxPointX, lastX);
        }
        
        if (firstRecord.contains("_x")) {
            double firstX = json::GetNumber(firstRecord, "_x");
            minPointX = fmin(minPointX, firstX);
            maxPointX = fmax(maxPointX, firstX);
        }
        _point.x = fmax(fmin(_point.x, maxPointX), minPointX);
        
        nlohmann::json tooltipMarkerItems;
        nlohmann::json records = geom->GetSnapRecords(chart_, _point, false);
        for (std::size_t index = 0; index < records.size(); ++index) {
            nlohmann::json &record = records[index];
            if (record.contains("_x") && record.contains("_y")) {
                nlohmann::json tooltipItem;
                tooltipItem["x"] = record.contains("_original_x") ? record["_original_x"] : record["_x"];
                tooltipItem["y"] = record["_y"];
                tooltipItem["color"] = record.contains("_color") ? record["_color"].get<std::string>() : GLOBAL_COLORS[0];
                tooltipItem["xTip"] = config["xTip"];
                tooltipItem["yTip"] = config["yTip"];
                
                auto &nameField = config.contains("yTipField") ? json::GetString(config, "yTipField") : tooltip::_GetToolTipGroupNameField(chart_, geom);
                auto &yScale = chart_->GetScale(nameField);
                tooltipItem["name"] = nameField;
                
                if (json::GetString(config, "xPositionType") == "record") {
                    _point.x = record.contains("_original_x") ? record["_original_x"] : record["_x"];
                }
                if (json::GetString(config, "yPositionType") == "record") {
                    _point.y = record["_y"];
                }
                tooltipItem["value"] = InvertYTip(_point, yScale);
                auto &xField = geom->GetXScaleField();
                if (xFieldPrefix.length() > 0) {
                    tooltipItem["title"] = chart_->GetScale(xField).GetTickText(record[xFieldPrefix + xField], chart_, index);
                } else {
                    tooltipItem["title"] = chart_->GetScale(xField).GetTickText(record[xField], chart_, index);
                }
                
                tooltipItem["touchX"] = _point.x;
                tooltipItem["touchY"] = _point.y;
                
                tooltipItem["recordIndex"] = json::GetIntNumber(record, "recordIndex", -1);
                
                tooltipItem["tooltipMarkerStyle"] = config["tooltipMarkerStyle"];
                
                tooltipMarkerItems.push_back(tooltipItem);
            }
        }
        
        if (tooltipMarkerItems.size() == 0) {
            chart_->GetLogTracer()->trace("%s", "#ToolTipController ShowToolTips interrupted with tooltipMarkerItems is empty.");
            return false;
        }
        tooltipMarkerItemsArray.push_back(tooltipMarkerItems);

        if (config["hidden"] == false) {
            const nlohmann::json &first = tooltipMarkerItems[0];
            if (chart_->GetCoord().IsTransposed()) {
                // TODO
            } else {
                util::Point xTipPoint = _point;
                xTipPoint.y = chart_->GetCoord().GetYAxis().x;
                auto &xField = geom->GetXScaleField();
                std::string xPosition = chart_->axisController_->GetAxisPosition(xField);
                tooltip->SetXTipContent(*chart_->canvasContext_, first, xTipPoint, chart_->GetCoord().GetXAxis(), xPosition);
            }

            auto &yField = geom->GetYScaleField();
            std::string yPosition = chart_->axisController_->GetAxisPosition(yField);
            tooltip->SetPosition(chart_->GetCoord(), *chart_->canvasContext_, _point, tooltipMarkerItems, yPosition);

            chart_->GetLogTracer()->trace("%s", "#ToolTipController ShowToolTips show tooltip.");
            tooltip->Show(*chart_->canvasContext_, chart_->GetLogTracer());
        }
    }

    if (configs_[0].contains("onPress") && configs_[0]["onPress"].is_string()) {
        nlohmann::json param = {{"tooltips", tooltipMarkerItemsArray}};
        auto rst = xg::json::ParseString(chart_->InvokeFunction(configs_[0]["onPress"], param.dump()));
        tooltipMarkerItemsArray = json::GetArray(rst, "tooltips", tooltipMarkerItemsArray);
    }

    std::for_each(actionListeners_.begin(), actionListeners_.end(),
                  [&](ToolTipMarkerItemsCallback callback) -> void { callback(tooltipMarkerItemsArray); });
    
    chart_->Redraw();

    chart_->GetLogTracer()->trace("ShowToolTips duration: %lums", (xg::CurrentTimestampAtMM() - timestampStart));
    return true;
}

bool tooltip::ToolTipController::ShowToolTip(const util::Point &point) {

    util::Point _point = point;
    
    //限制point.x的坐标为数据点的最后一个坐标
    double maxPointX = FLT_MIN;
    double minPointX = FLT_MAX;
    auto &geom = chart_->geoms_.front();
    const nlohmann::json &firstRecord = geom->GetFirstSnapRecord(chart_);
    const nlohmann::json &lastRecord = geom->GetLastSnapRecord(chart_);
    //在各分组中取最大的 当scale为timesharing的时候，first和last可能对调，所以min和max判断都需要
    if (lastRecord.contains("_x")) {
        double lastX = json::GetNumber(lastRecord, "_x");
        minPointX = fmin(minPointX, lastX);
        maxPointX = fmax(maxPointX, lastX);
    }
    
    if (firstRecord.contains("_x")) {
        double firstX = json::GetNumber(firstRecord, "_x");
        minPointX = fmin(minPointX, firstX);
        maxPointX = fmax(maxPointX, firstX);
    }
    _point.x = fmax(fmin(_point.x, maxPointX), minPointX);
    
    auto timestampStart = xg::CurrentTimestampAtMM();
    nlohmann::json tooltipMarkerItems;
    std::for_each(chart_->geoms_.begin(), chart_->geoms_.end(), [&](auto &geom) -> void {
        std::string xFieldPrefix = geom->isAdjustDodge() ? "original_" : "";
        nlohmann::json records = geom->GetSnapRecords(chart_, _point, false);
        for(std::size_t index = 0; index < records.size(); ++index) {
            nlohmann::json &record = records[index];
            if(record.contains("_x") && record.contains("_y")) {
                nlohmann::json tooltipItem;
                tooltipItem["x"] = record.contains("_original_x") ? record["_original_x"] : record["_x"];
                tooltipItem["y"] = record["_y"];
                tooltipItem["color"] = record.contains("_color") ? record["_color"].get<std::string>() : GLOBAL_COLORS[0];
                tooltipItem["xTip"] = config_["xTip"];
                tooltipItem["yTip"] = config_["yTip"];

                auto &nameField = config_.contains("yTipField") ? json::GetString(config_, "yTipField") : tooltip::_GetToolTipGroupNameField(chart_, geom);
                auto &yScale = chart_->GetScale(nameField);
                tooltipItem["name"] = nameField;

                if (json::GetString(config_, "xPositionType") == "record") {
                    _point.x = record.contains("_original_x") ? record["_original_x"] : record["_x"];
                }
                if (json::GetString(config_, "yPositionType") == "record") {
                    _point.y = record["_y"];
                }
                tooltipItem["value"] = InvertYTip(_point, yScale);
                auto &xField = geom->GetXScaleField();
                if (xFieldPrefix.length() > 0) {
                    tooltipItem["title"] = chart_->GetScale(xField).GetTickText(record[xFieldPrefix + xField], chart_, index);
                } else {
                    tooltipItem["title"] = chart_->GetScale(xField).GetTickText(record[xField], chart_, index);
                }
                
                tooltipItem["touchX"] = _point.x;
                tooltipItem["touchY"] = _point.y;
                
                tooltipItem["recordIndex"] = json::GetIntNumber(record, "recordIndex", -1);
                
                tooltipItem["tooltipMarkerStyle"] = config_["tooltipMarkerStyle"];
                
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
        //转换成map
        nlohmann::json param = {{"tooltip",tooltipMarkerItems}};
        auto rst = xg::json::ParseString(chart_->InvokeFunction(config_["onPress"], param.dump()));
        tooltipMarkerItems = json::GetArray(rst, "tooltip", tooltipMarkerItems);
    }

    if(config_["hidden"] == false) {
        const nlohmann::json &first = tooltipMarkerItems[0];
        if(chart_->GetCoord().IsTransposed()) {
            // TODO
        } else {
            util::Point xTipPoint = _point;
            xTipPoint.y = chart_->GetCoord().GetYAxis().x;
            toolTip_->SetXTipContent(*chart_->canvasContext_, first, xTipPoint, chart_->GetCoord().GetXAxis(), "");
        }

        this->toolTip_->SetPosition(chart_->GetCoord(), *chart_->canvasContext_, _point, tooltipMarkerItems, "left");

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
    return yScale.GetTickText(yVal, chart_, 0);
}
