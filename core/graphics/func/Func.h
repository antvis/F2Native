#ifndef XG_GRAPHICS_FUNC_H
#define XG_GRAPHICS_FUNC_H

#include <stdlib.h>
#include <string>
#include <unordered_map>
#include "../../utils/xtime.h"
#include "../../reflection/reflection.h"


namespace xg {

namespace func {

static std::string MakeFunctionId() { return std::to_string(xg::CurrentTimestampAtMM()) + "-" + std::to_string(rand()); }

//// Java|OC <--> C+ 的 Function 都必须保持在同线程同步调用
struct F2Function {

    F2Function() { functionId = MakeFunctionId(); }
    
    virtual const std::unordered_map<std::string, Any> Execute(const std::string &functionId, const std::string &param) = 0;

    virtual ~F2Function() {}
    std::string functionId;
    std::string hostChartId;
};


} // namespace func
} // namespace xg

#endif // XG_GRAPHICS_FUNC_H
