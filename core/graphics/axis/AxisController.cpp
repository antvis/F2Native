#include "graphics/axis/AxisController.h"
#include "graphics/XChart.h"
#include "graphics/func/Func.h"
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

void xg::axis::AxisController::DrawAxes(xg::XChart *chart, canvas::CanvasContext &context) {
    utils::Tracer *tracer = chart->GetLogTracer();

    this->axes.clear();
    const std::string &xField = chart->GetXScaleField();
    std::vector<std::string> yFields = chart->getYScaleFields();
    tracer->trace("InitAxis xField: %s, yField: %s", xField.c_str(), yFields[0].c_str());

    if(!axisConfig_.contains(xField)) {
        axisConfig_[xField] = AxisController::MergeDefaultConfig({});
    }

    if(!axisConfig_[xField]["hidden"]) {
        InitAxis(*chart, xField, 0, "x", yFields[0]);
    }

    for(std::size_t index = 0; index < yFields.size(); ++index) {
        const std::string &yField = yFields[index];
        if(!axisConfig_.contains(yField)) {
            axisConfig_[yField] = AxisController::MergeDefaultConfig({});
            axisConfig_[yField]["grid"] = false; // y 轴默认不显示网格
        }

        if(!axisConfig_[yField]["hidden"]) {
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
        const std::string &position = GetLinePosition(dimType, index, chart.GetCoord().IsTransposed());
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
        //            if(axis->key == "left") {
        //                axis->labelCfg["textAlign"] = "end";
        //            } else if(axis->key == "right") {
        //                axis->labelCfg["textAlign"] = "start";
        //            }
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

        const auto &textAlign = axis->labelCfg["textAlign"];
        const auto &textBaseline = axis->labelCfg["textBaseline"];

        func::F2Function *labelItemFunc = nullptr;
        if(axis->labelCfg.contains("item")) {
            std::string labelItemFuncId = axis->labelCfg["item"];
            labelItemFunc = func::FunctionManager::GetInstance().Find(labelItemFuncId);
        }

        for(std::size_t index = 0; index < axis->ticks.size(); ++index) {
            if(index == 0 || index == (axis->ticks.size() - 1) || index % ticksTmp == 0) {
                const xg::scale::Tick &tick = axis->ticks[index];
                if(labelItemFunc != nullptr) {
                    nlohmann::json param = {{"text", tick.text}, {"index", index}, {"count", axis->ticks.size()}};

                    nlohmann::json itemStyle = axis->labelCfg;
                    itemStyle["xOffset"] = 0;
                    itemStyle["yOffset"] = 0;

                    nlohmann::json itemStyleRst = labelItemFunc->Execute(param);
                    if(itemStyleRst.is_object()) {
                        itemStyle.merge_patch(itemStyleRst);
                    }

                    float textSize = itemStyle["textSize"];
                    textSize *= chart.GetRatio();

                    float xOffset = itemStyle["xOffset"];
                    float yOffset = itemStyle["yOffset"];
                    xOffset *= chart.GetRatio();
                    yOffset *= chart.GetRatio();

                    std::string textContent = tick.text;
                    if(itemStyle.contains("content") && itemStyle["content"].is_string()) {
                        textContent = itemStyle["content"];
                    }

                    std::unique_ptr<xg::shape::Text> text =
                        xg::make_unique<xg::shape::Text>(textContent, util::Point{xOffset, yOffset}, textSize, "", itemStyle["textColor"]);

                    text->ext["tickValue"] = tick.value;
                    if(itemStyle["textAlign"].is_string()) {
                        text->SetTextAlign(itemStyle["textAlign"]);
                    }
                    
                    if (itemStyle["textBaseline"].is_string()) {
                        text->SetTextBaseline(itemStyle["textBaseline"]);
                    }                    
                    xg::util::BBox bbox = text->GetBBox(chart.GetCanvasContext());

                    axis->maxWidth = fmax(axis->maxWidth, static_cast<double>(bbox.width));
                    axis->maxHeight = fmax(axis->maxHeight, static_cast<double>(bbox.height));

                    axis->labels.push_back(std::move(text));
                } else {
                    std::unique_ptr<xg::shape::Text> text =
                        xg::make_unique<xg::shape::Text>(tick.text, util::Point{0, 0}, labelFontSize, "", labelColor);
                    text->ext["tickValue"] = tick.value;
                    if (textAlign.is_string()) {
                        text->SetTextAlign(textAlign);
                    }else if(textAlign.is_array() && textAlign.size() > index){
                        text->SetTextAlign(textAlign[index]);
                    }
                    
                    if (textBaseline.is_string()) {
                        text->SetTextBaseline(textBaseline);
                    }else if(textBaseline.is_array() && textBaseline.size() > index) {
                        text->SetTextBaseline(textBaseline[index]);
                    }
                        
                    xg::util::BBox bbox = text->GetBBox(chart.GetCanvasContext());

                    axis->maxWidth = fmax(axis->maxWidth, static_cast<double>(bbox.width));
                    axis->maxHeight = fmax(axis->maxHeight, static_cast<double>(bbox.height));

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
        if(axis->gridCfg.is_object() && !axis->verticalField.empty()) {
            std::vector<xg::scale::Tick> verticalTicks = FormatTicks(chart.GetScale(axis->verticalField).GetTicks());

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

void xg::axis::AxisController::DrawAxis(xg::XChart &chart, std::unique_ptr<xg::axis::Axis> &axis, canvas::CanvasContext &context) {
    utils::Tracer *tracer = chart.GetLogTracer();
    tracer->trace("start draw axis: %s dimType: %s", axis->field.c_str(), axis->dimType.c_str());

    // draw grid  Polar except
    if(chart.GetCoord().GetType() != xg::canvas::coord::CoordType::Polar && axis->gridCfg.is_object() &&
       !axis->verticalField.empty() && !axis->gridPoints.empty()) {
        tracer->trace("%s", "draw grid line");
        float lineWidth = axis->gridCfg["lineWidth"].get<float>() * chart.GetRatio();
        // 有 grid 配置，绘制 grid
        std::for_each(axis->gridPoints.begin(), axis->gridPoints.end(), [&](const std::vector<util::Point> &points) -> void {
            if(axis->gridCfg["type"] == "line") {
                auto l = xg::make_unique<xg::shape::Polyline>(lineWidth, points, axis->gridCfg["stroke"], "", false);
                l->SetZIndex(-1);
                this->container_->AddElement(std::move(l));
            } else if(axis->gridCfg["type"] == "dash") {
                auto l = xg::make_unique<xg::shape::Polyline>(lineWidth, points, axis->gridCfg["stroke"], "", false);
                l->SetZIndex(-1);
                std::vector<float> dash = xg::json::ParseDashArray(axis->gridCfg["dash"], context.GetDevicePixelRatio());
                l->SetDashLine(dash);
                this->container_->AddElement(std::move(l));
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
            DrawLine(chart, std::move(line), axis->lineCfg);
        }
        // TODO draw tick line
    }

    tracer->trace("%s %lu", "draw txt labels: ", axis->labels.size());
    // std::for_each(axis.txtLabels.begin(), axis.txtLabels.end(),
    //               [&](xg::axis::LabelTXT &label) { DrawLabel(chart, axis, label, container, context); });

    DrawLabel(chart, axis, context);

    tracer->trace("%s\n", "end draw axis;");
}

void xg::axis::AxisController::DrawLine(XChart &chart, std::array<util::Point, 2> &&line, const nlohmann::json &lineCfg) {
    float lineWidth = lineCfg["lineWidth"].get<float>() * chart.GetRatio();
    if(lineCfg["type"] == "line") {
        std::unique_ptr<xg::shape::Element> _line = xg::make_unique<xg::shape::Line>(line[0], line[1], lineWidth, lineCfg["color"]);
        this->container_->AddElement(std::move(_line));
    } else if(lineCfg["type"] == "dash") {
        std::vector<util::Point> _points = {line[0], line[1]};
        auto l = xg::make_unique<xg::shape::Polyline>(lineWidth, _points, false);
        l->strokeStyle_ = util::ColorParser(lineCfg["color"]);
        l->SetZIndex(-1);
        std::vector<float> dash = xg::json::ParseDashArray(lineCfg["dash"], chart.GetRatio());
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
        auto txtPoint = text->GetPoint();
        pt.x += txtPoint.x;
        pt.y += txtPoint.y;

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
            pt.x += labelOffset;

            if(text->GetTextAlign() == "center") {
                pt.x -= axis->maxWidth / 2;
            } else if(text->GetTextAlign() == "left" || text->GetTextAlign() == "start") {
                pt.x -= axis->maxWidth;
            }

        } else if(axis->position == "right") {
            if(isFirst) {
                pt.y -= labelMargin;
            } else if(isLast) {
                pt.y += labelMargin;
            }
            pt.y += bbox.height / 2;
            pt.x += labelOffset; // + bbox.width / 2;

            if(text->GetTextAlign() == "center") {
                pt.x += axis->maxWidth / 2;
            } else if(text->GetTextAlign() == "right" || text->GetTextAlign() == "end") {
                pt.x += axis->maxWidth;
            }
        }
        text->SetPoint(pt);
        this->container_->AddElement(std::move(text));
    }
}

void xg::axis::AxisController::Clear() {
    this->axes.clear();
    this->container_->Clear();
}
