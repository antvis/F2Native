#ifndef XG_GRAPHICS_SCALE_TIMESHARINGLINEAR_H
#define XG_GRAPHICS_SCALE_TIMESHARINGLINEAR_H

#include "../Scale.h"
#include "../../util/json.h"
#include "../../../utils/xtime.h"

namespace xg {
namespace scale {

class TimeSharingLinear : public AbstractScale {
  public:
    TimeSharingLinear(const std::string &_field, const vector<const Any *> &_values, const ScaleCfg &_config)
        : AbstractScale(_field, _values, _config) {
        Change(_config);
    }
    
    //timeRange格式 @[@[@(1608687000000), @(1608694200000)], @[@(1608699600000), @(1608706800000)]]
    bool CheckValidTimeRange(const ScaleCfg &config) {
        bool rst = true;
        auto &timeRange = config.timeRange;
        for(std::size_t index = 0; index < timeRange.size(); ++index) {
            auto &range = timeRange[index];
            rst &= !std::isnan(range[0]) && !std::isnan(range[1]);
        }
        
        return rst;
    }

    ScaleType GetType() const noexcept override { return ScaleType::TimeSharingLinear; }

    virtual void Change(const ScaleCfg &cfg) override {
        //入口检查 其它方法都判断这个变量
        if (!(isTimeRangeValid_ = CheckValidTimeRange(config))) {
            return;
        }

        auto &timeRange = config.timeRange;
        auto &first = timeRange[0];
        auto &last = timeRange[timeRange.size() - 1UL];

        min = first[0];
        max = last[1];
        this->valueSize_ = _GetValuesSize();
    }

    double Scale(const Any &key) override {
        if (!isTimeRangeValid_) {
            return NAN;
        }
        
        double time = 0;
        if (key.GetType().IsString()) {
            time = stod(key.Cast<string>());
        } else if (key.GetType().IsNumber()) {
            time = key.Cast<double>();
        } else {
            return NAN;
        }
        if(time < min || time > max) {
            return NAN;
        }

        double index = 0;
        auto &timeRange = config.timeRange;
        for(std::size_t i = 0; i < timeRange.size(); ++i) {
            auto &range = timeRange[i];
            double range0 = range[0];
            double range1 = range[1];
            if(time >= range[1] && i == timeRange.size() - 1) {
                time = range[1];
            }

            if(time <= range[1]) {
                double delta = time - range0;
                //这个分支永远走不到 因为上面判断了time < min_ return
                if(delta < 0) {
                    delta = 0;
                }
                index += delta / 60000;
                break;
                //                double rst = (i + delta / (range1 - range0)) / timeRange.size();
                //                printf("%lf rst %lf time\n", rst, time);
                //                return rst;
            } else {
                index += (range1 - range0) / 60000 + 1;
            }
        }
        double rst = index / (valueSize_ -1 );
        return rst;
    }
    
    Any Invert(double val) override {
        if (!isTimeRangeValid_) {
            return 0;
        }
        std::size_t index = val * (valueSize_ -1);
        auto &timeRange = config.timeRange;
        for(std::size_t i = 0; i < timeRange.size(); ++i) {
            auto &range = timeRange[i];
            long long start = range[0];
            long long end = range[1];
            std::size_t count = (end - start) / (60 * 1000) + 1;
            if(index >= count) {
                index -= count;
            } else {
                return start + index * 60 * 1000;
            }
        }
        return 0;
    }

    inline std::size_t GetValuesSize() noexcept override { return this->valueSize_; }

    //timeRange格式为[[9:30, 11:30] [13:00, 16:00]]
    //返回的tick应该是9:30, 11.30/13:00 16:00
    std::string GetTickText(const Any &item, XChart *chart) override {
        if (!isTimeRangeValid_) {
            return "";
        }
        auto &timeRange = config.timeRange;
        long long timeZoneOffset = 0;
        bool forceTimeZone = false;
        if(config.timeZoneOffset > 0) {
            timeZoneOffset = config.timeZoneOffset;
            forceTimeZone = true;
        }
        timeZoneOffset *= 1000;
        for(std::size_t i = 0; i < timeRange.size(); ++i) {
            auto &range = timeRange[i];
            auto val = item.Cast<long long>();
            
            //11:30/13:00 right case
            //11:30/13:00 wrong case
            if(val == range[1] && (i + 1) < timeRange.size() && val != timeRange[i + 1][0]) {
                long long lastFirst = timeRange[i + 1][0];
                return xg::TimeStampToHHmm(val + timeZoneOffset, forceTimeZone) + "/" + xg::TimeStampToHHmm(lastFirst + timeZoneOffset, forceTimeZone);
                       
            }
        }
        return xg::TimeStampToHHmm(item.Cast<long long>() + timeZoneOffset, forceTimeZone);
    }

  protected:
    //未对外暴露 在入口处已经check过TimeRange了，这里不需要判断
    vector<string> CalculateTicks() override { return {}; }

  private:
    std::size_t _GetValuesSize() {
        std::size_t count = 0;
        auto &timeRange = config.timeRange;
        for(std::size_t i = 0; i < timeRange.size(); ++i) {
            auto &range = timeRange[i];
            long long start = range[0];
            long long end = range[1];
            count += (end - start) / (60 * 1000) + 1;
        }
        return count;
    }

  private:
    std::size_t valueSize_ = 0;
    bool isTimeRangeValid_;
    long long max, min;
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_TIMESHARINGLINEAR_H
