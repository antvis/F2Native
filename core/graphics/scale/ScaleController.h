#ifndef XG_GRAPHICS_SCALE_CONTROLLER_H
#define XG_GRAPHICS_SCALE_CONTROLLER_H

#include <algorithm>
#include <unordered_map>
#include <vector>
#include "Category.h"
#include "Identity.h"
#include "KLineCat.h"
#include "TimeCategory.h"
#include "continuous/Linear.h"
#include "continuous/TimeSharingLinear.h"
#include "../canvas/Coord.h"
#include "../util/json.h"
#include "../util/json_util.h"
#include "../../utils/Tracer.h"
#include "../../utils/common.h"
#include "../../nlohmann/json.hpp"

namespace xg {
class XChart;
namespace scale {

nlohmann::json AdjustRange(const nlohmann::json &fieldColumn, std::unique_ptr<canvas::coord::AbstractCoord> &coord);

std::unique_ptr<AbstractScale> MakeCategory(const std::string &field_,
                                            const nlohmann::json &data,
                                            nlohmann::json &config,
                                            utils::Tracer *tracer,
                                            std::unique_ptr<canvas::coord::AbstractCoord> &coord,
                                            nlohmann::json &fieldColumn);

std::unique_ptr<AbstractScale> MakeLinear(const std::string &field_,
                                          const nlohmann::json &data,
                                          nlohmann::json &config,
                                          utils::Tracer *tracer,
                                          std::unique_ptr<canvas::coord::AbstractCoord> &coord,
                                          nlohmann::json &fieldColumn);

std::unique_ptr<AbstractScale> MakeScale(const std::string &field_,
                                         const nlohmann::json &data,
                                         nlohmann::json &config,
                                         utils::Tracer *tracer,
                                         std::unique_ptr<canvas::coord::AbstractCoord> &coord);

class ScaleController {
  public:
    ScaleController(XChart *chart): chart_(chart) {}
    const std::unique_ptr<AbstractScale> &CreateScale(const std::string &field_,
                                                      const nlohmann::json &data,
                                                      utils::Tracer *tracer,
                                                      std::unique_ptr<canvas::coord::AbstractCoord> &coord);

    void UpdateColConfig(const std::string &field, nlohmann::json cfg) { colConfigs[field] = cfg; }

    inline bool Empty() { return scales_.empty(); }

    inline void Clear() {
        scales_.clear();
        colConfigs = {};
    };
    
    inline size_t ScaleCount() {
        return scales_.size();
    }
    
    void SyncYScale();
    void SyncYScale(const size_t valueStart, const size_t valueEnd, bool nice);
    void UpdateScale(const std::string &field, const nlohmann::json &cfg);
    void AdjustScale();

  private:
    std::vector<std::unique_ptr<AbstractScale>> scales_{};
    nlohmann::json colConfigs;
    XChart *chart_ = nullptr;
};
} // namespace scale
} // namespace xg

#endif // XG_GRAPHICS_SCALE_CONTROLLER_H
