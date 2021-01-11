#include "graphics/canvas/CanvasContext.h"
#include "graphics/canvas/Coord.h"
#include "graphics/scale/Scale.h"
#include "graphics/shape/Group.h"
#include "graphics/shape/Text.h"
#include "graphics/util/Point.h"
#include <array>
#include <nlohmann/json.hpp>
#include <string>
#include <utils/common.h>
#include <vector>

#ifndef XG_GRAPHICS_AXIS_CONTROLLER_H
#define XG_GRAPHICS_AXIS_CONTROLLER_H

using namespace xg::util;

namespace xg {

class XChart;
namespace axis {

class Axis final {
  public:
    Axis() {}
    Axis(const Axis &) = delete;
    ~Axis() {}
    std::string key;
    std::string type;
    std::string dimType;
    std::string position;
    std::vector<std::unique_ptr<shape::Text>> labels;

    double maxWidth = 0;
    double maxHeight = 0;
    xg::util::Point start;
    xg::util::Point end;
    std::vector<std::vector<util::Point>> gridPoints{}; // 网格 grid 矩阵
    nlohmann::json gridCfg;
    nlohmann::json labelCfg;
    nlohmann::json lineCfg;
    std::string field;
    std::string verticalField; // 相对垂直边 field
    std::vector<scale::Tick> ticks;
};

class AxisController {
  public:
    AxisController(shape::Group *_container) : container_(_container) {}

    ~AxisController() { container_ = nullptr; }

    void DrawAxes(XChart *chart, canvas::CanvasContext &context);

    void SetFieldConfig(std::string field, nlohmann::json config = {}) { axisConfig_[field] = MergeDefaultConfig(config); }

    void Clear();

  protected:
    static nlohmann::json MergeDefaultConfig(const nlohmann::json &config) {
        nlohmann::json gridCfg = {
            {"type", "line"},      // 网格线类型
            {"lineWidth", 1.0f},   // 网格线线宽
            {"stroke", "#E8E8E8"}, // 网格线颜色
        };

        nlohmann::json line = {
            // 轴线配置
            {"color", "#999"},
            {"lineWidth", 1.0f},
            {"type", "line"}, // 线类型
        };

        nlohmann::json label = {
            // 轴标签配置
            {"textColor", "#808080"}, // 标签文字颜色
            {"textSize", 10.f},       // 标签文字字号
            {"labelMargin", 0.f},     // 轴上标签的外边距. x 轴为左右外边距， y 轴为上下外边距
            {"labelOffset", 0.f}, // 轴上标签的垂直方向偏移量。x 轴为上下的整体偏移量， y 轴为左右的整体偏移量
            {"textAlign", "center"},  {"textBaseline", "bottom"},
        };

        nlohmann::json cfg = {{"label", label},  // 标签
                              {"grid", gridCfg}, // 网格
                              {"line", line},    // 轴
                              {"hidden", false}};
        if(config.is_object()) {
            cfg.merge_patch(config);
        }
        return cfg;
    }

  private:
    void InitAxis(XChart &chart, const std::string &field, const std::string &dimType, const std::string &verticalField);
    void InitAxisConfig(XChart &chart);

    void DrawAxis(XChart &chart, std::unique_ptr<Axis> &axis, canvas::CanvasContext &context);

    void DrawLabel(XChart &chart, std::unique_ptr<Axis> &axis, canvas::CanvasContext &context);

    void DrawLine(std::array<util::Point, 2> &&line, const nlohmann::json &lineCfg);

    util::Point GetOffsetPoint(std::unique_ptr<Axis> &axis, double value) {
        util::Point point;
        point.x = axis->start.x + (axis->end.x - axis->start.x) * value;
        point.y = axis->start.y + (axis->end.y - axis->start.y) * value;
        return point;
    }

    void GetSidePoint(util::Point &point, double labelOffset, std::string type) {
        util::Point offset = GetOffsetVector(point, labelOffset, type);
        point.x = point.x + offset.x;
        point.y = point.y + offset.y;
    }

    util::Point GetOffsetVector(util::Point point, double labelOffset, std::string type) {
        util::Point offset;
        if(type == "x") {
            offset.x = 0;
            offset.y = labelOffset;
        } else {
            offset.x -= labelOffset;
            offset.y = 0;
        }
        return offset;
    }

    std::string GetLinePosition(const std::string &dimType, int index, bool transposed) {
        if(dimType == "x") {
            return transposed ? "left" : "bottom";
        } else if(dimType == "y") {
            std::string position = index > 0 ? "right" : "left";
            if(transposed) {
                position = "bottom";
            }
            return position;
        }
        return "";
    }

    std::array<util::Point, 2> GetLineCfg(canvas::coord::AbstractCoord &coord, std::string dimType, std::string position) {
        util::Point start{};
        util::Point end{};
        if(dimType == "x") {
            start.x = 0;
            start.y = 0;
            end.x = 1;
            end.y = 0;
        } else {
            if(position == "right") {
                start.x = 1;
                start.y = 0;
                end.x = 1;
                end.y = 1;
            } else {
                start.x = 0;
                start.y = 0;
                end.x = 0;
                end.y = 1;
            }
        }
        start = coord.ConvertPoint(start);
        end = coord.ConvertPoint(end);
        return std::array<util::Point, 2>{start, end};
    }

    std::array<util::Point, 2> GetRadiusCfg(canvas::coord::AbstractCoord &coord) {
        util::Point start = util::Point(0, 0);
        util::Point end = util::Point(0, 1);
        if(coord.IsTransposed()) {
            end.Reset(1, 0);
        }
        start = coord.ConvertPoint(start);
        end = coord.ConvertPoint(end);
        return std::array<util::Point, 2>{start, end};
    }

    void UpdateLayout(XChart &chart);

  private:
    std::vector<std::unique_ptr<Axis>> axes;
    nlohmann::json axisConfig_;
    shape::Group *container_ = nullptr;
};
} // namespace axis
} // namespace xg
#endif // XG_GRAPHICS_AXIS_CONTROLLER_H
