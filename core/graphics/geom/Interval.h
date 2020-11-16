#ifndef XG_GRAPHICS_GEOM_INTERVAL_H
#define XG_GRAPHICS_GEOM_INTERVAL_H

#include "graphics/geom/Geom.h"

namespace xg {
namespace geom {

class Interval : public AbstractGeom {
  public:
    Interval(utils::Tracer *tracer) : AbstractGeom(tracer) {
        type_ = "interval";
        generatePoints_ = true;
    }
    float GetDefaultWidthRatio(XChart &chart);

    nlohmann::json CreateShapePointsCfg(XChart &chart, nlohmann::json &item, size_t index); // {x, y, y0, size}

    void BeforeMapping(XChart &chart, nlohmann::json &dataArray) override;

    nlohmann::json getRectPoints(nlohmann::json &cfg);

    virtual void Draw(XChart &chart, Group &container, const nlohmann::json &groupData) const override;

    // 柱子上标签配置
    Interval &Tag(const nlohmann::json config = {}) {
        nlohmann::json defaultCfg = {{"offset", -5}, // 距离柱子顶部的偏移量
                                     {"textAlign", "center"},
                                     {"textBaseline", "bottom"},
                                     {"fill", "#808080"},
                                     {"textSize", 10.f}};

        if(config.is_object()) {
            defaultCfg.merge_patch(config);
        }
        tagConfig_ = defaultCfg;
        return *this;
    }

    // 样式配置
    Interval &Style(const nlohmann::json config = {}) {
        nlohmann::json defaultCfg = {{"lineWidth", 1}, // 距离柱子顶部的偏移量
                                     {"stroke", "#fff"}};

        if(config.is_object()) {
            defaultCfg.merge_patch(config);
        }
        styleConfig_ = defaultCfg;
        return *this;
    }

  private:
    nlohmann::json tagConfig_;
    nlohmann::json styleConfig_;
};
} // namespace geom
} // namespace xg

#endif /* XG_GRAPHICS_GEOM_INTERVAL_H */
