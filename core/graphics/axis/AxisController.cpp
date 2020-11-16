#include "graphics/axis/AxisController.h"
#include "graphics/XChart.h"
#include "graphics/shape/Circle.h"
#include "graphics/shape/Line.h"
#include "graphics/shape/Polyline.h"
#include "graphics/shape/Text.h"
#include "utils/common.h"
#include "utils/xtime.h"

// 网格线绘制始终是从 0 - 1， 数据不足时主动补齐
static std::vector<xg::scale::Tick> FormatTicks(std::vector<xg::scale::Tick> ticks) {
    std::vector<xg::scale::Tick> rst(ticks);
    if(rst.size() > 0) {
        const xg::scale::Tick &first = rst[0];
        if(!xg::IsZero(first.value)) {
            xg::scale::Tick t;
            t.text = "";
            t.tickValue = "";
            t.value = 0.0;
            rst.insert(rst.begin(), std::move(t));
        }

        const xg::scale::Tick &last = rst[rst.size() - 1UL];
        if(!xg::IsEqual(last.value, 1.0)) {
            xg::scale::Tick t;
            t.text = "";
            t.tickValue = "";
            t.value = 1.0;
            rst.push_back(std::move(t));
        }
    }
    return rst;
}

void xg::axis::AxisController::DrawAxes(xg::XChart *chart, shape::Group *container, canvas::CanvasContext &context) {
    utils::Tracer *tracer = chart->GetLogTracer();

    this->axes.clear();
    const std::string &xField = chart->GetXScaleField();
    std::vector<std::string> yFields = chart->getYScaleFields();
    tracer->trace("InitAxis xField: %s, yField: %s", xField.c_str(), yFields[0].c_str());

    // 补齐轴的配置, TODO: 后面要优化成循环
    if(!axisConfig_.contains(xField)) {
        axisConfig_[xField] = AxisController::MergeDefaultConfig({});
    }

    const std::string &yField = yFields[0];
    if(!axisConfig_.contains(yField)) {
        axisConfig_[yField] = AxisController::MergeDefaultConfig({});
        axisConfig_[yField]["grid"] = false; // y 轴默认不显示网格
    }

    if(!axisConfig_[xField]["hidden"])
        InitAxis(*chart, xField, "x", yField);
    if(!axisConfig_[yField]["hidden"])
        InitAxis(*chart, yField, "y", xField);

    // 轴创建完成之后，重新计算 padding 值，更新可绘制区域
    UpdateLayout(*chart);

    tracer->trace("InitAxis xField: %s, yField: %s", yField.c_str(), xField.c_str());
    InitAxisConfig(*chart);

    std::for_each(axes.begin(), axes.end(),
                  [&](std::unique_ptr<xg::axis::Axis> &axis) { DrawAxis(*chart, axis, *container, context); });
}

void xg::axis::AxisController::InitAxis(XChart &chart, const std::string &field, const std::string &dimType, const std::string &verticalField) {
    //    xg::axis::Axis axis;
    auto axis = xg::make_unique<xg::axis::Axis>();
    if(chart.GetCoord().GetType() == xg::canvas::coord::CoordType::Polar) {
        if((dimType == "x" && !chart.GetCoord().IsTransposed()) || (dimType == "y" && chart.GetCoord().IsTransposed())) {
            axis->key = "circle";
            axis->type = "circle";
            axis->position = "circle";
        } else {
            axis->key = "radius";
            axis->type = "line";
            axis->position = "radius";
        }
    } else {
        const std::string &position = GetLinePosition(dimType, 0, chart.GetCoord().IsTransposed());
        axis->key = position;
        axis->type = "line";
        axis->position = position;
    }
    axis->dimType = dimType;
    axis->field = field;
    axis->verticalField = verticalField;

    nlohmann::json &fieldCfg = axisConfig_[axis->field];

    bool showGrid = axis->position == "bottom"; // bottom 默认显示轴
    if(fieldCfg["grid"].is_object()) {
        showGrid = true;
    }

    if(showGrid) {
        // 默认值
        axis->gridCfg = fieldCfg["grid"];
    }

    if(fieldCfg["label"].is_object()) {
        axis->labelCfg = fieldCfg["label"];

        if(axis->key == "left") {
            axis->labelCfg["textAlign"] = "end";
        } else if(axis->key == "right") {
            axis->labelCfg["textAlign"] = "start";
        }
    }

    if(fieldCfg["line"].is_object()) {
        axis->lineCfg = fieldCfg["line"];
    }

    scale::AbstractScale &scale = chart.GetScale(axis->field);
    axis->ticks = scale.GetTicks();
    int ticksTmp = 1;
    if(axis->ticks.size() > 600) {
        ticksTmp = 200;
    }

    if(axis->labelCfg.is_object()) {
        float labelMargin = axis->labelCfg["labelMargin"];
        labelMargin *= chart.GetRatio();

        float labelOffset = axis->labelCfg["labelOffset"];
        labelOffset *= chart.GetRatio();

        const std::string &labelColor = axis->labelCfg["textColor"];
        float labelFontSize = axis->labelCfg["textSize"];
        labelFontSize *= chart.GetRatio();

        const std::string &textAlign = axis->labelCfg["textAlign"];
        const std::string &textBaseline = axis->labelCfg["textBaseline"];

        for(int index = 0; index < axis->ticks.size(); ++index) {
            if(index == 0 || index == (axis->ticks.size() - 1) || index % ticksTmp == 0) {
                const xg::scale::Tick &tick = axis->ticks[index];
                std::unique_ptr<xg::shape::Text> text =
                    xg::make_unique<xg::shape::Text>(tick.text, util::Point{0, 0}, labelFontSize, "", labelColor);
                text->ext["tickValue"] = tick.value;
                text->SetTextAlign(textAlign);
                text->SetTextBaseline(textBaseline);
                xg::util::BBox bbox = text->GetBBox(chart.GetCanvasContext());

                axis->maxWidth = fmax(axis->maxWidth, static_cast<double>(bbox.width));
                axis->maxHeight = fmax(axis->maxHeight, static_cast<double>(bbox.height));

                axis->labels.push_back(std::move(text));
            }
        }
    }

    axes.push_back(std::move(axis));
}

