#ifndef XG_GRAPHICS_SCALE_CAT_H
#define XG_GRAPHICS_SCALE_CAT_H

#include <iterator>
#include <math.h>
#include <string>
#include <unordered_map>
#include "Scale.h"
#include "../util/json.h"
#include "../../utils/common.h"

using namespace std;

namespace xg {
namespace scale {

/**
 *  分类度量，主要针对字符串
 */
class Category : public AbstractScale {
  public:
    Category(const std::string &_field, const nlohmann::json &_values, const nlohmann::json &_config)
        : AbstractScale(_field, _values, _config) {
            
        InitConfig(_config);
        ticks = CalculateTicks();
    }

    ScaleType GetType() const noexcept override { return ScaleType::Cat; }

    void Change(const nlohmann::json &cfg = {}) override {
        values = json::GetArray(cfg, "values", values);
        
        //里面max和min的计算依赖上面的values
        InitConfig(cfg);
        this->ticks = this->CalculateTicks();
    }

    double Scale(const nlohmann::json &key) override {
        double percent = 0;
        std::size_t index = this->Transform(key);
        if(index + 1 > 0) {
            percent = CalculatePercent(static_cast<double>(index), this->min, this->max);
        } else {
            double kVal = 0.;
            if(key.is_number()) {
                kVal = key;
            }
            //GetValuesSize()是unsigned long,当等于0的时候GetValuesSize()-1会越界
            //转换成int避免这个问题
            std::size_t divide = fmax(1, (int)GetValuesSize() - 1);
            percent = kVal / (static_cast<double>(divide));
        }
        double value = CalculateValue(percent, this->rangeMin, this->rangeMax);
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
            nlohmann::json rst;
            if (this->tickCount == 0) {
                return {};
            }else if(this->tickCount == 1) {
                //对齐f2的展示 tickCount为1的时候 = tickCount为values.size()
                for (size_t index = 0; index < values.size(); ++index) {
                    const nlohmann::json &_item = values[index];
                    rst.push_back(_item);
                }
                return rst;
            }
            //cat平均算法，保头保尾
            else {
                size_t outlstep = values.size() / (tickCount - 1);
                outlstep = fmax(outlstep, 1);
                for (size_t index = 0; index < values.size(); index = index + outlstep) {
                    nlohmann::json &_item = values[index];
                    rst.push_back(_item);
                }
                         
                // 如果最后一个tick不等于原数据的最后一个
                auto &last = values[values.size() - 1];
                if(rst[rst.size() -1] != last) {
                    //if 逻辑如何走到？
                    if(rst.size() >= tickCount) {
                        rst[rst.size() - 1] = last;
                    }else {
                        rst.push_back(last);
                    }
                }
                
                return rst;
            }
        }
    
    // 定义域转 0~1 值域
    static inline double CalculatePercent(double index, double min, double max) {
        if(xg::IsEqual(max, min)) {
            return (index - min);
        }
        return (index - min) / (max - min);
    }

    static inline double CalculateValue(double percent, double min, double max) { return min + percent * (max - min); }

    virtual void InitConfig(const nlohmann::json &config) override {
        AbstractScale::InitConfig(config);

        if(config.contains("domain") && config["domain"].is_array()) {
            const nlohmann::json &domain = config["domain"];
            this->min = domain[0];
            this->max = domain[1];
        } else {
            this->min = 0.;
            this->max = fmax(0, values.size() - 1);
        }
        
        if(config.contains("tickCount")) {
            this->tickCount = config["tickCount"];
        } else {
            this->tickCount = GetValuesSize();
        }
    }
}; // namespace scale
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_CAT_H
