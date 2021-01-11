#include "utils/xtime.h"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

#ifndef XG_GRAPHICS_FUNC_H
#define XG_GRAPHICS_FUNC_H

namespace xg {

namespace func {

static std::string MakeFunctionId() { return std::to_string(xg::CurrentTimestampAtMM()); }

// Java|OC <--> C+ 的 Function 都必须保持在同线程同步调用
struct F2Function {

    F2Function() { functionId = MakeFunctionId(); }

    virtual nlohmann::json Execute(nlohmann::json t) = 0;

    virtual ~F2Function() {}
    std::string functionId;
    std::string hostChartId;
};

class FunctionManager {
  public:
    static FunctionManager &GetInstance() {
        static FunctionManager manager;
        return manager;
    }

    FunctionManager(FunctionManager const &) = delete;
    void operator=(FunctionManager const &) = delete;

    void Add(F2Function *function) { this->functions_[function->functionId] = function; }

    void Remove(std::string functionId) { functions_.erase(functionId); }

    F2Function *Find(std::string functionId) { return this->functions_[functionId]; }

    void Clear(const std::string hostChartId) {
        for(auto it = functions_.begin(); it != functions_.end();) {
            if(it->second != nullptr && it->second->hostChartId == hostChartId) {
                delete it->second;
                it = functions_.erase(it);
            } else {
                it++;
            }
        }
    }

  private:
    FunctionManager() {}

  private:
    std::unordered_map<std::string, F2Function *> functions_;
};

} // namespace func
} // namespace xg

#endif // XG_GRAPHICS_FUNC_H