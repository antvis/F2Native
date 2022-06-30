#ifndef XG_GRAPHICS_SCALE_LINEAR_H
#define XG_GRAPHICS_SCALE_LINEAR_H

#include <iomanip>
#include <math.h>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include "../Scale.h"
#include "../../global.h"
#include "../../util/json.h"
#include "../../util/json_util.h"
#include "../../../utils/StringUtil.h"
#include "../../../utils/common.h"

namespace xg {
namespace scale {

class Linear : public AbstractScale {
  public:
    // 线性度量
    Linear(const std::string &_field, const nlohmann::json &_values, const nlohmann::json &_config = {}) : AbstractScale(_field, _values, _config) {
        
        InitConfig(_config);
        hasMaxMin = !std::isnan(max) && !std::isnan(min);
        if (std::isnan(max) || std::isnan(min)) {
            std::array<double, 2> range = util::JsonArrayRange(_values);
            if (std::isnan(max)) {
                max = range[1];
            }
            
            if (std::isnan(min)) {
                min = range[0];
            }
        }
        ticks = CalculateTicks();
    }

    ScaleType GetType() const noexcept override { return ScaleType::Linear; }

    void Change(const nlohmann::json &cfg = {}) override {
        InitConfig(cfg);
        this->ticks = this->CalculateTicks();
    }

    double Scale(const nlohmann::json &key) override {
        if(!key.is_number()) {
            return std::nan("0.0");
        }
        
        if (std::isnan(max) || std::isnan(min)) {
            return std::nan("0.0");
        }
        
        if(xg::IsEqual(this->max, this->min)) {
            return this->rangeMin;
        }

        double percent = GetScalePercent(key);
        double value = rangeMin + percent * (rangeMax - rangeMin);
        
        //tick范围的代码保护，限制在0~1
        value = fmin(value, 1.0);
        value = fmax(value, 0);

        return value;
    }

    nlohmann::json Invert(double val) override {
        double percent = this->GetInvertPercent(val);
        double rt = this->min + percent * (this->max - this->min);
        nlohmann::json rst = rt;
        return rst;
    }

    std::string GetTickText(const nlohmann::json &item, XChart *chart) override;

  protected:
    nlohmann::json CalculateTicks() override {
        //用户没有主动设置max和min，则使用nice算法
        if ((nice == true && !hasMaxMin)) {
            nlohmann::json _ticks = NiceCalculateTicks();
            //修改最值
            this->min = _ticks[0];
            this->max = _ticks[_ticks.size() - 1];
            return _ticks;
        } else if (IsEqual(max, min)) {
            nlohmann::json rst = {min};
            return rst;
        } else {
            if(tickCount <= 2) {
                nlohmann::json rst = {min, max};
                return rst;
            }

            double interval = (this->max - this->min) / (this->tickCount - 1);
            
            nlohmann::json rst;
            for(int i = 0; i < tickCount; ++i) {                
                rst.push_back(this->min + i * interval);
            }
         
            return rst;
        }
    }

    std::vector<double> NiceCalculateTicks() {
        std::vector<double> rst;

        std::size_t count = tickCount;
        

        // 计算interval， 优先取tickInterval
        const double interval = GetBestInterval(count, max, min);
        //tick为-0.1, 0.1, 0.2的时候算出来inf,保护下
        if(std::isnan(interval) || std::isinf(interval)) {
            rst.push_back(min);
            rst.push_back(max);
            return rst;
        }
        tickInterval = interval;

        // 通过interval计算最小tick
        const double minTick = floor(min / interval) * interval;

        // 如果指定了tickInterval, count 需要根据指定的tickInterval来算计
        if(tickInterval != -1) {
            const std::size_t intervalCount = abs(ceil((max - minTick) / tickInterval)) + 1;
            // tickCount 作为最小 count 处理
            count = std::max(count, intervalCount);
        }

        int tickLength = 0;
        const int fixedLength = GetFixedLength(interval);
        while(tickLength < count) {
            rst.push_back(ToFixed(minTick + tickLength * interval, fixedLength));
            tickLength++;
        }
        return rst;
    }

