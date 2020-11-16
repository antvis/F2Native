#ifndef XG_GRAPHICS_SCALE_IDENTITY_H
#define XG_GRAPHICS_SCALE_IDENTITY_H

#include "graphics/scale/Scale.h"

namespace xg {
namespace scale {
/**
 * 常量度量
 */
class Identity : public AbstractScale {
  public:
    Identity(const std::string &_field, const nlohmann::json &_values) : AbstractScale(_field, _values) {
        this->ticks = this->CalculateTicks();
    }

    ScaleType GetType() const noexcept override { return ScaleType::Identity; }

    void Change(const nlohmann::json &cfg = {}) override {}

    double Scale(const nlohmann::json &key) const override {
        if(values.size() > 0 && key.is_number() && values[0] == key) {
            return key;
        }
        return this->rangeMin;
    }

    nlohmann::json Invert(double key) override {
        if(values.size() > 0) {
            return values[0];
        }
        nlohmann::json ret;
        return ret;
    }

  protected:
    nlohmann::json CalculateTicks() override { return values; }
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_IDENTITY_H
