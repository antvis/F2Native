#ifndef XG_GRAPHICS_AXIS_CONTROLLER_H
#define XG_GRAPHICS_AXIS_CONTROLLER_H

#include <array>
#include <string>
#include <vector>
#include "../canvas/CanvasContext.h"
#include "../canvas/Coord.h"
#include "../scale/Scale.h"
#include "../shape/Group.h"
#include "../shape/Text.h"
#include "../util/Point.h"
#include "../../utils/common.h"
#include "../../nlohmann/json.hpp"

using namespace xg::util;

namespace xg {

class XChart;
namespace axis {

class Axis {
  public:
    Axis() {}
    Axis(const Axis &) = delete;
    virtual ~Axis() {}
    std::string key;
    std::string type;
    std::string dimType;
    std::string position;
    std::vector<BBox> labelBBoxes;
    std::vector<std::unique_ptr<shape::Text>> labels;

    double maxWidth = 0;
    double maxHeight = 0;
    xg::util::Point start;
    xg::util::Point end;
    std::vector<std::vector<util::Point>> gridPoints{}; // 网格 grid 矩阵
    nlohmann::json gridCfg;
    nlohmann::json labelCfg;
    nlohmann::json lineCfg;
    nlohmann::json selectedLabelCfg;
    nlohmann::json selectedGridCfg;
    nlohmann::json selectedArray;
    bool selectionEnable = false;
    bool associateRecord = true;
    bool deselect = false;
    std::string field;
    std::string verticalField; // 相对垂直边 field
    std::vector<scale::Tick> ticks;
    
    virtual util::Point GetOffsetPoint(double value, double offset) const {
        util::Point point;
        point.x = start.x + (end.x - start.x) * value;
        point.y = start.y + (end.y - start.y) * value;
        return point;
    }
    
    const bool IsSelected(const int tickIndex);
    inline const nlohmann::json &GetLabelCfg();
    const nlohmann::json GetLabelCfg(const bool isSelected);
    const nlohmann::json GetLabelCfgForIndex(const int tickIndex);
    
    const std::tuple<std::vector<size_t>, std::vector<xg::scale::Tick>> GetSnapTicks(XChart *chart, util::Point point, const bool needReMapping);
};


class Circle : public Axis {
public:
    double startAngle;
    double endAngle;
    double radius;
    util::Point center;
    
    util::Point GetOffsetPoint(double value, double offset) const override {
        double angle = startAngle + (endAngle - startAngle) * value;
        return GetCirclePoint(angle, offset);
    }
    
    util::Point GetCirclePoint(double angle, double offset) const {
        return util::Point (
            center.x + std::cos(angle) * (radius + offset),
            center.y + std::sin(angle) * (radius + offset)
        );
    }
};

class AxisController {
  public:
    AxisController(shape::Group *_container) : container_(_container) {}

    ~AxisController() { container_ = nullptr; }

    void DrawAxes(XChart *chart, canvas::CanvasContext &context);
    
    void associateGeomTap(XChart *chart, util::Point point, nlohmann::json &geomRecords);
    const std::tuple<const bool, const bool, const nlohmann::json, const xg::scale::Tick> onTap(XChart *chart, util::Point point);

    void SetFieldConfig(const std::string &field, const nlohmann::json &config = {}) { axisConfig_[field] = MergeDefaultConfig(config); }
    
    void SetFieldSelection(const std::string &field, const nlohmann::json &selection = {}) { axisSelection_[field] = MergeDefaultSelection(selection); }

    void Clear();
    
    const std::string GetAxisPosition(const std::string &field);

  protected:
    static nlohmann::json MergeDefaultSelection(const nlohmann::json &selection) {
        nlohmann::json cfg = {{"enable", false}};
        if (selection.is_object()) {
            cfg.merge_patch(selection);
            cfg["enable"] = true;
        }
        return cfg;
    }
    static nlohmann::json MergeDefaultConfig(const nlohmann::json &config) {
        nlohmann::json gridCfg = {{"type", "line"},      // 网格线类型
                                  {"lineWidth", .6f},    // 网格线线宽
                                  {"stroke", "#E8E8E8"}, // 网格线颜色
                                  {"dash", {10, 10}}}; 

        nlohmann::json line = {// 轴线配置
                               {"color", "#999999"},
                               {"lineWidth", .6f},
                               {"type", "line"}, // 线类型
                               {"dash", {10, 10}}};

        nlohmann::json label = {
            // 轴标签配置
            {"textColor", "#808080"}, // 标签文字颜色
            {"textSize", 10.f},       // 标签文字字号
            {"labelMargin", 0.f},     // 轴上标签的外边距. x 轴为左右外边距， y 轴为上下外边距
            {"labelOffset", 0.f},     // 轴上标签的垂直方向偏移量。x 轴为上下的整体偏移量， y 轴为左右的整体偏移量
            {"textAlign", "center"},  //文字水平对齐方式 start center end
            {"textBaseline", "bottom"},//文字垂直对齐方式 top middle bottom
            {"inner", false}            //轴上的文字是否在图表内部
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
    void InitAxis(XChart &chart, const std::string &field, std::size_t index, const std::string &dimType, const std::string &verticalField);
    void InitAxisConfig(XChart &chart);

    void DrawAxis(XChart &chart, std::unique_ptr<Axis> &axis, canvas::CanvasContext &context);

    void DrawLabel(XChart &chart, std::unique_ptr<Axis> &axis, canvas::CanvasContext &context);

    void DrawLine(XChart &chart, std::array<util::Point, 2> &&line, const nlohmann::json &lineCfg);

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
    nlohmann::json axisSelection_;
    shape::Group *container_ = nullptr;
    nlohmann::json fieldSelectedArray_;
    bool isRepainting = false;
};
} // namespace axis
} // namespace xg
#endif // XG_GRAPHICS_AXIS_CONTROLLER_H
