#ifndef XG_GRAPHICS_SCALE_CAT_H
#define XG_GRAPHICS_SCALE_CAT_H

#include "graphics/scale/Scale.h"
#include "utils/common.h"
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
        this->InitConfig();
    }

    ScaleType GetType() const noexcept override { return ScaleType::Cat; }

    void Change(const nlohmann::json &cfg = {}) override {
        bool valuesChanged = false;
        if(cfg.contains("values")) {
            values = cfg["values"];
            valuesChanged = true;
        }

        if(cfg.contains("range") && cfg["range"].is_array()) {
            const nlohmann::json &range = cfg["range"];
            rangeMin = range[0];
            rangeMax = range[1];
            config_["range"] = range;
        }

        bool reCalcTicks = true;
        if(!cfg.contains("ticks")) {
            if(config_.contains("tickCount")) {
                this->tickCount = config_["tickCount"];
            }
        } else {
            if(cfg["ticks"].is_boolean()) {
                reCalcTicks = cfg["ticks"];
            } else if(cfg["ticks"].is_array()) {
                this->ticks = cfg["ticks"];
                reCalcTicks = false;
            }
        }

        if(cfg.contains("domain") && cfg["domain"].is_array()) {
            const nlohmann::json &domain = cfg["domain"];
            this->min = domain[0];
            this->max = domain[1];
        } else if(valuesChanged) {
            this->min = 0.;
            this->max = fmax(0, values.size() - 1);
        }

        if(reCalcTicks) {
            this->ticks = this->CalculateTicks();
        }
        scaledCache_.clear();
    }

    double Scale(const nlohmann::json &key) override {
        std::size_t keyHash = nlohmann::detail::hash(key);
        if(scaledCache_.find(keyHash) != scaledCache_.end()) {
            return scaledCache_.at(keyHash);
        }

        double percent = 0;
        if(key.is_string() || values.contains(key)) {
            std::size_t index = this->Transform(key);
            percent = CalculatePercent(static_cast<double>(index), this->min, this->max);
        } else {
            double kVal = 0.;
            if(key.is_number()) {
                kVal = key;
            }
            std::size_t divide = fmax(1, GetValuesSize() - 1);
            percent = kVal / (static_cast<double>(divide));
        }
        double value = CalculateValue(percent, this->rangeMin, this->rangeMax);
        scaledCache_[keyHash] = value;
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

    virtual std::size_t Transform(const nlohmann::json &key) const {
        if(values.empty()) {
            return -1;
        }

        std::size_t index = -1;
        for(size_t i = 0; i < values.size(); i++) {
            if(values[i] == key) {
                index = i;
                break;
            }
        }

        return index;
    }

    virtual inline std::size_t GetValuesSize() noexcept override { return this->max - this->min + 1; }

  protected:
    virtual nlohmann::json CalculateTicks() override {
        std::size_t valueSize = values.size();
        if(valueSize == 0) {
            return {};
        }
        if(this->tickCount > 0 && this->tickCount <= max - min + 1) {
            nlohmann::json rst;
            double Q[] = {1, 2, 5, 3, 4, 7, 6, 8, 9};
            double w[] = {0.25, 0.2, 0.5, 0.05};
            double outlmin, outlmax, outlstep;
            wilk_ext(min, max, this->tickCount, -1, Q, 6, w, &outlmin, &outlmax, &outlstep);

            int step = static_cast<int>(outlstep);

            if(step > 0) {
                std::size_t _max = static_cast<std::size_t>(max);
                for(std::size_t index = static_cast<std::size_t>(min); index <= _max; index += step) {
                    nlohmann::json &_item = values[index];
                    rst.push_back(_item);
                }
            }
            if(rst.size() < tickCount) {
                rst.push_back(values[valueSize - 1]);
            }
            return std::move(rst);
        }
        return values;
    }
    // 定义域转 0~1 值域
    static inline double CalculatePercent(double index, double min, double max) { return (index - min) / fmax(max - min, 1); }

    static inline double CalculateValue(double percent, double min, double max) { return min + percent * (max - min); }

    virtual void InitConfig() {
        nlohmann::json &range = config_["range"];
        rangeMin = range[0];
        rangeMax = range[1];

        if(config_.contains("domain") && config_["domain"].is_array()) {
            nlohmann::json &domain = config_["domain"];
            this->min = domain[0];
            this->max = domain[1];
        } else {
            this->min = 0.;
            this->max = fmax(0, values.size() - 1);
        }

        if(config_.contains("tick")) {
            SetTickCallbackFun(config_["tick"]);
        }

        if(config_.contains("tickCount")) {
            this->tickCount = config_["tickCount"];
        } else {
            this->tickCount = GetValuesSize();
        }

        this->ticks = this->CalculateTicks();
    }

  protected:
    nlohmann::json config_ = {
        {"range", {0.0, 1.0}},
    };

  private:
    std::unordered_map<std::size_t, double> scaledCache_;
}; // namespace scale
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_CAT_H
