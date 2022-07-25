#include "AxisController.h"
#include "../XChart.h"
#include "../func/Func.h"
#include "../shape/Circle.h"
#include "../shape/Line.h"
#include "../shape/Polyline.h"
#include "../shape/Text.h"
#include "../shape/Rect.h"
#include "../shape/Polygon.h"
#include "../../utils/common.h"
#include "../../utils/xtime.h"

void xg::axis::from_json(const nlohmann::json& j, AxisGridCfg& a) {
    AxisGridCfg d;
    if(j.is_boolean()) {
        a.hidden = true;
    }else if(j.is_object()) {
        a.type = j.value("type", d.type);
        a.lineWidth = j.value("lineWidth", d.lineWidth);
        a.stroke = j.value("stroke", d.stroke);
        a.dash = j.value("dash", d.dash);
        a.fill = j.value("fill", d.fill);
        a.hidden = j.value("hidden", d.hidden);
    } else {
        F2ASSERT(false, "AxisGridCfg from_json error json type");
    }
}

void xg::axis::from_json(const nlohmann::json& j, AxisLabelCfg& a) {
    AxisLabelCfg d;
    if(j.is_boolean()) {
        a.hidden = true;
    }else if(j.is_object()) {
        a.textColor = j.value("textColor", d.textColor);
        a.textSize = j.value("textSize", d.textSize);
        a.labelOffset = j.value("labelOffset", d.labelOffset);
        if(!json::GetArray(j, "textAlign").empty()) {
            a.textAligns = j.value("textAlign", d.textAligns);
        }else {
            a.textAlign = j.value("textAlign", d.textAlign);
        }
        
        if(!json::GetArray(j,"textBaseline").empty()) {
            a.textBaselines = j.value("textBaselines", d.textBaselines);
        }else {
            a.textBaseline = j.value("textBaseline", d.textBaseline);
        }
        
        a.inner = j.value("inner", d.inner);
        //functionId 只要解析一次就可以了
        string tempItem = j.value("item", d.item);
        a.item = tempItem.empty() ? a.item : tempItem;
        a.xOffset = j.value("xOffset", d.xOffset);
        a.yOffset = j.value("yOffset", d.yOffset);
        a.hidden = j.value("hidden", d.hidden);
    } else {
        F2ASSERT(false, "AxisLabelCfg from_json error json type");
    }
}

void xg::axis::from_json(const nlohmann::json& j, AxisLineCfg& a) {
    AxisLineCfg d;
    if(j.is_boolean()) {
        a.hidden = true;
    }else if(j.is_object()) {
        a.type = j.value("type", d.type);
        a.lineWidth = j.value("lineWidth", d.lineWidth);
        a.stroke = j.value("stroke", d.stroke);
        a.dash = j.value("dash", d.dash);
        a.hidden = j.value("hidden", d.hidden);
    } else {
        F2ASSERT(false, "AxisLineCfg from_json error json type");
    }
}

void xg::axis::from_json(const nlohmann::json& j, AxisCfg& a) {
    AxisCfg d;
    if(j.is_boolean()) {
        a.hidden = true;
    }else if(j.is_object()) {
        a.label = j.value("label", d.label);
        a.line = j.value("line", d.line);
        a.grid = j.value("grid", d.grid);
        a.hidden = j.value("hidden", d.hidden);
    } else {
        F2ASSERT(false, "AxisLineCfg from_json error json type");
    }
}

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

void xg::axis::AxisController::SetFieldConfig(const std::string &field, const AxisCfg &config) {
    axisConfig_[field] = config;
}

void xg::axis::AxisController::DrawAxes(xg::XChart *chart, canvas::CanvasContext &context) {
    utils::Tracer *tracer = chart->GetLogTracer();

    this->axes.clear();
    const std::string &xField = chart->GetXScaleField();
    std::vector<std::string> yFields = chart->getYScaleFields();
    tracer->trace("InitAxis xField: %s, yField: %s", xField.c_str(), yFields[0].c_str());

    if(!axisConfig_[xField].hidden) {
        InitAxis(*chart, xField, 0, "x", yFields[0]);
    }

    for(std::size_t index = 0; index < yFields.size(); ++index) {
        const std::string &yField = yFields[index];
        if(!axisConfig_[yField].hidden) {
            InitAxis(*chart, yField, index, "y", xField);
        }
    }

    // 轴创建完成之后，重新计算 padding 值，更新可绘制区域
    UpdateLayout(*chart);

    InitAxisConfig(*chart);

    std::for_each(axes.begin(), axes.end(), [&](std::unique_ptr<xg::axis::Axis> &axis) { DrawAxis(*chart, axis, context); });
}

