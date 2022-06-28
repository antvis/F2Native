#ifndef XG_GRAPHICS_ADJUST_STACK_H
#define XG_GRAPHICS_ADJUST_STACK_H

#include <array>
#include <limits>
#include <math.h>
#include "../../utils/common.h"
#include "../../nlohmann/json.hpp"

namespace xg {
namespace adjust {

class Stack {
  public:
    static void processStack(const std::string &xField, const std::string &yField, XDataGroup &dataArray) {
        std::map<std::string, float> positive;
        for(size_t i = 0; i < dataArray.size(); i++) {
            auto &data = dataArray[i];
            for(size_t j = 0; j < data.size(); j++) {
                auto &item = data[j];
                auto y = (*item.data)[yField];
                if(y.is_number() && (*item.data)[xField].is_string()) {
                    if(y >= XG_EPS) {
                        const std::string &xkey = (*item.data)[xField];
                        double y_d = y.is_array() ? y[1] : y;
                        item.adjust.push_back(positive[xkey]);
                        item.adjust.push_back(static_cast<float>(y_d + positive[xkey]));
                        positive[xkey] = y_d + positive[xkey];
                    }
                }
            }
        }
    }
};
} // namespace adjust
} // namespace xg
#endif // XG_GRAPHICS_ADJUST_STACK_H
