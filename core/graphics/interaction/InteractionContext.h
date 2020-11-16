#include "graphics/scale/Scale.h"
#include <array>
#include <nlohmann/json.hpp>

#ifndef XG_GRAPHICS_EVENT_INTERACTION_CTX_H
#define XG_GRAPHICS_EVENT_INTERACTION_CTX_H

namespace xg {
class XChart;
namespace interaction {
class InteractionContext {
  public:
    InteractionContext(XChart *chart);
    ~InteractionContext();

    void Start();

    void DoZoom(double leftScale, double rightScale, double zoom);

    void DoMove(double ratio);

    void UpdateRange(std::array<double, 2> newRange = {0, 1});

    void Repaint(nlohmann::json &newValues);

    void UpdateScale(const std::string &field, nlohmann::json cfg);

    void UpdateFollowScale(scale::AbstractScale &pinchScale, nlohmann::json &pinchValues);

    void UpdateTicks();

  protected:
    void OnAfterChartInit();

  private:
    XChart *chart_ = nullptr;
    std::array<double, 2> range_ = {0, 1};
    std::array<double, 2> startRange_ = {0, 1};
    double minCount_ = 10; // 缩放最小点数
    int lastTickCount_ = 0;
    double minScale_ = .0;
    nlohmann::json values_ = {};
};
} // namespace interaction
} // namespace xg

#endif // XG_GRAPHICS_EVENT_INTERACTION_CTX_H
