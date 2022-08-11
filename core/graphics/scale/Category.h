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
    Category(const std::string &_field, const vector<Any> &_values, const ScaleCfg &_config)
        : AbstractScale(_field, _values, _config) {
        Change(_config);
    }

    ScaleType GetType() const noexcept override { return ScaleType::Cat; }

    void Change(const ScaleCfg &cfg) override {
        config = cfg;
        
        if (config.HasDomain()) {
            min = config.domain[0];
            max = config.domain[1];
        } else {
            min = 0;
            max = fmax(0, values.size() - 1);
        }
        
        if (config.HasRange()) {
            range = config.range;
        }
        
        tickCount = std::isnan(config.tickCount) ? GetValuesSize() : config.tickCount;
            
        if (config.ticks.empty()) {
            ticks = CalculateTicks();
        }
    }

    double Scale(const Any &key) override {
        double percent = 0;
        std::size_t index = this->Transform(key);
        if(index + 1 > 0) {
            percent = CalculatePercent(static_cast<double>(index), min, max);
        } else {
            double kVal = 0.;
            if(key.GetType().IsNumber()) {
                kVal = key;
            }
            //GetValuesSize()是unsigned long,当等于0的时候GetValuesSize()-1会越界
            //转换成int避免这个问题
            std::size_t divide = fmax(1, (int)GetValuesSize() - 1);
            percent = kVal / (static_cast<double>(divide));
        }
        double value = CalculateValue(percent, range[0], range[1]);
        return value;
    }

    Any Invert(double val) override {
        double percent = CalculatePercent(val, range[0], range[1]);
        double domainRange = max - min;
        int index = static_cast<int>(round(domainRange * percent) + min);
        if(index < min || index > max) {
            return Any();
        }
        return values[index];
    }

    virtual std::size_t Transform(const Any &key) const {
        if(values.empty()) {
            return -1;
        }

        std::size_t index = -1;
        for(size_t i = 0; i < values.size(); i++) {
            if(values[i].IsEqual(key)) {
                index = i;
                break;
            }
        }

        return index;
    }

    virtual inline std::size_t GetValuesSize() noexcept override { return max - min + 1; }
    virtual inline double GetMax() noexcept override { return max; }
    virtual inline double GetMin() noexcept override { return min; }

  protected:
    virtual vector<string> CalculateTicks() override {
            std::size_t valueSize = values.size();
            if(valueSize == 0) {
                return {};
            }
            vector<string> rst;
            if (tickCount == 0) {
                return {};
            }else if(tickCount == 1) {
                //对齐f2的展示 tickCount为1的时候 = tickCount为values.size()
                for (size_t index = 0; index < values.size(); ++index) {
                    auto &_item = values[index];
                    rst.push_back(_item.Cast<string>());
                }
                return rst;
            }
            //cat平均算法，保头保尾
            else {
                size_t outlstep = values.size() / (tickCount - 1);
                outlstep = fmax(outlstep, 1);
                for (size_t index = 0; index < values.size(); index = index + outlstep) {
                    auto &_item = values[index];
                    rst.push_back(_item.Cast<std::string>());
                }
                         
                // 如果最后一个tick不等于原数据的最后一个
                auto &last = values[values.size() - 1];
                if(rst[rst.size() -1] != last.Cast<std::string>()) {
                    //if 逻辑如何走到？
                    if(rst.size() >= tickCount) {
                        rst[rst.size() - 1] = last.Cast<std::string>();
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
protected:
    size_t min, max, tickCount;
    array<float, 2> range = {0, 1};
}; // namespace scale
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_CAT_H