void xg::axis::AxisController::InitAxis(XChart &chart, const std::string &field, std::size_t index, const std::string &dimType, const std::string &verticalField) {
    //    xg::axis::Axis axis;
    std::unique_ptr<Axis> axis;
    if(chart.GetCoord().GetType() == xg::canvas::coord::CoordType::Polar) {
        //环形饼图 || 雷达图的x轴
        if((dimType == "x" && !chart.GetCoord().IsTransposed()) || (dimType == "y" && chart.GetCoord().IsTransposed())) {
            axis = xg::make_unique<xg::axis::Circle>();
            axis->key = "circle";
            axis->type = "circle";
            axis->position = "circle";
        } else {
            //饼图的y轴
            axis = xg::make_unique<xg::axis::Axis>();
            axis->key = "radius";
            axis->type = "line";
            axis->position = "radius";
        }
    } else {
        axis = xg::make_unique<xg::axis::Axis>();
        const std::string &position = GetLinePosition(dimType, index, chart.GetCoord().IsTransposed());
        axis->key = position;
        axis->type = "line";
        axis->position = position;
    }
    axis->dimType = dimType;
    axis->field = field;
    axis->verticalField = verticalField;

    auto &fieldCfg = axisConfig_[axis->field];
    axis->gridCfg = fieldCfg.grid;

    axis->labelCfg = fieldCfg.label;
    axis->lineCfg = fieldCfg.line;


    scale::AbstractScale &scale = chart.GetScale(axis->field);
    axis->ticks = scale.GetTicks(&chart);

    int ticksTmp = 1;
    if(axis->ticks.size() > 600) {
        ticksTmp = 200;
    }

    if(!axis->labelCfg.hidden) {
        float labelOffset = axis->labelCfg.labelOffset;
        labelOffset *= chart.GetCanvasContext().GetDevicePixelRatio();

        const std::string &labelColor = axis->labelCfg.textColor;
        float labelFontSize = axis->labelCfg.textSize * chart.GetCanvasContext().GetDevicePixelRatio();
        
        bool inner = axis->labelCfg.inner;

        float ratio = chart.GetCanvasContext().GetDevicePixelRatio();
        for(std::size_t index = 0; index < axis->ticks.size(); ++index) {
            if(index == 0 || index == (axis->ticks.size() - 1) || index % ticksTmp == 0) {
                const xg::scale::Tick &tick = axis->ticks[index];
                if(!axis->labelCfg.item.empty()) {
                    nlohmann::json param = {{"text", tick.text}, {"index", index}, {"count", axis->ticks.size()}, {"content", tick.text}, {"xOffset", 0}, {"yOffset", 0}};

                    nlohmann::json itemStyleRst(xg::json::ParseString(chart.InvokeFunction(axis->labelCfg.item, param.dump())));
                    
                    from_json(itemStyleRst, axis->labelCfg);

                    float textSize = axis->labelCfg.textSize * ratio;

                    float xOffset = axis->labelCfg.xOffset *ratio;
                    float yOffset = axis->labelCfg.yOffset *ratio;

                    std::string textContent = axis->labelCfg.content.empty() ? tick.text : axis->labelCfg.content;

                    std::unique_ptr<xg::shape::Text> text =
                        xg::make_unique<xg::shape::Text>(textContent, util::Point{xOffset, yOffset}, textSize, "", axis->labelCfg.textColor);

                    text->ext["tickValue"] = tick.value;
                    text->SetTextAlign(axis->labelCfg.textAlign);
                    
                    text->SetTextBaseline(axis->labelCfg.textBaseline);
                    xg::util::BBox bbox = text->GetBBox(chart.GetCanvasContext());

                    axis->maxWidth = inner ? 0 : fmax(axis->maxWidth, static_cast<double>(bbox.width));
                    axis->maxHeight = inner ? 0 : fmax(axis->maxHeight, static_cast<double>(bbox.height));

                    axis->labels.push_back(std::move(text));
                } else {
                    std::unique_ptr<xg::shape::Text> text =
                        xg::make_unique<xg::shape::Text>(tick.text, util::Point{0, 0}, labelFontSize, "", labelColor);
                    text->ext["tickValue"] = tick.value;
                    if (!axis->labelCfg.textAligns.empty()) {
                        text->SetTextAlign(axis->labelCfg.textAligns[index]);
                    }else {
                        text->SetTextAlign(axis->labelCfg.textAlign);
                    }
                    
                    if (!axis->labelCfg.textBaselines.empty()) {
                        text->SetTextBaseline(axis->labelCfg.textBaselines[index]);
                    }else {
                        text->SetTextBaseline(axis->labelCfg.textBaseline);
                    }
                        
                    xg::util::BBox bbox = text->GetBBox(chart.GetCanvasContext());

                    axis->maxWidth = inner ? 0 : fmax(axis->maxWidth, static_cast<double>(bbox.width));
                    axis->maxHeight = inner ? 0 : fmax(axis->maxHeight, static_cast<double>(bbox.height));

                    axis->labels.push_back(std::move(text));
                }
            }
        }
    }

    axes.push_back(std::move(axis));
}

