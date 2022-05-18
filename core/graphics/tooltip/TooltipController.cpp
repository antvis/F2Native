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

// 该函数并不是一个常规的实现，而是用简单地协议把结果拼接成字符串。谨慎使用
// if geomIndex == -1, then the tooltip info of all geoms will be returned
// eg. geomIndex == 0, returns "valueX:1634925427968;valueY:35657.786836;touchX:694.545288;touchY:345.000000"
//     geomIndex == -1, returns "valueX:1634925427968;valueY:35657.786836;touchX:694.545288;touchY:345.000000#valueX:1634925427968;valueY:35657.786836;touchX:694.545288;touchY:345.000000"
// "#" separates tooltip info of different geom
// ";" separates tooltip attributes of a tooltip
// ":" separates key and value of an attribute
const std::string tooltip::ToolTipController::GetTooltipInfos(float touchX, float touchY, int geomIndex) {
    if (chart_ == nullptr) {
        return "";
    }

    //chart_->logTracer_->trace("%s", "TooltipController:GetTooltipInfos()");
    if (chart_->geoms_.size() <= 0 || geomIndex < 0 || geomIndex >= chart_->geoms_.size()) {
        return "";
    }
    util::Point point;
    point.x = touchX;
    point.y = touchY;
    chart_->logTracer_->trace("TooltipController:GetTooltipInfos:pos(%f,%f),geomIndex %d",
                            (float)point.x, (float)point.y, geomIndex);

    //限制point.x的坐标为数据点的最后一个坐标
    double maxPointX = FLT_MIN;
    double minPointX = FLT_MAX;
    if (chart_->geoms_.size() > 0 && geomIndex >= 0 && geomIndex < chart_->geoms_.size()) {
        const nlohmann::json &firstRecords = chart_->geoms_[geomIndex]->GetFirstSnapRecord(chart_);
        if (firstRecords.is_array() && firstRecords.size() > 0) {
            for(std::size_t index = 0; index < firstRecords.size(); ++index) {
                const nlohmann::json &firstRecord = firstRecords[index];
                if (firstRecord.contains("_x")) {
                    double firstX = firstRecord["_x"].get<double>();
                    maxPointX = fmax(maxPointX, firstX);
                    minPointX = fmin(minPointX, firstX);
                } else {
                    chart_->logTracer_->trace("TooltipController:GetTooltipInfos:invalid first");
                }
            }
        }
        const nlohmann::json &lastRecords = chart_->geoms_[geomIndex]->GetLastSnapRecord(chart_);
        if (lastRecords.is_array() && lastRecords.size() > 0) {
            for(std::size_t index = 0; index < lastRecords.size(); ++index) {
                const nlohmann::json &lastRecord = lastRecords[index];
                if (lastRecord.contains("_x")) {
                    double lastX = lastRecord["_x"].get<double>();
                    maxPointX = fmax(maxPointX, lastX);
                    minPointX = fmin(minPointX, lastX);
                } else {
                    chart_->logTracer_->trace("TooltipController:GetTooltipInfos:invalid last");
                }
            }
        }
    }
    point.x = fmax(fmin(point.x, maxPointX), minPointX);

    double minPointY = chart_->margin_[1] + chart_->padding_[1];
    double maxPointY = chart_->margin_[1] + chart_->height_ - chart_->padding_[3];
    point.y = fmax(fmin(point.y, maxPointY), minPointY);

    std::string xField = chart_->GetXScaleField();
    std::string yField = chart_->getYScaleFields()[0];
    chart_->logTracer_->trace("TooltipController:GetTooltipInfos xField %s, yField %s", xField.c_str(), yField.c_str());

    scale::AbstractScale &scaleX = chart_->GetScale(xField);
    scale::AbstractScale &scaleY = chart_->GetScale(yField);

    util::Point invertPoint = chart_->GetCoord().InvertPoint(point);
    nlohmann::json xVal = scaleX.Invert(invertPoint.x);
    nlohmann::json yVal = scaleY.Invert(invertPoint.y);

    std::string valueX = xVal.dump();
    std::string valueY = yVal.dump();
    chart_->logTracer_->trace("TooltipController:GetTooltipInfos valueX %s, valueY %s", valueX.c_str(), valueY.c_str());

    // get the x position of selected item
    std::string posX = std::to_string(point.x);
    std::string posY = std::to_string(point.y);

    chart_->logTracer_->trace("TooltipController:GetTooltipInfos:got position info sucess");

    std::string output("valueX:");
    output.append(valueX);
    output.append(";valueY:");
    output.append(valueY);
    output.append(";touchX:");
    output.append(posX);
    output.append(";touchY:");
    output.append(posY);

    return output;
}