void xg::axis::AxisController::InitAxisConfig(xg::XChart &chart) {
    std::for_each(axes.begin(), axes.end(), [&](std::unique_ptr<xg::axis::Axis> &axis) {
        const std::vector<xg::scale::Tick> &ticks = axis->ticks;

        // grid Points
        if(axis->gridCfg.is_object() && !axis->verticalField.empty()) {
            std::vector<xg::scale::Tick> verticalTicks = FormatTicks(chart.GetScale(axis->verticalField).GetTicks());

            for(int index = 0; index < ticks.size(); ++index) {
                auto &tick = ticks[index];
                std::vector<util::Point> linePoints;

                for(int position = 0; position < verticalTicks.size(); ++position) {
                    auto &verticalTick = verticalTicks[position];
                    double x = axis->dimType == "x" ? tick.value : verticalTick.value;
                    double y = axis->dimType == "x" ? verticalTick.value : tick.value;
                    if(x >= 0 && x <= 1 && y >= 0 && y <= 1) {
                        util::Point rt = chart.GetCoord().ConvertPoint(util::Point(x, y));
                        linePoints.push_back(std::move(rt));
                    }
                }
                axis->gridPoints.push_back(std::move(linePoints));
            }
        }
    });
}

void xg::axis::AxisController::UpdateLayout(xg::XChart &chart) {
    std::array<double, 4> &userPadding = chart.padding_;
    // std::array<double, 4>& legendPadding = chart.lengendPadding_;

    // 由于暂时不设计 appendPading 和 auto 属性，下面这块逻辑备注下。
    //    const padding = [
    //     userPadding[0] === 'auto' ? legendRange.top + appendPadding[0] * 2 : userPadding[0],
    //     userPadding[1] === 'auto' ? legendRange.right + appendPadding[1] : userPadding[1],
    //     userPadding[2] === 'auto' ? legendRange.bottom + appendPadding[2] : userPadding[2],
    //     userPadding[3] === 'auto' ? legendRange.left + appendPadding[3] : userPadding[3]
    //   ];
    std::array<double, 4> newPadding = userPadding;

    if(chart.GetCoord().GetType() == xg::canvas::coord::CoordType::Polar) {

    } else {
        std::for_each(axes.begin(), axes.end(), [&](std::unique_ptr<xg::axis::Axis> &axis) {
            float labelOffset = 0.f;
            if(axis->labelCfg.is_object()) {
                labelOffset = axis->labelCfg["labelOffset"];
            }
            labelOffset *= chart.GetRatio();

            if(axis->key == "right") {
                newPadding[2] += axis->maxWidth + static_cast<double>(labelOffset);
            } else if(axis->key == "left") {
                newPadding[0] += axis->maxWidth + static_cast<double>(labelOffset);
            } else if(axis->key == "bottom") {
                newPadding[3] += axis->maxHeight + static_cast<double>(labelOffset);
            }
        });
        chart.UpdateLayout(newPadding);
    }
}

