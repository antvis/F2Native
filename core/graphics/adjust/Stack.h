#ifndef XG_GRAPHICS_ADJUST_STACK_H
#define XG_GRAPHICS_ADJUST_STACK_H

#include <array>
#include <limits>
#include <math.h>
#include "../../utils/common.h"

namespace xg {
namespace adjust {

class Stack {
  public:
    static void processStack(const std::string &xField, const std::string &yField, XDataGroup &dataArray) {
        std::map<std::string, double> positive;
        for(size_t i = 0; i < dataArray.size(); i++) {
            auto &data = dataArray[i];
            for(size_t j = 0; j < data.size(); j++) {
                auto &item = data[j];
                auto &x = item.data.at(xField);
                auto &y = item.data.at(yField);
                if(y.GetType().IsNumber() && x.GetType().IsString()) {
                    auto y_d = y.Cast<double>();
                    if(y_d >= XG_EPS) {
                        const std::string &xkey = x.Cast<std::string>();
                        item.adjust.push_back(positive[xkey]);
                        item.adjust.push_back(y_d + positive[xkey]);
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