    double GetScalePercent(double value) const { return (value - this->min) / (this->max - this->min); }

    double GetInvertPercent(double value) const { return (value - this->rangeMin) / (this->rangeMax - this->rangeMin); }

    double GetBestInterval(const std::size_t tickCount, const double max, const double min) {
        // 如果最大最小相等，则直接按1处理
        if(IsEqual(max, min)) {
            return 1 * GetFactor(max);
        }
        // 1.计算平均刻度间隔
        const double avgInterval = (max - min) / (tickCount - 1);
        // 2.数据标准归一化 映射到[1-10]区间
        const double factor = GetFactor(avgInterval);
        const double calInterval = avgInterval / factor;
        const double calMax = max / factor;
        const double calMin = min / factor;
        // 根据平均值推算最逼近刻度值
        int similarityIndex = 0;
        for(int index = 0; index < SNAP_COUNT_ARRAY.size(); index++) {
            const double item = SNAP_COUNT_ARRAY[index];
            if(calInterval <= item) {
                similarityIndex = index;
                break;
            }
        }
        const double similarityInterval = GetInterval(similarityIndex, tickCount, calMin, calMax);

        // 小数点位数还原到数据的位数, 因为similarityIndex有可能是小数，所以需要保留similarityIndex自己的小数位数
        const int fixedLength = GetFixedLength(similarityInterval) + GetFixedLength(factor);
        return ToFixed(similarityInterval * factor, fixedLength);
    }

    double GetFactor(double number) {
        // 取正数
        number = fabs(number);
        double factor = 1.f;

        if(IsZero(number)) {
            return factor;
        }

        // 小于1,逐渐放大
        if(number < 1) {
            int count = 0;
            while(number < 1) {
                factor = factor / 10;
                number = number * 10;
                count++;
            }
            // 浮点数计算出现问题
            if(std::to_string(factor).size() > 12) {
                factor = ToFixed(factor, count);
            }
            return factor;
        }
        // 大于10逐渐缩小
        while(number > 10) {
            factor = factor * 10;
            number = number / 10;
        }

        return factor;
    }

    double GetInterval(const std::size_t startIndex, const std::size_t tickCount, const double min, const double max) const {
        bool verify = false;
        double interval = SNAP_COUNT_ARRAY[startIndex];
        // 刻度值校验，如果不满足，循环下去
        for(std::size_t i = startIndex; i < SNAP_COUNT_ARRAY.size(); i++) {
            if(IntervalIsVerify(SNAP_COUNT_ARRAY[i], tickCount, max, min)) {
                // 有符合条件的interval
                interval = SNAP_COUNT_ARRAY[i];
                verify = true;
                break;
            }
        }
        // 如果不满足, 依次缩小10倍，再计算
        if(!verify) {
            return 10 * GetInterval(0, tickCount, min / 10, max / 10);
        }
        return interval;
    }

    // 刻度是否满足展示需求
    bool IntervalIsVerify(const double interval, const std::size_t tickCount, const double max, const double min) const {
        const double minTick = floor(min / interval) * interval;
        if(minTick + (tickCount - 1) * interval >= max) {
            return true;
        }
        return false;
    }

    // 计算小数点应该保留的位数
    int GetFixedLength(const double num) const {
        string str = std::to_string(num);
        vector<string> sv;
        StringUtil::Split(std::to_string(num), sv, '.');
        if(sv.size() >= 2) {
            return std::min((int)(sv.size()), 20);
        }
        return 0;
    }

    double ToFixed(const double val, const int decimal) const { return val; }

  private:
    void InitConfig(const nlohmann::json &cfg) override {
        AbstractScale::InitConfig(cfg);
        nice = json::GetBool(cfg, "nice", nice);
        precision = json::GetIntNumber(cfg, "precision", precision);
    }

  public:
    double tickInterval = -1;
    int precision = 0; // tickText 小数点精度
    bool nice = true;
    bool hasMaxMin = false; //用户设置了max和min则为true，只设置了其中一个则为false
};
} // namespace scale
} // namespace xg

#endif /* Linear_h */
