#ifndef XG_GRAPHICS_SCALE_TIME_CAT_H
#define XG_GRAPHICS_SCALE_TIME_CAT_H

#include <time.h>
#include "../scale/Category.h"

namespace xg {
namespace scale {
class TimeCategory : public Category {
  public:
    TimeCategory(const std::string &_field,
                 const vector<const Any *> &_values,
                 const ScaleCfg &config,
                 std::string mask = "HH::mm")
        : Category(_field, _values, config), mask(mask) {}

    ScaleType GetType() const noexcept override { return ScaleType::TimeCat; }

    std::string GetTickText(const Any &item, XChart *chart) override {
        if(item.GetType().IsString()) {
            return item.Cast<std::string>();
        } else if(item.GetType().IsArray()) {
            struct tm *p;
            time_t t;
            t = item.Cast<int>();
            p = gmtime(&t);
            char s[100];
            strftime(s, 100, "%H:%M", p);
            return std::string(s);
        } else {
            return "";
        }
    }

  protected:
    std::string mask;
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_TIME_CAT_H
