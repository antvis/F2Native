//
//  ScaleController.cpp
//  F2
//
//  Created by weiqing.twq on 2022/6/1.
//

#include "ScaleController.h"
#include "../XChart.h"

using namespace xg;
using namespace xg::scale;

const std::unique_ptr<AbstractScale> &scale::ScaleController::CreateScale(const std::string &field_,
                                                  const nlohmann::json &data,
                                                  utils::Tracer *tracer,
                                                  std::unique_ptr<canvas::coord::AbstractCoord> &coord) {
    nlohmann::json fieldConfig = json::Get(colConfigs, field_);
    if(!scales_.empty()) {
        std::string _key = field_;
        if(fieldConfig.contains("assign")) {
            _key = fieldConfig["assign"];
        }

        std::vector<std::unique_ptr<AbstractScale>>::iterator it =
            std::find_if(scales_.begin(), scales_.end(),
                         [&](const std::unique_ptr<AbstractScale> &item) { return (item->field == _key); });

        if(it != scales_.end()) {
            // TODO 更新 scale config, 暂时没有什么 config
            return (*it);
        }
    }
    std::unique_ptr<AbstractScale> _target = MakeScale(field_, data, colConfigs[field_], tracer, coord);
    scales_.push_back(std::move(_target));
    return scales_[scales_.size() - 1];
}

void scale::ScaleController::SyncYScale() {
    auto &xScale = chart_->GetScale(chart_->GetXScaleField());
    size_t valueStart = 0;
    size_t valueEnd = xScale.values.size();
//    const auto &domain = json::GetArray(chart_->GetSyncYConfig(), "domain");
//    if (domain.size() == 2) {
//        valueStart = domain[0];
//        valueEnd = domain[1];
//    }
    SyncYScale(valueStart, valueEnd, true);
}
    
void scale::ScaleController::SyncYScale(const size_t valueStart, const size_t valueEnd, bool nice) {
    if (chart_->GetCoord().GetType() == CoordType::Polar ||
        chart_->GetGeoms().size() == 1) {
        return;
    }
//    double rangeMin = DBL_MAX, rangeMax = DBL_MIN;
//    std::for_each(chart_->GetGeoms().begin(), chart_->GetGeoms().end(), [&](auto &geom) -> void {
//        util::JsonRangeInGeomDataArray(geom->GetDataArray(), geom->GetYScaleField(), valueStart, valueEnd, &rangeMin, &rangeMax);
//    });
//    std::for_each(chart_->GetGeoms().begin(), chart_->GetGeoms().end(), [&](auto &geom) -> void {
//        UpdateScale(geom->GetYScaleField(), {{"min", rangeMin}, {"max", rangeMax}});
//    });
}
    
void scale::ScaleController::UpdateScale(const std::string &field, const nlohmann::json &cfg) {
    auto &scale = chart_->GetScale(field);
    scale.Change(cfg);
}

void scale::ScaleController::AdjustScale() {
    if (chart_->GetCoord().GetType() == CoordType::Polar) {
        return;
    }
    std::for_each(chart_->GetGeoms().begin(), chart_->GetGeoms().end(), [&](auto &geom) -> void {
        if (geom->GetType() == "interval") {
            auto &xScale = chart_->GetScale(geom->GetYScaleField());
            if (xScale.min > 0) {
                xScale.Change({{"min", 0}});
            } else if(xScale.max < 0) {
                xScale.Change({{"max", 0}});
            }
            
            auto &yScale = chart_->GetScale(geom->GetXScaleField());
            yScale.Change({{"range", {0.1, 0.9}}});
        }
    });
}