void xg::axis::AxisController::InitAxisConfig(xg::XChart &chart) {
    auto ts = xg::CurrentTimestampAtMM();
    std::for_each(axes.begin(), axes.end(), [&](std::unique_ptr<xg::axis::Axis> &axis) {
        const std::vector<xg::scale::Tick> &ticks = axis->ticks;

        // grid Points
        if(!axis->verticalField.empty()) {
            std::vector<xg::scale::Tick> verticalTicks = FormatTicks(chart.GetScale(axis->verticalField).GetTicks(&chart));

            for(std::size_t index = 0; index < ticks.size(); ++index) {
                auto &tick = ticks[index];
                std::vector<util::Point> linePoints;

                for(std::size_t position = 0; position < verticalTicks.size(); ++position) {
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

    chart.GetLogTracer()->trace("#InitAxisesConfig  %lums", (xg::CurrentTimestampAtMM() - ts));
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
        
        std::for_each(axes.begin(), axes.end(), [&](std::unique_ptr<xg::axis::Axis> &axis) {
            if (axis->type == "circle") {
                float labelOffset = axis->labelCfg.labelOffset * chart.GetCanvasContext().GetDevicePixelRatio();
                newPadding[0] += axis->maxWidth + labelOffset;
                newPadding[1] += axis->maxHeight + labelOffset;
                newPadding[2] += axis->maxWidth + labelOffset;
                newPadding[3] += axis->maxHeight + labelOffset;
            }            
        });
        chart.UpdateLayout(newPadding);
    } else {
        std::for_each(axes.begin(), axes.end(), [&](std::unique_ptr<xg::axis::Axis> &axis) {
            float labelOffset = axis->labelCfg.labelOffset * chart.GetCanvasContext().GetDevicePixelRatio();

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

void xg::axis::AxisController::DrawAxis(xg::XChart &chart, std::unique_ptr<xg::axis::Axis> &axis, canvas::CanvasContext &context) {
    utils::Tracer *tracer = chart.GetLogTracer();
    tracer->trace("start draw axis: %s dimType: %s", axis->field.c_str(), axis->dimType.c_str());
    
    //解决雷达图上线后，饼图的grid被画出来的问题，为了兼容已上线的饼图业务 需要把饼图的grid影藏
    //todo 后面业务卡片上改后 在把hideGrid去掉即可
    // field: "percent",
    //      config: {
    //      line: false,
    //      label: false,
    //  }
    //},
    auto &coord = chart.GetCoord();
    bool hideGrid = coord.GetType() == coord::CoordType::Polar && axis->labelCfg.hidden && axis->lineCfg.hidden;
    // draw grid
    if(!hideGrid &&
       !axis->verticalField.empty() &&
       !axis->gridPoints.empty()) {
        tracer->trace("%s", "draw grid line");
        float lineWidth = axis->gridCfg.lineWidth * chart.GetCanvasContext().GetDevicePixelRatio();
        auto &type = axis->gridCfg.type;
        auto position = axis->position;
        auto &stroke = axis->gridCfg.stroke;
       
        std::vector<float> dash = json::ScaleDash(axis->gridCfg.dash, context.GetDevicePixelRatio());
        
        //雷达图轴上的背景色
        std::vector<string> fill = axis->gridCfg.fill;
        auto xAxis = coord.GetXAxis();
        int index = 0;
        int radius0 = 0;
        vector<Point> points0;
        
        //雷达图
        if (axis->position == "radius") {
            // 有 grid 配置，绘制 grid
            std::for_each(axis->gridPoints.begin(), axis->gridPoints.end(), [&](const std::vector<util::Point> &points) -> void {
                //雷达图第一个数据y为null 创建不出正确的度量，没有points..
                if (points.size() == 0) {
                    return;
                }
                //第一个是圆心 不上色
                const string &fillColor = fill.size() && index > 0 ? fill[(index -1) % fill.size()] : "";
                auto point = Vector2D {points[0].x - coord.GetCenter().x, points[0].y - coord.GetCenter().y};
                double radius = Vector2DUtil::Length(point);
                if(type == "line") {
                    auto p = xg::make_unique<xg::shape::Polygon>(lineWidth, points, points0, stroke, fillColor, false);
                    p->SetZIndex(-2);
                    this->container_->AddElement(std::move(p));
                }else if(type == "dash") {
                    auto p = xg::make_unique<xg::shape::Polygon>(lineWidth, points, points0, stroke, fillColor, false);
                    p->SetDashLine(dash);
                    p->SetZIndex(-2);
                    this->container_->AddElement(std::move(p));
                }else if(type == "arc") {
                    auto arc = xg::make_unique<xg::shape::Rect>(coord.GetCenter(), radius, radius0, xAxis.x, xAxis.y,  fillColor, lineWidth, stroke);
                    arc->SetZIndex(-2);
                    this->container_->AddElement(std::move(arc));
                }else if(type == "arcdash") {
                    auto arc = xg::make_unique<xg::shape::Rect>(coord.GetCenter(), radius, radius0, xAxis.x, xAxis.y,  fillColor, lineWidth, stroke);
                    arc->SetZIndex(-2);
                    arc->SetDashLine(dash);
                    this->container_->AddElement(std::move(arc));
                }
                ++index;
                radius0 = radius;
                points0 = points;
            });
        }else {
            std::for_each(axis->gridPoints.begin(), axis->gridPoints.end(), [&](const std::vector<util::Point> &points) -> void {
                if(type == "line") {
                    auto l = xg::make_unique<xg::shape::Polyline>(lineWidth, points, stroke, "", false);
                    l->SetZIndex(-1);
                    this->container_->AddElement(std::move(l));
                } else if(type == "dash") {
                    auto l = xg::make_unique<xg::shape::Polyline>(lineWidth, points,stroke, "", false);
                    l->SetZIndex(-1);
                    l->SetDashLine(dash);
                    this->container_->AddElement(std::move(l));
                }
            });
        }
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

        if(!axis->lineCfg.hidden) {
            tracer->trace("%s", "draw axis Line");
            DrawLine(chart, std::move(line), axis->lineCfg);
        }
        // TODO draw tick line
    }else if(axis->type == "circle") {
        if(chart.GetCoord().GetType() == xg::canvas::coord::CoordType::Polar) {
            axis::Circle *circle = static_cast<axis::Circle *>(axis.get());
            auto &coord = chart.GetCoord();
            circle->center = coord.GetCenter();
            circle->radius = coord.GetRadius();
            circle->startAngle = coord.GetXAxis().x;
            circle->endAngle = coord.GetXAxis().y;
        }
    }

    tracer->trace("%s %lu", "draw txt labels: ", axis->labels.size());
    // std::for_each(axis.txtLabels.begin(), axis.txtLabels.end(),
    //               [&](xg::axis::LabelTXT &label) { DrawLabel(chart, axis, label, container, context); });

    DrawLabel(chart, axis, context);

    tracer->trace("%s\n", "end draw axis;");
}

void xg::axis::AxisController::DrawLine(XChart &chart, std::array<util::Point, 2> &&line, const AxisLineCfg &lineCfg) {
    float lineWidth = lineCfg.lineWidth * chart.GetCanvasContext().GetDevicePixelRatio();
    if(lineCfg.type == "line") {
        std::unique_ptr<xg::shape::Element> _line = xg::make_unique<xg::shape::Line>(line[0], line[1], lineWidth, lineCfg.stroke);
        this->container_->AddElement(std::move(_line));
    } else if(lineCfg.type == "dash") {
        std::vector<util::Point> _points = {line[0], line[1]};
        auto l = xg::make_unique<xg::shape::Polyline>(lineWidth, _points, false);
        l->SetStorkColor(lineCfg.stroke);
        l->SetZIndex(-1);
        std::vector<float> dash = xg::json::ParseDashArray(lineCfg.dash, chart.GetCanvasContext().GetDevicePixelRatio());
        l->SetDashLine(dash);
        this->container_->AddElement(std::move(l));
    }
}

void xg::axis::AxisController::DrawLabel(XChart &chart, std::unique_ptr<xg::axis::Axis> &axis, canvas::CanvasContext &context) {
    // xg::util::Point start = GetOffsetPoint(axis, label.value);
    // GetSidePoint(start, 5 * chart.GetRatio(), axis.dimType);
    // 修改x,y 这里计算的真实值
    // std::unique_ptr<xg::shape::Text> text = xg::make_unique<xg::shape::Text>(label.text.c_str(), xg::util::Point(start.x,
    // start.y), label.fontSize, "", label.fill);

    float labelOffset = axis->labelCfg.labelOffset * context.GetDevicePixelRatio();

    auto &coord = chart.GetCoord();
    
    //textAlign 默认center textBaseline 默认bottom
    //pt默认对应的是字体中下方的锚点
    while(axis->labels.size() > 0) {
        auto text = std::move(axis->labels[0]);
        axis->labels.erase(axis->labels.begin());

        double tickValue = text->ext["tickValue"];
        xg::util::Point pt = axis->GetOffsetPoint(tickValue, labelOffset);
        string dimType = axis->dimType;
        if(chart.GetCoord().IsTransposed()) {
            dimType = axis->dimType == "x" ? "y" : "x";
        }
        xg::util::BBox bbox = text->GetBBox(context);
        
        //等于把pt.y轴下移了5个dp，textbaseline变成了middle
        //这时候textAlign是center textBaseline是middle，等于pt对应了字体中心的锚点
//        GetSidePoint(pt, inner ? 0 : bbox.height / 2, dimType);
        auto txtPoint = text->GetPoint();
        pt.x += txtPoint.x;
        pt.y += txtPoint.y;

        if(axis->position == "bottom") {
            pt.y += labelOffset;
            pt.y += bbox.height;
            pt.y += (text->GetLineCount() - 1) * text->GetSpacingY() * context.GetDevicePixelRatio();
        } else if(axis->position == "left") {
            pt.y += bbox.height / 2;
            pt.x -= labelOffset;

            if(text->GetTextAlign() == "center") {
                pt.x -= axis->maxWidth / 2;
            } else if(text->GetTextAlign() == "left" || text->GetTextAlign() == "start") {
                pt.x -= axis->maxWidth;
            }
        } else if(axis->position == "right") {
            pt.y += bbox.height / 2;
            pt.x += labelOffset; // + bbox.width / 2;

            if(text->GetTextAlign() == "center") {
                pt.x += axis->maxWidth / 2;
            } else if(text->GetTextAlign() == "right" || text->GetTextAlign() == "end") {
                pt.x += axis->maxWidth;
            }
        }
        //雷达图 x轴
        else if(axis->position == "circle") {
            //pt.x pt.y 对应字体的中心点
            if (xg::IsEqual(pt.x, coord.GetCenter().x)) {
                //do nothing
            }else if (pt.x > coord.GetCenter().x) {
                //等价于textAlign = left
                pt.x += bbox.width / 2;
            }else if(pt.x < coord.GetCenter().x) {
                //等价于textAlign = right
                pt.x -= bbox.width / 2;
            }
            
            //字体的y坐标和圆心相等
            if (xg::IsEqual(pt.y, coord.GetCenter().y)) {
                //do nothing
            }
            //字体的y坐标在圆心的下方
            else if (pt.y > coord.GetCenter().y) {
                //正下方的点
                if (xg::IsEqual(pt.x, coord.GetCenter().x) ) {
                    //等价于textbaseline = top
                    pt.y  += bbox.height / 2;
                }
            }
            //字体的y坐标在圆心的上方
            else if(pt.y < coord.GetCenter().y) {
                //正上方的点
                if (xg::IsEqual(pt.x, coord.GetCenter().x) ) {
                    //等价于textbaseline = bottom
                    pt.y -= bbox.height / 2;
                }
            }
        }
        //雷达图y轴
        else if(axis->position == "radius") {
            pt.x -= labelOffset;
        }
        text->SetPoint(pt);
        this->container_->AddElement(std::move(text));
    }
}

void xg::axis::AxisController::Clear() {
    this->axes.clear();
    this->container_->Clear();
}
