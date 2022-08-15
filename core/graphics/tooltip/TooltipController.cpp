#include "TooltipController.h"
#include "../XChart.h"
#include "../func/Func.h"
#include "../global.h"
#include "../../utils/xtime.h"

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

void tooltip::ToolTipController::Init(const ToolTipCfg &cfg) {
    config_ = cfg;
}

bool tooltip::ToolTipController::OnPressStart(event::Event &event) {
    chart_->GetLogTracer()->trace("%s", "#ToolTipController onPressStart");
    if(!config_.onPressStart.empty()) {
        chart_->InvokeFunction(config_.onPressStart);
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

    if(!config_.onPressEnd.empty()) {
        chart_->InvokeFunction(config_.onPressEnd);
    }

    if(this->toolTip_.get() == nullptr) {
        return false;
    }

    return this->HideToolTip();
}

void tooltip::ToolTipController::OnRender() {
    if(this->toolTip_.get() != nullptr)
        return;
    this->toolTip_ = std::make_unique<ToolTip>(container_, config_);
}

void tooltip::ToolTipController::OnClearInner() { this->container_->Clear(); }

bool tooltip::ToolTipController::ShowToolTip(const util::Point &point) {

    util::Point _point = point;
    
    //限制point.x的坐标为数据点的最后一个坐标
    double maxPointX = FLT_MIN;
    double minPointX = FLT_MAX;
    auto &geom = chart_->geoms_.front();
    auto &firstRecord = geom->GetFirstSnapRecord(chart_);
    auto &lastRecord = geom->GetLastSnapRecord(chart_);
    //在各分组中取最大的 当scale为timesharing的时候，first和last可能对调，所以min和max判断都需要
    if (!std::isnan(lastRecord._x)) {
        minPointX = fmin(minPointX, lastRecord._x);
        maxPointX = fmax(maxPointX, lastRecord._x);
    }
    
    if (!std::isnan(firstRecord._x)) {
        minPointX = fmin(minPointX, firstRecord._x);
        maxPointX = fmax(maxPointX, firstRecord._x);
    }
    _point.x = fmax(fmin(_point.x, maxPointX), minPointX);
    
    auto timestampStart = xg::CurrentTimestampAtMM();
    ToolTipItemList tooltipMarkerItems;
    std::for_each(chart_->geoms_.begin(), chart_->geoms_.end(), [&](auto &geom) -> void {
        // TODO geom is visible
        auto records = geom->GetSnapRecords(chart_, _point);
        for(std::size_t index = 0; index < records.size(); ++index) {
            auto &record = records[index];
            if(!std::isnan(record._x) ) {
                ToolTipItem tooltipItem;
                tooltipItem.x = record._x;
                tooltipItem.y = record._y0.empty() ? record._y : record._y0[0];
                tooltipItem.color = record._color;
                tooltipItem.xTip = config_.xTip;
                tooltipItem.yTip = config_.yTip;

                auto &nameField = geom->GetYScaleField();
                auto &yScale = chart_->GetScale(nameField);
                tooltipItem.name = nameField;

                _point.x = record._x;
                tooltipItem.value = InvertYTip(_point, yScale);
                tooltipItem.title = chart_->GetScale(geom->GetXScaleField()).GetTickText(record.data[geom->GetXScaleField()], chart_);
                
                tooltipItem.touchX = _point.x;
                tooltipItem.touchY = _point.y;
                tooltipMarkerItems.tooltip.push_back(tooltipItem);
            }
        }
    });

    if(tooltipMarkerItems.tooltip.size() == 0) {
        chart_->GetLogTracer()->trace("%s", "#ToolTipController ShowToolTip interrupted with tooltipMarkerItems is empty.");
        return false;
    }

    this->container_->Clear();

    if(!config_.onPress.empty()) {
        //转换成map
        string param = tooltipMarkerItems.ToJson();
        
        auto rst = chart_->InvokeFunction(config_.onPress, param);
        if (rst.count("tooltip")) {
            auto &tooltips = rst.at("tooltip").Cast<vector<Any> &>();
            tooltipMarkerItems.tooltip.clear();
//            for (auto &item : tooltips) {
//                auto obj = json::FromMapByType(item.Cast<unordered_map<string, Any>>(), typeof (ToolTipItem)).Cast<ToolTipItem>();
//                tooltipMarkerItems.tooltip.push_back(std::move(obj));
//            }
        }
    }

    if(!config_.hidden) {
        const ToolTipItem &first = tooltipMarkerItems.tooltip[0];
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
                      [&](ToolTipMarkerItemsCallback callback) -> void {
            callback(); });

        chart_->Redraw();
    }

    chart_->GetLogTracer()->trace("ShowToolTip duration: %lums", (xg::CurrentTimestampAtMM() - timestampStart));
    return true;
}

bool tooltip::ToolTipController::HideToolTip() {
    std::for_each(actionListeners_.begin(), actionListeners_.end(),
                  [&](ToolTipMarkerItemsCallback callback) -> void { callback(); });
    container_->Clear();
    chart_->Redraw();
    return true;
}

std::string tooltip::ToolTipController::InvertYTip(const util::Point &p, xg::scale::AbstractScale &yScale) {
    util::Point invertPoint = chart_->GetCoord().InvertPoint(p);
    Any yVal = yScale.Invert(invertPoint.y);
    return yScale.GetTickText(yVal, chart_);
}
