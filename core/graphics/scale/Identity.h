#ifndef XG_GRAPHICS_SCALE_IDENTITY_H
#define XG_GRAPHICS_SCALE_IDENTITY_H

#include "Scale.h"

namespace xg {
namespace scale {
/**
 * 常量度量
 */
class Identity : public AbstractScale {
  public:
    Identity(const std::string &_field, const vector<const Any *> &_values) : AbstractScale(_field, _values, {}) {
        this->ticks = this->CalculateTicks();
    }

    ScaleType GetType() const noexcept override { return ScaleType::Identity; }

    void Change(const ScaleCfg &cfg) override {}

    double Scale(const Any &key) override {
        if(values.size() > 0 && key.GetType().IsNumber()) {
            return key.Cast<double>();
        }
        return config.range[0];
    }

    Any Invert(double key) override {
        if(values.size() > 0) {
            return values[0];
        }
        return Any();
    }

  protected:
    vector<string> CalculateTicks() override { return {}; }
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_IDENTITY_H
