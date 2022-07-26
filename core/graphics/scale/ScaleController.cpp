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

array<float, 2> xg::scale::AdjustRange(const nlohmann::json &fieldColumn, std::unique_ptr<canvas::coord::AbstractCoord> &coord) {
    array<float, 2> cfg = {NAN, NAN};
    const std::size_t size = fieldColumn.size();
    if(size <= 1) {
        cfg = {0.5, 1.0};
    } else {
        if(coord->GetType() == canvas::coord::CoordType::Polar) {
            if(!coord->IsTransposed()) {
                cfg = {0, 1.0f - 1.0f / static_cast<float>(size)};
            } else {
                float offset = 1.0f / static_cast<float>(size) * 3.0 / 4.0;
                cfg = {offset / 2.0f, 1.0f - offset / 2.0f};
            }
        }
    }
    return cfg;
}

std::unique_ptr<AbstractScale> xg::scale::MakeCategory(const std::string &field_,
                                                    const nlohmann::json &data,
                                                    const ScaleCfg &config,
                                                    utils::Tracer *tracer,
                                                    std::unique_ptr<canvas::coord::AbstractCoord> &coord,
                                                    const nlohmann::json &fieldColumn) {
    tracer->trace("MakeScale: %s, return Category. ", field_.c_str());
    auto cat = xg::make_unique<Category>(field_, fieldColumn, config);
    auto cfg = cat->GetConfig();
    cfg.range = AdjustRange(fieldColumn, coord);
    cat->Change(cfg);
    return cat;
}

std::unique_ptr<AbstractScale> xg::scale::MakeLinear(const std::string &field_,
                                                const nlohmann::json &data,
                                                const ScaleCfg &config,
                                                utils::Tracer *tracer,
                                                std::unique_ptr<canvas::coord::AbstractCoord> &coord,
                                                const nlohmann::json &fieldColumn) {
    tracer->trace("MakeScale: %s, return Linear. ", field_.c_str());
    return xg::make_unique<Linear>(field_, fieldColumn, config);
}


std::unique_ptr<AbstractScale> xg::scale::MakeScale(const std::string &field_,
                                                const nlohmann::json &data,
                                                const ScaleCfg &config,
                                                utils::Tracer *tracer,
                                                std::unique_ptr<canvas::coord::AbstractCoord> &coord) {

    if(!data.is_array() || data.size() <= 0) {
        tracer->trace("MakeScale: %s, return Identity. data is empty", field_.c_str());
        return xg::make_unique<Identity>(field_, nlohmann::json());
    }

    nlohmann::json firstObj = data[0];
    nlohmann::json firstVal = firstObj[field_];
    //如果field_对应的数据是数组，会把数组打平
    nlohmann::json fieldColumn = util::JsonArrayByKey(data, field_);

    if(!config.type.empty()) {
        const std::string &type = config.type;
        if(type == "timeCat") {
            tracer->trace("MakeScale: %s, return TimeCategory. ", field_.c_str());
            return xg::make_unique<TimeCategory>(field_, fieldColumn, config);
        } else if(type == "timeSharing") {
            tracer->trace("MakeScale: %s, return TimeSharingLinear. ", field_.c_str());
            return xg::make_unique<TimeSharingLinear>(field_, fieldColumn, config);
        } else if(type.substr(0, 6) == "kline-") {
            tracer->trace("MakeScale: %s, return KlineCat. ", field_.c_str());
            return xg::make_unique<KLineCat>(field_, fieldColumn, config);
        } else if(type == "cat") {
            return MakeCategory(field_, data, config, tracer, coord, fieldColumn);
        } else if(type == "linear") {
            return MakeLinear(field_, data, config, tracer, coord, fieldColumn);
        }
    }

    if(firstVal.is_string()) {
        return MakeCategory(field_, data, config, tracer, coord, fieldColumn);
    } else if(firstVal.is_number() || firstVal.is_array()) {
        return MakeLinear(field_, data, config, tracer, coord, fieldColumn);
    }

    tracer->trace("MakeScale: %s, return default Identity. ", field_.c_str());
    return xg::make_unique<Identity>(field_, fieldColumn);
}


const std::unique_ptr<AbstractScale> &scale::ScaleController::CreateScale(const std::string &field_,
                                                  const nlohmann::json &data,
                                                  utils::Tracer *tracer,
                                                  std::unique_ptr<canvas::coord::AbstractCoord> &coord) {
    if(!scales_.empty() && scales_.count(field_) > 0) {
        return scales_[field_];
    }
    
    std::unique_ptr<AbstractScale> _target = MakeScale(field_, data, colConfigs[field_], tracer, coord);
    scales_[field_] = std::move(_target);
    return scales_[field_];
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
    double rangeMin = DBL_MAX, rangeMax = DBL_MIN;
    std::for_each(chart_->GetGeoms().begin(), chart_->GetGeoms().end(), [&](auto &geom) -> void {
        util::JsonRangeInGeomDataArray(geom->GetDataArray(), geom->GetYScaleField(), valueStart, valueEnd, &rangeMin, &rangeMax);
    });

    std::for_each(chart_->GetGeoms().begin(), chart_->GetGeoms().end(), [&](auto &geom) -> void {
        auto &scale = chart_->GetScale(geom->GetYScaleField());
        auto config = scale.GetConfig();
        config.min = rangeMin;
        config.max = rangeMax;
        UpdateScale(geom->GetYScaleField(), config);
    });
}
    
void scale::ScaleController::UpdateScale(const std::string &field, const ScaleCfg &cfg) {
    auto &scale = chart_->GetScale(field);
    scale.Change(cfg);
}

void scale::ScaleController::AdjustScale() {
    if (chart_->GetCoord().GetType() == CoordType::Polar) {
        return;
    }
    std::for_each(chart_->GetGeoms().begin(), chart_->GetGeoms().end(), [&](auto &geom) -> void {
        if (geom->GetType() == "interval") {
            auto &yScale = chart_->GetScale(geom->GetYScaleField());
            auto yConfig = yScale.GetConfig();
            if (yScale.GetMin() > 0) {
                yConfig.min = 0;
                yScale.Change(yConfig);
            } else if(yScale.GetMax() < 0) {
                yConfig.max = 0;
                yScale.Change(yConfig);
            }
        }
    });
}

void scale::ScaleController::AdjustRange() {
    if (chart_->GetCoord().GetType() == CoordType::Polar) {
        return;
    }
    std::for_each(chart_->GetGeoms().begin(), chart_->GetGeoms().end(), [&](auto &geom) -> void {
        if (geom->GetType() == "interval") {
            auto &xScale = chart_->GetScale(geom->GetXScaleField());
            auto xConfig = xScale.GetConfig();
            if (!xConfig.HasRange()) {
                xConfig.range = {0.1, 0.9};
                xScale.Change(xConfig);
            }
        }
    });
}
