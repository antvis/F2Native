#ifndef XG_GRAPHICS_SCALE_LINEAR_H
#define XG_GRAPHICS_SCALE_LINEAR_H

#include "graphics/scale/Scale.h"
#include "utils/common.h"
#include <WilkinsonExtended/WilkinsonExtended.h>
#include <iomanip>
#include <math.h>
#include <sstream>

namespace xg {
namespace scale {

static double fixedBase(double v, int interval, std::string type) {
    double div;
    if(type == "ceil") {
        div = ceil(v / interval);
    } else if(type == "floor") {
        div = floor(v / interval);
    } else {
        div = round(v / interval);
    }

    return div * interval;
}

class Linear : public AbstractScale {
  public:
    // 线性度量
    // @param field 字段名称
    // @param values 原始值数组
    // @param min 度量最小值
    // @param max 度量最大值
    Linear(const std::string &_field, nlohmann::json _values, nlohmann::json _config = {}) : AbstractScale(_field, _values) {
        InitConfig(_config);
        // TODO min 和 max 的校正逻辑还需要再优化
        int _index = static_cast<int>((max - min) / 5);

        int SNAP_ARRAY[5] = {1, 2, 4, 5, 10};
        int interval = (_index >= 5) ? 10 : SNAP_ARRAY[_index];

        min = fixedBase(min, interval, "floor");
        max = fixedBase(max, interval, "ceil");

        this->minLimit = min;
        this->maxLimit = max;
        this->ticks = this->CalculateTicks();
    }

    ScaleType GetType() const noexcept override { return ScaleType::Linear; }

    void Change(const nlohmann::json &cfg = {}) override {
        InitConfig(cfg);
        if(cfg.contains("values")) {
            values = cfg["values"];
        }

        this->minLimit = min;
        this->maxLimit = max;
        if(!cfg.contains("ticks")) {
            this->ticks = this->CalculateTicks();
        }
    }

    double Scale(const nlohmann::json &key) const override {
        if(!key.is_number()) {
            return std::nan("0.0");
        }
        if(xg::IsEqual(this->max, this->min)) {
            return this->rangeMin;
        }

        // std::size_t cacheKey = key.HashCode();
        // if(scaleCache_.find(cacheKey) != scaleCache_.end()) {
        //     return scaleCache_.at(cacheKey);
        // }

        double percent = GetScalePercent(key);
        double value = rangeMin + percent * (rangeMax - rangeMin);

        // scaleCache_[cacheKey] = value;
        return value;
    }

    nlohmann::json Invert(double val) override {
        double percent = this->GetInvertPercent(val);
        double rt = this->min + percent * (this->max - this->min);
        nlohmann::json rst = rt;
        return rst;
    }

    std::string GetTickText(const nlohmann::json &item) override {
        // 处理 TickText 数值精度
        if(item.is_string()) {
            return item.get<std::string>();
        } else if(item.is_number_integer()) {
            return std::to_string(item.get<int>());
        } else if(item.is_number_float()) {
            float val = item.get<float>();
            if(fabs(val) < XG_EPS) {
                return "0";
            }
            std::stringstream ss;
            ss << std::fixed << std::setprecision(precision) << val;
            return ss.str();
        } else {
            return ""; // TODO get Tick text from callback
        }
    }

  protected:
    nlohmann::json CalculateTicks() override {
        nlohmann::json rst;
        double outlmin, outlmax, outlstep;
        easy_wilk_ext(min, max, this->tickCount, -1, &outlmin, &outlmax, &outlstep);

        if(tickInterval > 0) {
            // TODO  Demo 可以先不处理
        }

        double tickMin = minLimit;
        double tickMax = maxLimit;
        if(tickMin > tickMax) {
            double temp = tickMin;
            tickMin = tickMax;
            tickMax = temp;
        }
        if(tickCount <= 2) {
            rst.push_back(tickMin);
            rst.push_back(tickMax);
            return rst;
        }

        for(double index = tickMin; index <= tickMax; index += outlstep) {
            rst.push_back(index);
        }

        // TODO filter
        return rst;
    }

    double GetScalePercent(double value) const { return (value - this->min) / (this->max - this->min); }

    double GetInvertPercent(double value) const { return (value - this->rangeMin) / (this->rangeMax - this->rangeMin); }

  private:
    void InitConfig(const nlohmann::json &cfg) {
        if(cfg.is_object()) {
            config_.merge_patch(cfg);
        }
        tickCount = config_["tickCount"];
        precision = config_["precision"];

        min = config_["min"];
        max = config_["max"];
        nlohmann::json &range = config_["range"];
        rangeMin = range[0];
        rangeMax = range[1];
    }

  public:
    double minLimit = 0; // 暂时没用到，后续再补充
    double maxLimit = 0; // 暂时没用到，后续再补充
    int tickInterval = -1;
    int precision = 0; // tickText 小数点精度

    nlohmann::json config_ = {{"tickCount", 6}, {"precision", 0}, {"range", {0.0, 1.0}}};

  private:
    std::map<std::size_t, double> scaleCache_;
};
} // namespace scale
} // namespace xg

#endif /* Linear_h */
