#ifndef XG_GRAPHICS_SCALE_CONTROLLER_H
#define XG_GRAPHICS_SCALE_CONTROLLER_H

#include <algorithm>
#include <unordered_map>
#include <vector>
#include "Category.h"
#include "Identity.h"
#include "KLineCat.h"
#include "TimeCategory.h"
#include "continuous/Linear.h"
#include "continuous/TimeSharingLinear.h"
#include "../canvas/Coord.h"
#include "../util/json.h"
#include "../util/json_util.h"
#include "../../utils/Tracer.h"
#include "../../utils/common.h"
#include "../../nlohmann/json.hpp"

namespace xg {
namespace scale {

static nlohmann::json AdjustRange(const nlohmann::json &fieldColumn, std::unique_ptr<canvas::coord::AbstractCoord> &coord) {
    nlohmann::json cfg;
    const std::size_t size = fieldColumn.size();
    if(size <= 1) {
        cfg["range"] = {0.5, 1.0};
    } else {
        if(coord->GetType() == canvas::coord::CoordType::Polar) {
            if(!coord->IsTransposed()) {
                cfg["range"] = {0, 1.0 - 1.0 / static_cast<float>(size)};
            } else {
                float offset = 1.0 / static_cast<float>(size) * 3.0 / 4.0;
                cfg["range"] = {offset / 2.0, 1.0 - offset / 2.0};
            }
        }
    }
    return cfg;
}

static std::unique_ptr<AbstractScale> MakeCategory(const std::string &field_,
                                                const nlohmann::json &data,
                                                nlohmann::json &config,
                                                utils::Tracer *tracer,
                                                std::unique_ptr<canvas::coord::AbstractCoord> &coord,
                                                nlohmann::json &fieldColumn) {
    nlohmann::json a_config = AdjustRange(fieldColumn, coord);
    if(a_config.is_object()) {
        config.merge_patch(a_config);
    }
    tracer->trace("MakeScale: %s, return Category. ", field_.c_str());
    return xg::make_unique<Category>(field_, fieldColumn, config);
}

static std::unique_ptr<AbstractScale> MakeLinear(const std::string &field_,
                                                const nlohmann::json &data,
                                                nlohmann::json &config,
                                                utils::Tracer *tracer,
                                                std::unique_ptr<canvas::coord::AbstractCoord> &coord,
                                                nlohmann::json &fieldColumn) {
    if(!config.contains("max") || !config.contains("min")) {
        std::array<double, 2> range = util::JsonArrayRange(fieldColumn);
        if(!config.contains("max")) {
            config["max"] = range[1];
        }

        if(!config.contains("min")) {
            config["min"] = range[0];
        }
    }

    tracer->trace("MakeScale: %s, return Linear. ", field_.c_str());
    return xg::make_unique<Linear>(field_, fieldColumn, config);
}


static std::unique_ptr<AbstractScale> MakeScale(const std::string &field_,
                                                const nlohmann::json &data,
                                                nlohmann::json &config,
                                                utils::Tracer *tracer,
                                                std::unique_ptr<canvas::coord::AbstractCoord> &coord) {

    if(!data.is_array() || data.size() <= 0) {
        tracer->trace("MakeScale: %s, return Identity. data is empty", field_.c_str());
        return xg::make_unique<Identity>(field_, nlohmann::json());
    }

    nlohmann::json firstObj = data[0];
    nlohmann::json firstVal = firstObj[field_];
    //如果field_对应的数据是数组的画，会把数组打平
    nlohmann::json fieldColumn = util::JsonArrayByKey(data, field_);

    if(config.contains("type")) {
        std::string type = config["type"];
        if(type == "timeCat") {
            tracer->trace("MakeScale: %s, return TimeCategory. ", field_.c_str());
            return xg::make_unique<TimeCategory>(field_, fieldColumn, config);
        } else if(type == "timeSharing") {
            tracer->trace("MakeScale: %s, return TimeSharingLinear. ", field_.c_str());
            return xg::make_unique<TimeSharingLinear>(field_, fieldColumn, config);
        } else if(type.substr(0, 6) == "kline-") {
            tracer->trace("MakeScale: %s, return KlineCat. ", field_.c_str());
            config["klineType"] = type;
            return xg::make_unique<KLineCat>(field_, fieldColumn, config);
        } else if(type == "cat") {
            return MakeCategory(field_, data, config, tracer, coord, fieldColumn);
        }
        //历史资金卡片写了个错误的度量 实际为cat,但写了linear 待其优化后再进
//        else if(type == "linear") {
//            return MakeLinear(field_, data, config, tracer, coord, fieldColumn);
//        }
    }

    if(firstVal.is_string()) {
        return MakeCategory(field_, data, config, tracer, coord, fieldColumn);
    } else if(firstVal.is_number() || firstVal.is_array()) {
        return MakeLinear(field_, data, config, tracer, coord, fieldColumn);
    }

    tracer->trace("MakeScale: %s, return default Identity. ", field_.c_str());
    return xg::make_unique<Identity>(field_, fieldColumn);
}

class ScaleController {
  public:
    const std::unique_ptr<AbstractScale> &CreateScale(const std::string &field_,
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

    void UpdateColConfig(const std::string &field, nlohmann::json cfg) { colConfigs[field] = cfg; }

    bool Empty() { return scales_.empty(); }

    void Clear() {
        scales_.clear();
        colConfigs = {};
    };
    
    size_t ScaleCount() {
        return scales_.size();
    }

  private:
    std::vector<std::unique_ptr<AbstractScale>> scales_{};
    nlohmann::json colConfigs;
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_CONTROLLER_H
