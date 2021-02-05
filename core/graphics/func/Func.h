#include "utils/xtime.h"
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <unordered_map>

#ifndef XG_GRAPHICS_FUNC_H
#define XG_GRAPHICS_FUNC_H

namespace xg {

namespace func {

static std::string MakeFunctionId() {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 100000);

    return std::to_string(xg::CurrentTimestampAtMM()) + "-" + std::to_string(distrib(gen));
}

//// Java|OC <--> C+ 的 Function 都必须保持在同线程同步调用
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

static nlohmann::json InvokeFunction(const std::string &functionId, const nlohmann::json &param) {
    F2Function *func = FunctionManager::GetInstance().Find(functionId);
    if(func == nullptr) {
        return nlohmann::json();
    }
    return func->Execute(param);
}

} // namespace func
} // namespace xg

#endif // XG_GRAPHICS_FUNC_H
