#ifndef XG_GRAPHICS_SCALE_CONTROLLER_H
#define XG_GRAPHICS_SCALE_CONTROLLER_H

#include "graphics/canvas/Coord.h"
#include "graphics/scale/Category.h"
#include "graphics/scale/Identity.h"
#include "graphics/scale/TimeCategory.h"
#include "graphics/scale/continuous/Linear.h"
#include "graphics/scale/continuous/TimeSharingLinear.h"
#include "graphics/util/json_util.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <utils/common.h>
#include <vector>

namespace xg {
namespace scale {

static nlohmann::json AdjustRange(const nlohmann::json &fieldColumn, std::unique_ptr<canvas::coord::AbstractCoord> &coord) {
    nlohmann::json cfg;
    const std::size_t size = fieldColumn.size();
    if(size <= 1) {
        cfg["range"] = {0.5, 1.0};
    } else {
        if(coord->GetType() == coord::CoordType::Polar) {
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

static std::unique_ptr<AbstractScale> MakeScale(const std::string &field_,
                                                const nlohmann::json &data,
                                                nlohmann::json config,
                                                utils::Tracer *tracer,
                                                std::unique_ptr<canvas::coord::AbstractCoord> &coord) {

    if(!data.is_array() || data.size() <= 0) {
        tracer->trace("MakeScale: %s, return Identity. data is empty", field_.c_str());
        return xg::make_unique<Identity>(field_, nlohmann::json());
    }

    nlohmann::json firstObj = data[0];
    nlohmann::json firstVal = firstObj[field_];
    nlohmann::json fieldColumn = util::JsonArrayByKey(data, field_);

    if(config.contains("type")) {
        std::string type = config["type"];
        if(type == "timeCat") {
            tracer->trace("MakeScale: %s, return TimeCategory. ", field_.c_str());
            return xg::make_unique<TimeCategory>(field_, fieldColumn, config);
        } else if(type == "timeSharing") {
            tracer->trace("MakeScale: %s, return TimeSharingLinear. ", field_.c_str());
            return xg::make_unique<TimeSharingLinear>(field_, fieldColumn, config);
        }
    }

    if(firstVal.is_string()) {
        tracer->trace("MakeScale: %s, return Category. ", field_.c_str());
        nlohmann::json a_config = AdjustRange(fieldColumn, coord);
        if(a_config.is_object()) {
            config.merge_patch(a_config);
        }
        return xg::make_unique<Category>(field_, fieldColumn, config);
    } else if(firstVal.is_number()) {
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

    tracer->trace("MakeScale: %s, return default Identity. ", field_.c_str());
    return xg::make_unique<Identity>(field_, fieldColumn);
}

class ScaleController {
  public:
    const std::unique_ptr<AbstractScale> &CreateScale(const std::string &field_,
                                                      const nlohmann::json &data,
                                                      utils::Tracer *tracer,
                                                      std::unique_ptr<canvas::coord::AbstractCoord> &coord) {
        if(!scales_.empty()) {
            std::vector<std::unique_ptr<AbstractScale>>::iterator it =
                std::find_if(scales_.begin(), scales_.end(),
                             [&](const std::unique_ptr<AbstractScale> &item) { return (item->field == field_); });

            // find_if 如果没找到返回的是 last，再做一次 check
            if(it != scales_.end()) {
                // TODO 更新 scale config, 暂时没有什么 config
                //                tracer->trace("scale :%s 命中缓存", field_.c_str());
                return (*it);
            }
        }
        std::unique_ptr<AbstractScale> _target = MakeScale(field_, data, colConfigs[field_], tracer, coord);
        scales_.push_back(std::move(_target));
        return scales_[scales_.size() - 1];
    }

    void UpdateColConfig(const std::string &field, nlohmann::json cfg) { colConfigs[field] = cfg; }

  private:
    std::vector<std::unique_ptr<AbstractScale>> scales_{};
    nlohmann::json colConfigs;
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_CONTROLLER_H