void xg::axis::AxisController::DrawAxis(xg::XChart &chart,
                                        std::unique_ptr<xg::axis::Axis> &axis,
                                        xg::shape::Group &container,
                                        canvas::CanvasContext &context) {
    utils::Tracer *tracer = chart.GetLogTracer();
    tracer->trace("start draw axis: %s dimType: %s", axis->field.c_str(), axis->dimType.c_str());

    // draw grid  Polar except
    if(chart.GetCoord().GetType() != xg::canvas::coord::CoordType::Polar && axis->gridCfg.is_object() &&
       !axis->verticalField.empty() && !axis->gridPoints.empty()) {
        tracer->trace("%s", "draw grid line");
        // 有 grid 配置，绘制 grid
        std::for_each(axis->gridPoints.begin(), axis->gridPoints.end(), [&](const std::vector<util::Point> &points) -> void {
            if(axis->gridCfg["type"] == "line") {
                auto l = xg::make_unique<xg::shape::Polyline>(axis->gridCfg["lineWidth"], points, axis->gridCfg["stroke"], "", false);
                l->SetZIndex(-1);
                container.AddElement(std::move(l));
            } else if(axis->gridCfg["type"] == "dash") {
                auto l = xg::make_unique<xg::shape::Polyline>(axis->gridCfg["lineWidth"], points, axis->gridCfg["stroke"], "", false);
                l->SetZIndex(-1);
                // TODO dash 参数可自定义传入
                l->SetDashLine(std::vector<float>{2, 2});
                container.AddElement(std::move(l));
            }
        });
    }
    if(axis->type == "line") {
        std::array<util::Point, 2> line = GetLineCfg(chart.GetCoord(), axis->dimType, axis->position);
        if(chart.GetCoord().GetType() == xg::canvas::coord::CoordType::Polar) {
            line = GetRadiusCfg(chart.GetCoord());
        }
        axis->start.x = line[0].x;
        axis->start.y = line[0].y;
        axis->end.x = line[1].x;
        axis->end.y = line[1].y;

        bool drawLine = axis->lineCfg.is_object();
        if(drawLine) {
            tracer->trace("%s", "draw axis Line");
            DrawLine(std::move(line), axis->lineCfg, container);
        }
        // TODO draw tick line
    }

    tracer->trace("%s %lu", "draw txt labels: ", axis->labels.size());
    // std::for_each(axis.txtLabels.begin(), axis.txtLabels.end(),
    //               [&](xg::axis::LabelTXT &label) { DrawLabel(chart, axis, label, container, context); });

    DrawLabel(chart, axis, container, context);

    tracer->trace("%s\n", "end draw axis;");
}

void xg::axis::AxisController::DrawLine(std::array<util::Point, 2> &&line, const nlohmann::json &lineCfg, shape::Group &container) {
    if(lineCfg["type"] == "line") {
        std::unique_ptr<xg::shape::Element> _line =
            xg::make_unique<xg::shape::Line>(line[0], line[1], lineCfg["lineWidth"], lineCfg["color"]);
        container.AddElement(std::move(_line));
    }
}

void xg::axis::AxisController::DrawLabel(XChart &chart, std::unique_ptr<xg::axis::Axis> &axis, xg::shape::Group &container, canvas::CanvasContext &context) {
    // xg::util::Point start = GetOffsetPoint(axis, label.value);
    // GetSidePoint(start, 5 * chart.GetRatio(), axis.dimType);
    // 修改x,y 这里计算的真实值
    // std::unique_ptr<xg::shape::Text> text = xg::make_unique<xg::shape::Text>(label.text.c_str(), xg::util::Point(start.x,
    // start.y), label.fontSize, "", label.fill);

    float labelMargin = 0.f;
    if(axis->labelCfg.is_object()) {
        labelMargin = axis->labelCfg["labelMargin"];
    }
    labelMargin *= context.GetDevicePixelRatio();

    float labelOffset = 0.f;
    if(axis->labelCfg.is_object()) {
        labelOffset = axis->labelCfg["labelOffset"];
    }
    labelOffset *= context.GetDevicePixelRatio();

    size_t size = axis->labels.size();
    while(axis->labels.size() > 0) {
        auto text = std::move(axis->labels[0]);
        axis->labels.erase(axis->labels.begin());

        bool isFirst = axis->labels.size() == (size - 1);
        bool isLast = axis->labels.size() == 0;

        double tickValue = text->ext["tickValue"];
        xg::util::Point pt = GetOffsetPoint(axis, tickValue);
        string dimType = axis->dimType;
        if(chart.GetCoord().IsTransposed()) {
            dimType = axis->dimType == "x" ? "y" : "x";
        }
        GetSidePoint(pt, 5 * chart.GetRatio(), dimType);

        xg::util::BBox bbox = text->GetBBox(context);

        if(axis->position == "bottom") {
            if(isFirst) {
                pt.x += labelMargin;
            } else if(isLast) {
                pt.x -= labelMargin;
            }
            pt.y += labelOffset;
            pt.y += bbox.height / 2;
            pt.y += (text->GetLineCount() - 1) * text->GetSpacingY() * context.GetDevicePixelRatio();
        } else if(axis->position == "left") {
            if(isFirst) {
                pt.y -= labelMargin;
            } else if(isLast) {
                pt.y += labelMargin;
            }
            pt.y += bbox.height / 2;
        } else if(axis->position == "right") {
            if(isFirst) {
                pt.y -= labelMargin;
            } else if(isLast) {
                pt.y += labelMargin;
            }
        }
        text->SetPoint(pt);
        container.AddElement(std::move(text));
    }
}
