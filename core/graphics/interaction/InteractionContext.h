#ifndef XG_GRAPHICS_EVENT_INTERACTION_CTX_H
#define XG_GRAPHICS_EVENT_INTERACTION_CTX_H

#include <array>
#include "../scale/Scale.h"
#include "../../nlohmann/json.hpp"

namespace xg {
class XChart;
namespace interaction {
class InteractionContext {
  public:
    InteractionContext(XChart *chart);
    ~InteractionContext();

    void Start();

    bool DoZoom(double leftScale, double rightScale, double zoom);

    bool DoMove(double deltaX, double deltaY);

    bool UpdateRange(std::array<double, 2> newRange = {0, 1});

    bool Repaint(nlohmann::json &newValues, std::size_t valueStart, std::size_t valueEnd);

    void UpdateScale(const std::string &field, nlohmann::json cfg);

    void UpdateFollowScale(scale::AbstractScale &pinchScale, nlohmann::json &pinchValues, std::size_t valueStart, std::size_t valueEnd);

    void UpdateTicks();

    void SetTypeConfig(std::string type, nlohmann::json config);
    
    void Clear() {}

  protected:
    void OnAfterChartInit();

  private:
    XChart *chart_ = nullptr;
    std::array<double, 2> range_ = {0, 1};
    int lastTickCount_ = 0;
    double minScale_ = .0;
    nlohmann::json values_;
    nlohmann::json config_;
    std::size_t minCount_ = 10; // 缩放最小点数
    std::size_t maxCount_ = 0;
};
} // namespace interaction
} // namespace xg

#endif // XG_GRAPHICS_EVENT_INTERACTION_CTX_H
