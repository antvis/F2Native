#ifndef XG_GRAPHICS_SCALE_CAT_H
#define XG_GRAPHICS_SCALE_CAT_H

#include "graphics/scale/Scale.h"
#include <WilkinsonExtended/WilkinsonExtended.h>
#include <iterator>
#include <math.h>
#include <string>
#include <unordered_map>

using namespace std;

namespace xg {
namespace scale {

/**
 *  分类度量，主要针对字符串
 */
class Category : public AbstractScale {
  public:
    Category(const std::string &_field, const nlohmann::json &_values, const nlohmann::json &config = {})
        : AbstractScale(_field, _values) {
        if(config.is_object()) {
            config_.merge_patch(config);
        }
        if(config_.contains("tickCount")) {
            this->tickCount = config_["tickCount"];
        } else {
            this->tickCount = _values.size();
        }
        nlohmann::json &range = config_["range"];
        rangeMin = range[0];
        rangeMax = range[1];
        this->ticks = this->CalculateTicks();
    }

    ScaleType GetType() const noexcept override { return ScaleType::Cat; }

    void Change(const nlohmann::json &cfg = {}) override {
        if(cfg.is_object()) {
            config_.merge_patch(cfg);
        }

        if(cfg.contains("values")) {
            values = cfg["values"];
        }

        nlohmann::json &range = config_["range"];
        rangeMin = range[0];
        rangeMax = range[1];
        if(!cfg.contains("ticks")) {
            if(config_.contains("tickCount")) {
                this->tickCount = config_["tickCount"];
            }
            this->ticks = this->CalculateTicks();
        }
    }

    double Scale(const nlohmann::json &key) const override {
        double percent = 0;
        if(key.is_string() || values.contains(key)) {
            int index = this->Transform(key);
            percent = CalculatePercent(index, this->min, this->max);
        } else {
            double kVal = 0.;
            if(key.is_number()) {
                kVal = key;
            }
            std::size_t divide = values.size() - 1;
            if(divide == 0)
                divide = 1;
            percent = kVal / (static_cast<float>(divide));
        }
        double value = CalculateValue(percent, this->rangeMin, this->rangeMax);
        // scaleCache_[cacheKey] = value;
        return value;
    }

    nlohmann::json Invert(double val) override {
        double percent = CalculatePercent(val, this->rangeMin, this->rangeMax);
        double domainRange = this->max - this->min;
        int index = static_cast<int>(round(domainRange * percent) + this->min);
        if(index < this->min || index > this->max) {
            return nlohmann::json();
        }
        return values[index];
    }

    virtual int Transform(const nlohmann::json &key) const {
        if(values.empty()) {
            return -1;
        }

        for(size_t i = 0; i < values.size(); i++) {
            if(values[i] == key) {
                return i;
            }
        }

        // std::size_t hashCode = key.HashCode();
        // if(dataIndexTable_.find(hashCode) != dataIndexTable_.end()) {
        //     return dataIndexTable_.at(hashCode);
        // }
        return -1;
    }

  protected:
    nlohmann::json CalculateTicks() override {
        std::size_t valueSize = values.size();
        if(valueSize == 0) {
            return {};
        }
        min = 0;
        max = valueSize - 1 > 0 ? valueSize - 1 : valueSize;
        if(this->tickCount > 0 && this->tickCount <= max - min) {
            nlohmann::json rst;
            double Q[] = {1, 2, 5, 3, 4, 7, 6, 8, 9};
            double w[] = {0.25, 0.2, 0.5, 0.05};
            double outlmin, outlmax, outlstep;
            wilk_ext(min, max, this->tickCount, -1, Q, 6, w, &outlmin, &outlmax, &outlstep);

            int step = static_cast<int>(outlstep);

            if(step > 0) {
                for(int index = min; index <= max; index += step) {
                    nlohmann::json _item = values[index];
                    rst.push_back(_item);
                }
            }
            if(rst.size() < tickCount) {
                rst.push_back(values[valueSize - 1]);
            }
            return rst;
        }
        return values;
    }
    // 定义域转 0~1 值域
    static inline double CalculatePercent(double index, double min, double max) { return (index - min) / (max - min); }

    static inline double CalculateValue(double percent, double min, double max) { return min + percent * (max - min); }

  protected:
    nlohmann::json config_ = {
        {"range", {0, 1.}},
    };

  private:
    std::map<std::size_t, double> scaleCache_;
}; // namespace scale
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_CAT_H
