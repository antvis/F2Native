#ifndef XG_GRAPHICS_SCALE_H
#define XG_GRAPHICS_SCALE_H

#include <string>
#include <vector>
#include <map>
#include "../func/Func.h"
#include "../util/json.h"
#include "../../reflection/reflection.h"

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
};

bool IsCategory(ScaleType type);
bool IsLinear(ScaleType type);

struct Tick {
    // 转换后的 tick 值。
    // TODO 增加可以由用户自定义的转换函数。
    std::string text;
    Any tickValue; // 原始 tick 值
    double value;             // 值域值
};

struct ScaleCfg {
    std::string field;//冗余一个field
    std::string type;
    std::vector<float> range= {NAN, NAN};

    float tickCount = NAN;  // 刻度总数
    double min = NAN;
    double max = NAN;
    //回调方法
    std::string tick;

    vector<std::string> ticks;
    
    bool nice = true;
    int precision = 0;
    
    //cat
    std::vector<float> domain = {NAN, NAN};
    
    //分时
    vector<vector<long long>> timeRange;
    long long timeZoneOffset = 0;
    
    //kline
    std::string dateFormate = "";
    
    inline bool HasRange() {
        return !std::isnan(range[0]) && !std::isnan(range[1]);
    }
    
    inline bool HasDomain() {
        return !std::isnan(domain[0]) && !std::isnan(domain[1]);
    }
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(ScaleCfg)
        FIELDS(FIELD(&ScaleCfg::field),
               FIELD(&ScaleCfg::type),
               FIELD(&ScaleCfg::range),
               FIELD(&ScaleCfg::tickCount),
               FIELD(&ScaleCfg::min),
               FIELD(&ScaleCfg::max),
               FIELD(&ScaleCfg::tick),
               FIELD(&ScaleCfg::ticks),
               FIELD(&ScaleCfg::nice),
               FIELD(&ScaleCfg::precision),
               FIELD(&ScaleCfg::domain),
               FIELD(&ScaleCfg::timeRange),
               FIELD(&ScaleCfg::timeZoneOffset),
               FIELD(&ScaleCfg::dateFormate))
        CTORS(DEFAULT_CTOR(ScaleCfg))
    END_TYPE
#endif
};


/**
 *  度量基类
 */
class AbstractScale {
  public:
    AbstractScale(const std::string &_field, const vector<const Any *> &_values, const ScaleCfg &_cfg) : field(_field), config(_cfg) {
        this->values = _values;
    }
    virtual ~AbstractScale() {}

    // 度量类型 [Identity | Cat | TimeCat | Linear]
    virtual ScaleType GetType() const noexcept = 0;

    virtual void Change(const ScaleCfg &cfg) = 0;

    // 将定义域值转换为值域值
    virtual double Scale(const Any &key) = 0;

    // 将值域值转换为定义域值
    virtual Any Invert(double key) = 0;

    // 重置度量, 重新生成 ticks
    void Reset() { this->ticks = this->CalculateTicks(); }

    std::vector<Tick> GetTicks(XChart *chart) {
        std::vector<Tick> ticks_;
        ticks_.reserve(ticks.size());

        for(size_t i = 0; i < ticks.size(); i++) {
            //warning must be const std::string &
            const std::string &item = ticks[i];
            scale::Tick tick;
            tick.text = this->GetTickText(item, chart);
            tick.value = this->Scale(item);
            tick.tickValue = item;
            ticks_.push_back(std::move(tick));
        }
        return ticks_;
    }

    inline const std::string &GetField() const { return field; }

    virtual inline std::size_t GetValuesSize() noexcept { return values.size(); }

    virtual std::string GetTickText(const Any &item, XChart *chart);
    
    virtual inline double GetMax() noexcept {return config.max;}
    virtual inline double GetMin() noexcept {return config.min;}
    inline const ScaleCfg &GetConfig() noexcept { return config;}
  public:
    //轴上的标签
    vector<std::string> ticks;

    //配置的字段
    ScaleCfg config;
    
    //度量字段名称
    std::string field;
  protected:
    virtual vector<std::string> CalculateTicks() = 0;
  public:
    vector<const Any *> values;
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_H
