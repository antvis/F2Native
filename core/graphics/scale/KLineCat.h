#ifndef XG_GRAPHICS_SCALE_CAT_KLINE_H
#define XG_GRAPHICS_SCALE_CAT_KLINE_H

#include <set>
#include <string>
#include "Category.h"
#include "../../utils/StringUtil.h"
#include "../../utils/xtime.h"

namespace xg {
namespace scale {

class KLineCat : public Category {
  public:
    KLineCat(const std::string &_field, const vector<Any> &_values, const ScaleCfg &config)
        : Category(_field, _values, config) {
        // ["kline-day", "kline-week", "kline-month", "kline-minutes-1", "kline-minutes-5", "kline-minutes-15", "kline-minutes-30", "kline-minutes-60", "kline-minutes-120"]
            
        std::vector<std::string> lineTypes;
        StringUtil::Split(config.type, lineTypes, '-');
        if (lineTypes.size() >= 3) {
            kLineType_ = lineTypes[1];
            minutes_ = std::stoi(lineTypes[2]);
        } else if(lineTypes.size() >= 2) {
            kLineType_ = lineTypes[1];
        }
        this->PreProcessTicks();
        Change(config);
    }

    ScaleType GetType() const noexcept override { return ScaleType::Kline; }

    void Change(const ScaleCfg &cfg) override {
        Category::Change(cfg);
    }

  public:
    vector<string> CalculateTicks() override {
        if(kLineType_ == "minutes") {
            return CalculateMinutesTicks();
        } else {
            return CalculateDaysTicks();
        }
    }

  private:
    vector<string> CalculateDaysTicks() {
        std::size_t start = static_cast<std::size_t>(min);
        std::size_t end = static_cast<std::size_t>(max);

        std::size_t intervalStep = 1;
        if(kLineType_ == "day") {
            intervalStep = 1;
        } else if(kLineType_ == "week") {
            intervalStep = 2;
        } else if(kLineType_ == "month") {
            intervalStep = 11;
        }

        std::vector<std::size_t> indicators;
        for(std::size_t index = start; index <= end; ++index) {
            auto &item = values[index];

            std::size_t itemHash = item.hash();
            if(allTicksCache_.find(itemHash) != allTicksCache_.end()) {
                indicators.emplace_back(index);
            }
        }

        vector<string> rst;
        for(std::size_t index = 0; index < indicators.size(); index += intervalStep) {
            rst.push_back(values[indicators[index]]);
        }
        return rst;
    }

    vector<string> CalculateMinutesTicks() {
        std::size_t columnCount = this->GetValuesSize();
        std::size_t start = static_cast<std::size_t>(min);
        std::size_t end = static_cast<std::size_t>(max);

        std::size_t timeRange = minutes_ * columnCount;
        std::size_t timeStep = 1;
        if(timeRange <= 240) {
            // if timeRange less 4 hour, set interval delta is 30 minutes.
            timeStep = 1;
        } else if(240 < timeRange && timeRange <= 720) {
            // set interval is 60 minutes.
            timeStep = 2;
        } else {
            // set interval is 6 days.
            timeStep = 21;
        }

        std::vector<std::size_t> indicators;
        for(std::size_t index = start; index <= end; index++) {
            auto &item = values[index];

            std::size_t itemHash = item.hash();
            if(allTicksCache_.find(itemHash) != allTicksCache_.end()) {
                indicators.emplace_back(index);
            }
        }

        vector<string> rst;
        for(std::size_t index = 0; index < indicators.size(); index += timeStep) {
            rst.push_back(values[indicators[index]]);
        }
        return rst;
    }

    std::tm ConvertDataToTS(Any &data) {
        if(data.GetType().IsString()) {
            // date str
            if(!config.dateFormate.empty()) {
                return DateParserAtTM(data.Cast<std::string>(), config.dateFormate);
            }
            return DateParserAtTM(data.Cast<std::string>());
        } else if(data.GetType().IsNumber()) {
            // timestamp
            long long t = data.Cast<long long>();
            long timeZoneOffset = 0;
            bool forceTimeZone = false;
            if(config.timeZoneOffset != 0) {
                timeZoneOffset = config.timeZoneOffset;
                forceTimeZone = true;
            }
            timeZoneOffset *= 1000;
            return xg::DateParserTimeStamp(t + timeZoneOffset, forceTimeZone);
        } else {
            return std::tm{};
        }
    }

    void PreProcessTicks() {
        if(kLineType_ == "minutes") {
            for(std::size_t step = 0; step < values.size() - 1; step++) {
                auto &cur = values[step];
                auto &next = values[step + 1];

                std::tm curTime = ConvertDataToTS(cur);
                std::tm nextTime = ConvertDataToTS(next);
                if(curTime.tm_mday != nextTime.tm_mday) {
                    allTicksCache_[next.hash()] = nextTime;
                    step++;
                } else {
                    if(curTime.tm_min % 30 == 0) {
                        allTicksCache_[cur.hash()] = curTime;
                    } else {
                        continue;
                    }
                }
            }
        } else {
            for(std::size_t step = 0; step < values.size() - 1; step++) {
                auto &cur = values[step];
                auto &next = values[step + 1];

                std::tm curTime = ConvertDataToTS(cur);
                std::tm nextTime = ConvertDataToTS(next);
                if(curTime.tm_mon != nextTime.tm_mon) {
                    allTicksCache_[next.hash()] = nextTime;
                    step++;
                }
            }
        }
    }

  private:
    std::unordered_map<std::size_t, std::tm> allTicksCache_;
    std::string kLineType_;
    std::size_t minutes_;
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_CAT_KLINE_H
