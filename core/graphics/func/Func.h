#ifndef XG_GRAPHICS_FUNC_H
#define XG_GRAPHICS_FUNC_H

#include <stdlib.h>
#include <string>
#include <unordered_map>
#include "../../utils/xtime.h"
#include "../../nlohmann/json.hpp"


namespace xg {

namespace func {

static std::string MakeFunctionId() { return std::to_string(xg::CurrentTimestampAtMM()) + "-" + std::to_string(rand()); }

//// Java|OC <--> C+ 的 Function 都必须保持在同线程同步调用
struct F2Function {

    F2Function() { functionId = MakeFunctionId(); }

    virtual nlohmann::json Execute(nlohmann::json t) { return nlohmann::json(); }

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

    void Add(F2Function *function) {
        std::unique_lock<std::mutex> lock(mutex_);
        this->functions_[function->functionId] = function;
    }

    void Remove(std::string functionId) {
        std::unique_lock<std::mutex> lock(mutex_);
        functions_.erase(functionId);
    }

    F2Function *Find(std::string functionId) {
        std::unique_lock<std::mutex> lock(mutex_);
        auto it = this->functions_.find(functionId);
        if(it == this->functions_.end()) {
            return nullptr;
        }
        return it->second;
    }

    void Clear(const std::string hostChartId) {
        std::unique_lock<std::mutex> lock(mutex_);
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
    std::mutex mutex_;
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
