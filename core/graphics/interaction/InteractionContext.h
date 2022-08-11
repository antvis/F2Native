#ifndef XG_GRAPHICS_EVENT_INTERACTION_CTX_H
#define XG_GRAPHICS_EVENT_INTERACTION_CTX_H

#include <array>
#include "../scale/Scale.h"
#include "../../nlohmann/json.hpp"
#include "../../reflection/reflection.h"

namespace xg {
class XChart;
namespace interaction {

struct PinchCfg {
    float minCount = NAN;
    float maxCount = NAN;
    
    //是否设置了pinch
    bool enable = false;
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(PinchCfg)
        FIELDS(FIELD(&PinchCfg::minCount),
               FIELD(&PinchCfg::maxCount),
               FIELD(&PinchCfg::enable))
        CTORS(DEFAULT_CTOR(PinchCfg))
    END_TYPE
#endif
};
extern void from_json(const nlohmann::json& j, PinchCfg& p);

struct PanCfg {
    //是否设置了pan
    bool enable = false;
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(PanCfg)
        FIELDS(FIELD(&PanCfg::enable))
        CTORS(DEFAULT_CTOR(PanCfg))
    END_TYPE
#endif
};
extern void from_json(const nlohmann::json& j, PanCfg& p);

class InteractionContext {
  public:
    InteractionContext(XChart *chart);
    ~InteractionContext();

    void Start();

    bool DoZoom(double leftScale, double rightScale, double zoom);

    bool DoMove(double deltaX, double deltaY);

    bool UpdateRange(std::array<double, 2> newRange = {0, 1});

    bool Repaint(const vector<Any> &newValues, std::size_t valueStart, std::size_t valueEnd);

    void UpdateScale(const std::string &field, const scale::ScaleCfg &cfg);

    void UpdateFollowScale(scale::AbstractScale &pinchScale, const vector<Any> &pinchValues, std::size_t valueStart, std::size_t valueEnd);

    void UpdateTicks();

    inline void SetTypeConfig(const std::string &type, const PinchCfg &config) {
        pinch_ = config;
    }
    inline void SetTypeConfig(const std::string &type, const PanCfg &config) {
        pan_ = config;
    }
    
    void Clear() {}

  protected:
    void OnAfterChartInit();

  private:
    XChart *chart_ = nullptr;
    std::array<double, 2> range_ = {0, 1};
    int lastTickCount_ = 0;
    double minScale_ = .0;
    std::vector<Any> values_;
    PinchCfg pinch_;
    PanCfg pan_;
    size_t minCount_ = 10;
    size_t maxCount_ = 0;
};
} // namespace interaction
} // namespace xg

#endif // XG_GRAPHICS_EVENT_INTERACTION_CTX_H
