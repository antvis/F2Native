#include "graphics/scale/Scale.h"
#include <utils/xtime.h>

#ifndef XG_GRAPHICS_SCALE_TIMESHARINGLINEAR_H
#define XG_GRAPHICS_SCALE_TIMESHARINGLINEAR_H

namespace xg {
namespace scale {

class TimeSharingLinear : public AbstractScale {
  public:
    TimeSharingLinear(const std::string &_field, nlohmann::json _values, nlohmann::json _config = {})
        : AbstractScale(_field, _values), config_(_config) {
        if(!config_.contains("timeRange") || !config_["timeRange"].is_array()) {
            // throw std::runtime_error("not found time range.");
            return;
        }
        nlohmann::json &timeRange = config_["timeRange"];
        if(timeRange.is_array() && timeRange.size() > 0) {
            nlohmann::json &first = timeRange[0];
            if(first.is_array() && first.size() > 0) {
                min_ = first[0];
            }

            nlohmann::json &last = timeRange[timeRange.size() - 1UL];
            if(last.is_array() && last.size() > 0) {
                max_ = last[1];
            }
        }
        this->valueSize_ = _GetValuesSize();
        this->ticks = this->CalculateTicks();
    }

    ScaleType GetType() const noexcept override { return ScaleType::TimeSharingLinear; }

    virtual void Change(const nlohmann::json &cfg = {}) override {
        if(cfg.is_object()) {
            config_.merge_patch(cfg);
        }

        if(cfg.contains("values")) {
            values = cfg["values"];
        }

        nlohmann::json &timeRange = config_["timeRange"];
        if(timeRange.is_array() && timeRange.size() > 0) {
            nlohmann::json &first = timeRange[0];
            if(first.is_array() && first.size() > 0) {
                min_ = first[0];
            }

            nlohmann::json &last = timeRange[timeRange.size() - 1UL];
            if(last.is_array() && last.size() > 0) {
                max_ = last[1];
            }
        }

        this->valueSize_ = _GetValuesSize();
        if(!cfg.contains("ticks")) {
            this->ticks = this->CalculateTicks();
        }
    }

    double Scale(const nlohmann::json &key) override {
        if (!key.is_number()) {
            return std::nan("0.0");
        }
        double time = key;

        if(time < min_ || time > max_)
            return 0;

        double index = 0;
        const nlohmann::json &timeRange = config_["timeRange"];
        for(std::size_t i = 0; i < timeRange.size(); ++i) {
            const nlohmann::json &range = timeRange[i];
            double range0 = range[0];
            double range1 = range[1];
            if(time >= range[1] && i == timeRange.size() - 1) {
                time = range[1];
            }

            if(time <= range[1]) {
                double delta = time - range0;
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

    inline std::size_t GetValuesSize() noexcept override { return this->valueSize_; }

    std::string GetTickText(const nlohmann::json &item) override {
        nlohmann::json &timeRange = config_["timeRange"];
        long long timeZoneOffset = 0;
        bool forceTimeZone = false;
        if(config_.contains("timeZoneOffset")) {
            timeZoneOffset = config_["timeZoneOffset"];
            forceTimeZone = true;
        }
        timeZoneOffset *= 1000;
        for(std::size_t i = 0; i < timeRange.size(); ++i) {
            nlohmann::json &range = timeRange[i];
            if(item == range[0] && i > 0) {
                long long lastEnd = timeRange[i - 1][1];
                if(lastEnd == item) {
                    return xg::TimeStampToHHmm(item.get<long long>() + timeZoneOffset, forceTimeZone);
                }
                return xg::TimeStampToHHmm(lastEnd + timeZoneOffset, forceTimeZone) + "/" +
                       xg::TimeStampToHHmm(item.get<long long>() + timeZoneOffset, forceTimeZone);
            }
        }
        return xg::TimeStampToHHmm(item.get<long long>() + timeZoneOffset, forceTimeZone);
    }

  protected:
    nlohmann::json CalculateTicks() override {
        nlohmann::json rst;
        nlohmann::json &timeRange = config_["timeRange"];
        for(std::size_t i = 0; i < timeRange.size(); ++i) {
            nlohmann::json &range = timeRange[i];
            rst.push_back(range[0]);

            if(i == timeRange.size() - 1) {
                rst.push_back(range[1]);
            }
        }
        return rst;
    }

    nlohmann::json Invert(double val) override {
        std::size_t index = val * (valueSize_ -1);
        nlohmann::json &timeRange = config_["timeRange"];
        for(std::size_t i = 0; i < timeRange.size(); ++i) {
            nlohmann::json &range = timeRange[i];
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

  private:
    std::size_t _GetValuesSize() {
        std::size_t count = 0;
        nlohmann::json &timeRange = config_["timeRange"];
        for(std::size_t i = 0; i < timeRange.size(); ++i) {
            nlohmann::json &range = timeRange[i];
            long long start = range[0];
            long long end = range[1];
            count += (end - start) / (60 * 1000) + 1;
        }
        return count;
    }

  private:
    nlohmann::json config_;
    nlohmann::json min_;
    nlohmann::json max_;
    std::size_t valueSize_ = 0;
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_TIMESHARINGLINEAR_H
