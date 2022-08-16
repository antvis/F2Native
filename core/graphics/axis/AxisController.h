#ifndef XG_GRAPHICS_AXIS_CONTROLLER_H
#define XG_GRAPHICS_AXIS_CONTROLLER_H

#include <array>
#include <string>
#include <vector>
#include <array>
#include "../canvas/CanvasContext.h"
#include "../canvas/Coord.h"
#include "../scale/Scale.h"
#include "../shape/Group.h"
#include "../shape/Text.h"
#include "../util/Point.h"
#include "../../utils/common.h"
#include "../../reflection/reflection.h"

using namespace xg::util;

namespace xg {

class XChart;
namespace axis {

struct AxisGridCfg {
    string type = "line";
    float lineWidth = .6f;
    string stroke = "#E8E8E8";
    std::vector<float> dash = {10, 10};
    //雷达图轴上的背景色
    std::vector<string> fill;
    std::vector<float> fillOpacity;
    bool hidden = false;
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(AxisGridCfg)
        FIELDS(FIELD(&AxisGridCfg::type),
               FIELD(&AxisGridCfg::lineWidth),
               FIELD(&AxisGridCfg::stroke),
               FIELD(&AxisGridCfg::dash),
               FIELD(&AxisGridCfg::fill),
               FIELD(&AxisGridCfg::fillOpacity),
               FIELD(&AxisGridCfg::hidden))
        CTORS(DEFAULT_CTOR(AxisGridCfg))
    END_TYPE
#endif
};

struct AxisLabelCfg {
    string textColor = "#808080"; // 标签文字颜色
    float textSize =  DEFAULT_FONTSIZE;       // 标签文字字号
    float labelOffset = 5.f;     // 轴上标签的垂直方向偏移量。x 轴为上下的整体偏移量， y 轴为左右的整体偏移量
    string textAlign = "center";  //文字水平对齐方式 start center end
    string textBaseline = "bottom";//文字垂直对齐方式 top middle bottom
    
    vector<string> textAligns;  //文字水平对齐方式 start center end
    vector<string> textBaselines;//文字垂直对齐方式 top middle bottom
    bool inner = false;            //轴上的文字是否在图表内部
    string item = "";//回调方法的id
    float xOffset = 0;
    float yOffset = 0;
    string content = "";
    bool hidden = false;

    //这里没有__EMSCRIPTEN__宏，是因为AxisController需要反射AxisLabelCfg
    BEGIN_TYPE(AxisLabelCfg)
        FIELDS(FIELD(&AxisLabelCfg::textColor),
               FIELD(&AxisLabelCfg::textSize),
               FIELD(&AxisLabelCfg::labelOffset),
               FIELD(&AxisLabelCfg::textAlign),
               FIELD(&AxisLabelCfg::textBaseline),
               FIELD(&AxisLabelCfg::textAligns),
               FIELD(&AxisLabelCfg::textBaselines),
               FIELD(&AxisLabelCfg::inner),
               FIELD(&AxisLabelCfg::item),
               FIELD(&AxisLabelCfg::xOffset),
               FIELD(&AxisLabelCfg::yOffset),
               FIELD(&AxisLabelCfg::content),
               FIELD(&AxisLabelCfg::hidden))
        CTORS(DEFAULT_CTOR(AxisLabelCfg))
    END_TYPE
};

struct AxisLineCfg {
    string type = "line";      // 网格线类型
    float lineWidth = .6f;    // 网格线线宽
    string stroke = "#E8E8E8"; // 网格线颜色
    std::vector<float> dash =  {10, 10};
    bool hidden = false;
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(AxisLineCfg)
        FIELDS(FIELD(&AxisLineCfg::type),
               FIELD(&AxisLineCfg::lineWidth),
               FIELD(&AxisLineCfg::stroke),
               FIELD(&AxisLineCfg::dash),
               FIELD(&AxisLineCfg::hidden))
        CTORS(DEFAULT_CTOR(AxisLineCfg))
    END_TYPE
#endif
};

struct AxisCfg {
    bool hidden = false;
    AxisGridCfg grid;
    AxisLabelCfg label;
    AxisLineCfg line;
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(AxisCfg)
        FIELDS(FIELD(&AxisCfg::grid),
               FIELD(&AxisCfg::label),
               FIELD(&AxisCfg::line),
               FIELD(&AxisCfg::hidden))
        CTORS(DEFAULT_CTOR(AxisCfg))
    END_TYPE
#endif
};

class Axis {
  public:
    Axis() {}
    Axis(const Axis &) = delete;
    virtual ~Axis() {}
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
    AxisGridCfg gridCfg;
    AxisLabelCfg labelCfg;
    AxisLineCfg lineCfg;
    std::string field;
    std::string verticalField; // 相对垂直边 field
    std::vector<scale::Tick> ticks;
    
    virtual util::Point GetOffsetPoint(double value, double offset) const {
        util::Point point;
        point.x = start.x + (end.x - start.x) * value;
        point.y = start.y + (end.y - start.y) * value;
        return point;
    }
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

    void SetFieldConfig(const std::string &field, const AxisCfg &config);

    void Clear();

  private:
    void InitAxis(XChart &chart, const std::string &field, std::size_t index, const std::string &dimType, const std::string &verticalField);
    void InitAxisConfig(XChart &chart);

    void DrawAxis(XChart &chart, std::unique_ptr<Axis> &axis, canvas::CanvasContext &context);

    void DrawLabel(XChart &chart, std::unique_ptr<Axis> &axis, canvas::CanvasContext &context);

    void DrawLine(XChart &chart, std::array<util::Point, 2> &&line, const AxisLineCfg &lineCfg);

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
    std::unordered_map<string, AxisCfg> axisConfig_;
    shape::Group *container_ = nullptr;
};
} // namespace axis
} // namespace xg
#endif // XG_GRAPHICS_AXIS_CONTROLLER_H
