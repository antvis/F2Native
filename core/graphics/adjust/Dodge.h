#include <nlohmann/json.hpp>

#ifndef XG_GRAPHICS_ADJUST_DODGE_H
#define XG_GRAPHICS_ADJUST_DODGE_H

#define ADJUST_DODGE_RATIO 0.5
#define ADJUST_MARGIN_RATIO 0.05

namespace xg {
namespace adjust {
class Dodge {
  public:
    static void processDodge(const std::string &xField, const std::string &yField, nlohmann::json &dataArray) {
        std::size_t count = dataArray.size();
        if(count == 0 || !dataArray.is_array()) {
            return;
        }

        for(std::size_t groupIdx = 0; groupIdx < count; ++groupIdx) {
            nlohmann::json &groupData = dataArray[groupIdx];

            for(std::size_t index = 0, len = groupData.size(); index < len; ++index) {
                nlohmann::json &obj = groupData[index];
                double value = obj[xField];

                double pre = len == 1 ? value - 1 : value - 0.5;
                double next = len == 1 ? value + 1 : value + 0.5;

                std::array<double, 2> range = {pre, next};
                double dodgeValue = GetDodgeOffset(range, groupIdx, count);
                obj[xField] = dodgeValue;
            }
        }
    }

  private:
    static double GetDodgeOffset(std::array<double, 2> range, std::size_t index, std::size_t count) {
        double pre = range[0];
        double next = range[1];

        double tickLength = next - pre;
        double width = tickLength * ADJUST_DODGE_RATIO / count;
        double margin = ADJUST_MARGIN_RATIO * width;
        double offset = 1.0 / 2.0 * (tickLength - count * width - (count - 1) * margin) +
                        ((index + 1) * width + index * margin) - 1.0 / 2.0 * width - 1.0 / 2.0 * tickLength;

        return (pre + next) / 2 + offset;
    }
};
} // namespace adjust
} // namespace xg

#endif // XG_GRAPHICS_ADJUST_DODGE_H
