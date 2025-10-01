//
// Created by luna on 2024/6/20.
//

#ifndef STOCK_PLOT_CORE_FIVEDAYSLINEAR_H
#define STOCK_PLOT_CORE_FIVEDAYSLINEAR_H


#include "../Scale.h"
#include "../../util/json.h"
#include "../../../utils/xtime.h"

namespace xg {
    namespace scale {

        class FiveDaysLinear : public AbstractScale {
        public:
            FiveDaysLinear(const std::string &_field, nlohmann::json _values, nlohmann::json _config = {})
                    : AbstractScale(_field, _values), config_(_config) {
                //入口检查 其它方法都判断这个变量
                if (!(isTimeRangeValid_ = CheckValidTimeRange(_config))) {
                    return;
                }

                nlohmann::json &timeRange = config_["timeRange"];
                nlohmann::json &first = timeRange[0];
                nlohmann::json &last = timeRange[timeRange.size() - 1UL];

                min_ = first[0];
                max_ = last[1];
                this->valueSize_ = _GetValuesSize();
                this->ticks = this->CalculateTicks();
            }

            //timeRange格式 @[@[@(1608687000000), @(1608694200000)], @[@(1608699600000), @(1608706800000)]]
            bool CheckValidTimeRange(const nlohmann::json &config) {
                const nlohmann::json &timeRange = json::GetArray(config, "timeRange");
                for(std::size_t index = 0; index < timeRange.size(); ++index) {
                    const nlohmann::json &range = timeRange[index];
                    bool valid = range.is_array() && range.size() == 2 && range[0].is_number() && range[1].is_number();

                    if (!valid) {
                        return false;
                    }
                }

                return timeRange.size();
            }

            ScaleType GetType() const noexcept override { return ScaleType::FiveDaysLinear; }

            virtual void Change(const nlohmann::json &cfg = {}) override {
                if (!isTimeRangeValid_) {
                    return;
                }

                if(cfg.is_object()) {
                    config_.merge_patch(cfg);
                }

                if(cfg.contains("values")) {
                    values = cfg["values"];
                }

                nlohmann::json &timeRange = config_["timeRange"];
                nlohmann::json &first = timeRange[0];
                nlohmann::json &last = timeRange[timeRange.size() - 1UL];
                min_ = first[0];
                max_ = last[1];

                this->valueSize_ = _GetValuesSize();
                if(!cfg.contains("ticks")) {
                    this->ticks = this->CalculateTicks();
                }
            }

            double Scale(const nlohmann::json &key) override {
                if (!isTimeRangeValid_) {
                    return std::nan("0");
                }

                if (!key.is_number()) {
                    return std::nan("0");
                }
                double time = key;

                if(time < min_ || time > max_) {
                    return std::nan("0");
                }

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
                        //这个分支永远走不到 因为上面判断了time < min_ return
                        if(delta < 0) {
                            delta = 0;
                        }
                        index += delta / 60000;
                        break;
                    } else {
                        index += (range1 - range0) / 60000 + 1;
                    }
                }
                double rst = index / (valueSize_ -1 );
                return rst;
            }

            nlohmann::json Invert(double val) override {
                if (!isTimeRangeValid_) {
                    return 0;
                }
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

            inline std::size_t GetValuesSize() noexcept override { return this->valueSize_; }

            //timeRange格式为[
            // [2024-06-14 09:30:00, 2024-06-14 11:30:00],[2024-06-14 13:00:00, 2024-06-14 15:00:00],
            // [2024-06-17 09:30:00, 2024-06-17 11:30:00],[2024-0 6-17 13:00:00, 2024-06-17 15:00:00],
            // [2024-06-18 09:30:00, 2024-06-18 11:30:00],[2024-06-18 13:00:00, 2024-06-18 15:00:00],
            // [2024-06-19 09:30:00, 2024-06-19 11:30:00],[2024-06-19 13:00:00, 2024-06-19 15:00:00],
            // [2024-06-20 09:30:00, 2024-06-20 11:30:00],[2024-06-20 13:00:00, 2024-06-20 15:00:00]
            // ]
            //返回的tick应该是
            // 2024-06-14 09:30:00,2024-06-17 09:30:00,
            // 2024-06-18 09:30:00,2024-06-19 09:30:00,
            // 2024-06-20 09:30:00,2024-06-20 15:00:00
            std::string GetTickText(const nlohmann::json &item, XChart *chart, std::size_t index) override {
                if (!isTimeRangeValid_) {
                    return "";
                }
//                nlohmann::json &timeRange = config_["timeRange"];
                int dateTickCount = json::GetIntNumber(this->config_, "tickCount", 5);
                if (index >= dateTickCount){
                    return "";
                }
                long long timeZoneOffset = 0;
                bool forceTimeZone = false;
                if(config_.contains("timeZoneOffset")) {
                    timeZoneOffset = config_["timeZoneOffset"];
                    forceTimeZone = true;
                }
                timeZoneOffset *= 1000;
                return xg::TimeStampToFormat(item.get<long long>() + timeZoneOffset, forceTimeZone, "%m-%d");
            }

        protected:
            //未对外暴露 在入口处已经check过TimeRange了，这里不需要判断
            nlohmann::json CalculateTicks() override {
                nlohmann::json rst;
                //timeRange格式为 @"timeRange": @[@[@(1608687000000), @(1608694200000)], @[@(1608699600000), @(1608706800000)]]
                nlohmann::json &timeRange = config_["timeRange"];
                for(std::size_t i = 0; i < timeRange.size(); ++i) {
                    nlohmann::json &range = timeRange[i];
                    //size为n的情况下rst的组合为
                    //{timeRange[0][0], timeRange[0][1], timeRange[1][1],...timeRange[N][1]}
                    if (i % 2 == 0) {
                        rst.push_back(range[0]);
                    } else if (i == timeRange.size() - 1){
                        rst.push_back(range[1]);
                    }
                }
                return rst;
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
            bool isTimeRangeValid_;
        };
    } // namespace scale
} // namespace xg


#endif //STOCK_PLOT_CORE_FIVEDAYSLINEAR_H
