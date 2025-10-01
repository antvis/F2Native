#ifndef XG_GRAPHICS_SCALE_H
#define XG_GRAPHICS_SCALE_H

#include <string>
#include <vector>
#include <map>
#include "../func/Func.h"
#include "../../nlohmann/json.hpp"

namespace xg {
class XChart;
namespace scale {

enum class ScaleType {
    Identity = 1,
    Cat,
    TimeCat,
    Kline,
    Linear,
    TimeSharingLinear,
    FiveDaysLinear
};

static bool IsCategory(ScaleType type) {
    return type == ScaleType::Cat || type == ScaleType::TimeCat || type == ScaleType::Kline;
}
static bool IsLinear(ScaleType type) {
    return type == ScaleType::Linear || type == ScaleType::TimeSharingLinear || type == ScaleType::FiveDaysLinear;
}

class Tick {
  public:
    // 转换后的 tick 值。
    // TODO 增加可以由用户自定义的转换函数。
    std::string text;
    nlohmann::json tickValue; // 原始 tick 值
    double value;             // 值域值
};

/**
 *  度量基类
 */
class AbstractScale {
  public:
    AbstractScale(const std::string &_field, const nlohmann::json &_values) : field(_field) {
        if(_values.is_array()) {
            this->values = _values;
        }
    }
    virtual ~AbstractScale() {}

    // 度量类型 [Identity | Cat | TimeCat | Linear]
    virtual ScaleType GetType() const noexcept = 0;

    virtual void Change(const nlohmann::json &cfg = {}) = 0;

    // double Scale(const nlohmann::json &key) {
    //     util::Data _key = util::ConvertJsonToData(key);
    //     return Scale(_key);
    // }

    // 将定义域值转换为值域值
    virtual double Scale(const nlohmann::json &key) = 0;

    // 将值域值转换为定义域值
    virtual nlohmann::json Invert(double key) = 0;

    // 重置度量, 重新生成 ticks
    void Reset() { this->ticks = this->CalculateTicks(); }

    std::vector<Tick> GetTicks(XChart *chart) {
        std::vector<Tick> ticks_;

        for(size_t i = 0; i < ticks.size(); i++) {
            nlohmann::json &item = ticks[i];
            scale::Tick tick;
            tick.text = this->GetTickText(item, chart, i);
            tick.value = this->Scale(item);
            tick.tickValue = item;
            ticks_.push_back(std::move(tick));
        }
        return ticks_;
    }

    inline const std::string &GetField() const { return field; }

    virtual inline std::size_t GetValuesSize() noexcept { return values.size(); }

    virtual std::string GetTickText(const nlohmann::json &item, XChart *chart, std::size_t index);
  public:
    std::string field;     // 度量字段名称
    double rangeMin = 0.0; // 取值范围
    double rangeMax = 1.0; // 取值范围
    size_t tickCount = 5;  // 刻度总数
    double min = 0;
    double max = 0;

  protected:
    virtual nlohmann::json CalculateTicks() = 0;
  public:
    // 刻度值, 通过 wilkinson 算法计算得出
    nlohmann::json ticks; // 数组
    nlohmann::json values;
    std::string tickCallbackId;
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_H
