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
    Identity(const std::string &_field, const nlohmann::json &_values) : AbstractScale(_field, _values, {}) {
        this->ticks = this->CalculateTicks();
    }

    ScaleType GetType() const noexcept override { return ScaleType::Identity; }

    void Change(const ScaleCfg &cfg) override {}

    double Scale(const nlohmann::json &key) override {
        if(values.size() > 0 && key.is_number() && values[0] == key) {
            return key;
        }
        return config.range[0];
    }

    nlohmann::json Invert(double key) override {
        if(values.size() > 0) {
            return values[0];
        }
        nlohmann::json ret;
        return ret;
    }

  protected:
    vector<string> CalculateTicks() override { return {}; }
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_IDENTITY_H
