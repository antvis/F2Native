#include "graphics/axis/AxisController.h"
#include "graphics/XChart.h"
#include "graphics/func/Func.h"
#include "graphics/shape/Circle.h"
#include "graphics/shape/Line.h"
#include "graphics/shape/Polyline.h"
#include "graphics/shape/Text.h"
#include "graphics/shape/Rect.h"
#include "graphics/shape/Polygon.h"
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

static std::vector<std::string> GetFillColor(const nlohmann::json &config) {
    std::vector<string> fill;
    if (config.is_array()) {
        for (int i = 0; i < config.size(); ++i) {
            fill.push_back(config[i]);
        }
    }else if(config.is_string()) {
        fill.push_back(config.get<string>());
    }
    return fill;
}

const bool xg::axis::Axis::IsSelected(const int tickIndex) {
    bool isSelected = false;
    if (this->selectedLabelCfg.size() > 0 && this->selectedArray.size() > 0) {
        for (nlohmann::json &defaultSelectedItem : this->selectedArray) {
            int index = json::GetIntNumber(defaultSelectedItem, "tickIndex", -1);
            if (tickIndex == index) {
                isSelected = true;
                break;
            }
        }
    }
    return isSelected;
}

inline const nlohmann::json &xg::axis::Axis::GetLabelCfg() {
    return this->labelCfg;
}

const nlohmann::json xg::axis::Axis::GetLabelCfg(const bool isSelected) {
    if (isSelected) {
        nlohmann::json config = this->labelCfg;
        config.merge_patch(this->selectedLabelCfg);
        return config;
    } else {
        return this->labelCfg;
    }
}

const nlohmann::json xg::axis::Axis::GetLabelCfgForIndex(const int tickIndex) {
    const bool isSelected = this->IsSelected(tickIndex);
    return this->GetLabelCfg(isSelected);
}

const std::tuple<std::vector<size_t>, std::vector<xg::scale::Tick>> xg::axis::Axis::GetSnapTicks(XChart *chart, util::Point point, const bool needReMapping) {
    std::vector<scale::Tick> rst{};
    std::vector<size_t> rstIdx{};
    const size_t labelsSize = this->labelBBoxes.size();
    for (size_t i = 0; i < this->labelBBoxes.size(); i++) {
        const BBox &bbox = this->labelBBoxes[i];
        bool labelCanHandle = false;
        labelCanHandle = util::BBoxUtil::IsPointHitBBox(point, bbox, this->dimType);
        if (labelCanHandle) {
            const size_t ticksSize = ticks.size();
            if (i < ticksSize && ticksSize == labelsSize) {
                rst.push_back(this->ticks[i]);
                rstIdx.push_back(i);
            }
        }
    }
    return std::make_tuple(rstIdx, rst);
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

void xg::axis::AxisController::associateGeomTap(XChart *chart, util::Point point, nlohmann::json &geomRecords) {
    bool canHandle = false;
    for (auto &record : geomRecords) {
        const int recordIndex = json::GetIntNumber(record, "recordIndex", -1);
        if (recordIndex >= 0) {
            for (std::unique_ptr<xg::axis::Axis> &axis : this->axes) {
                if (!axis->associateRecord) {
                    break;
                }
                std::vector<xg::scale::Tick> ticks;
                nlohmann::json newSelectedArray = {};
                const size_t ticksSize = axis->ticks.size();
                if (recordIndex < ticksSize) {
                    canHandle = true;
                    ticks.push_back(axis->ticks[recordIndex]);
                    nlohmann::json newSelectedArray = {};
                    newSelectedArray.push_back({{"tickIndex", recordIndex}});
                    axis->selectedArray = newSelectedArray;
                    this->fieldSelectedArray_[axis->field] = newSelectedArray;
                }
            }
        }
    }
    
    if (canHandle) {
        this->isRepainting = true;
    }
}

const std::tuple<const bool, const bool, const nlohmann::json, const xg::scale::Tick> xg::axis::AxisController::onTap(XChart *chart, util::Point point) {
    bool canHandle = false;
    bool isSelect = true;
    nlohmann::json tickInfo = {};
    scale::Tick tickObj;
    for (std::unique_ptr<xg::axis::Axis> &axis : this->axes) {
        std::vector<size_t> indexes;
        std::vector<xg::scale::Tick> ticks;
        std::tie(indexes, ticks) = axis->GetSnapTicks(chart, point, true);
        if (indexes.size() > 0) {
            canHandle = true;
            const scale::Tick &tick = ticks[0];
            const size_t tickIdx = indexes[0];
            
            nlohmann::json newSelectedArray = {};
            for (const nlohmann::json &selectedItem : axis->selectedArray) {
                if (tickIdx == json::GetIntNumber(selectedItem, "tickIndex", -1)) {
                    isSelect = false;
                    if (!axis->deselect) {
                        canHandle = false;
                    }
                } else {
//                    newSelectedArray.push_back(selectedItem);
                }
            }
            
            if (canHandle) {
                if (isSelect) {
                    newSelectedArray.push_back({{"tickIndex", tickIdx}});
                }
                axis->selectedArray = newSelectedArray;
                this->fieldSelectedArray_[axis->field] = newSelectedArray;
                if (axis->associateRecord) {
                    tickInfo = {
                        {"tickIndex", tickIdx},
                        {"recordIndex", tickIdx},
                        {"field", axis->field},
                    };
                    tickObj = tick;
                } else {
                    tickInfo = {
                        {"tickIndex", tickIdx},
                        {"field", axis->field},
                    };
                    tickObj = tick;
                }
                break;
            }
        }
    };
    
    if (canHandle) {
        this->isRepainting = true;
    }
    
    return std::make_tuple(canHandle, isSelect, tickInfo, tickObj);
}

const std::string xg::axis::AxisController::GetAxisPosition(const std::string &field) {
    std::string position;
    std::for_each(this->axes.begin(), this->axes.end(), [&](std::unique_ptr<xg::axis::Axis> &axis){
        if (axis->field == field) {
            position = axis->position;
        }
    });
    return position;
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

    nlohmann::json &fieldCfg = axisConfig_[axis->field];
    nlohmann::json &fieldSlt = axisSelection_[axis->field];
    
    if (this->isRepainting) {
        axis->selectedArray = this->fieldSelectedArray_[axis->field];
    } else if (fieldSlt["defaultSelected"].is_array()) {
        axis->selectedArray = fieldSlt["defaultSelected"];
        this->fieldSelectedArray_[axis->field] = axis->selectedArray;
    }
    
    if (fieldSlt["associateRecord"].is_boolean()) {
        axis->associateRecord = json::GetBool(fieldSlt, "associateRecord", true);
    }
    
    if (fieldSlt["deselect"].is_boolean()) {
        axis->deselect = json::GetBool(fieldSlt, "deselect", false);
    }
    
    if (fieldSlt["enable"].is_boolean()) {
        axis->selectionEnable = json::GetBool(fieldSlt, "enable", false);
    }

    bool showGrid = axis->position == "bottom"; // bottom 默认显示轴
    if(fieldCfg["grid"].is_object()) {
        showGrid = true;
    }

    if(showGrid) {
        // 默认值
        axis->gridCfg = fieldCfg["grid"];
        if (fieldSlt["selectedGrid"].is_object()) {
            axis->selectedGridCfg = fieldSlt["selectedGrid"];
        }
    }

    if(fieldCfg["label"].is_object()) {
        axis->labelCfg = fieldCfg["label"];
        if (fieldSlt["selectedLabel"].is_object()) {
            axis->selectedLabelCfg = fieldSlt["selectedLabel"];
        }
        //            if(axis->key == "left") {
        //                axis->labelCfg["textAlign"] = "end";
        //            } else if(axis->key == "right") {
        //                axis->labelCfg["textAlign"] = "start";
        //            }
    }

    if(fieldCfg["line"].is_object()) {
        axis->lineCfg = fieldCfg["line"];
    }
    
    this->isRepainting = false;

    scale::AbstractScale &scale = chart.GetScale(axis->field);
    axis->ticks = scale.GetTicks(&chart);

    int ticksTmp = 1;
    if(axis->ticks.size() > 600) {
        ticksTmp = 200;
    }

    const nlohmann::json &labelCfg = axis->GetLabelCfg();
    if (labelCfg.is_object()) {
        const float pixelRatio = chart.GetCanvasContext().GetDevicePixelRatio();

        std::string labelItemFuncId;
        if(labelCfg.contains("item")) {
            labelItemFuncId = labelCfg["item"];
        }

        for(std::size_t index = 0; index < axis->ticks.size(); ++index) {
            if(index == 0 || index == (axis->ticks.size() - 1) || index % ticksTmp == 0) {
                const xg::scale::Tick &tick = axis->ticks[index];
                nlohmann::json itemStyle = axis->GetLabelCfgForIndex((int)index);
                const float maxWidth = json::GetNumber(itemStyle, "maxWidth", 0) * pixelRatio;
                const auto &textAlign = itemStyle["textAlign"];
                const auto &textBaseline = itemStyle["textBaseline"];
                const bool inner = json::GetBool(itemStyle, "inner");
                if(!labelItemFuncId.empty()) {
                    nlohmann::json param = {{"text", tick.text}, {"index", index}, {"count", axis->ticks.size()}, {"content", tick.text}};
                    param.merge_patch(itemStyle);
                    
                    itemStyle["xOffset"] = 0;
                    itemStyle["yOffset"] = 0;

                    nlohmann::json itemStyleRst = xg::json::ParseString(chart.InvokeFunction(labelItemFuncId, param.dump()));
                    if(itemStyleRst.is_object()) {
                        itemStyle.merge_patch(itemStyleRst);
                    }

                    float textSize = itemStyle["textSize"];
                    textSize *= pixelRatio;

                    float xOffset = itemStyle["xOffset"];
                    float yOffset = itemStyle["yOffset"];
                    xOffset *= pixelRatio;
                    yOffset *= pixelRatio;

                    std::string textContent = json::GetString(itemStyle, "content", tick.text);

                    std::unique_ptr<xg::shape::Text> text =
                        xg::make_unique<xg::shape::Text>(textContent, util::Point{xOffset, yOffset}, textSize, "", itemStyle["textColor"]);
                    // maxWidth_<1不会改变文字大小
                    text->AdaptTextFontSize(chart.GetCanvasContext(), 0, 1, maxWidth);

                    text->ext["tickValue"] = tick.value;
                    if(itemStyle["textAlign"].is_string()) {
                        text->SetTextAlign(itemStyle["textAlign"]);
                    }
                    
                    if (itemStyle["textBaseline"].is_string()) {
                        text->SetTextBaseline(itemStyle["textBaseline"]);
                    }

                    if (itemStyle.contains("font")){
                        nlohmann::json font = json::GetObject(itemStyle, "font");
                        if (!font.is_null()){
                            std::string fontStyle = json::GetString(font, "fontStyle", "");
                            std::string fontVariant = json::GetString(font, "fontVariant", "");
                            std::string fontWeight = json::GetString(font, "fontWeight", "");
                            std::string fontFamily = json::GetString(font, "fontFamily", "");
                            text->SetTextFont(fontStyle, fontVariant, fontWeight, fontFamily);
                        }
                    }

                    xg::util::BBox bbox = text->GetBBox(chart.GetCanvasContext());

                    axis->maxWidth = inner ? 0 : fmax(axis->maxWidth, static_cast<double>(bbox.width));
                    axis->maxHeight = inner ? 0 : fmax(axis->maxHeight, static_cast<double>(bbox.height));

                    axis->labels.push_back(std::move(text));
                } else {
                    float labelFontSize = itemStyle["textSize"];
                    labelFontSize *= pixelRatio;
                    const std::string &labelColor = itemStyle["textColor"];
                    std::unique_ptr<xg::shape::Text> text =
                    xg::make_unique<xg::shape::Text>(tick.text, util::Point{0, 0}, labelFontSize, "", labelColor);
                    // maxWidth_<1不会改变文字大小
                    text->AdaptTextFontSize(chart.GetCanvasContext(), 0, 1, maxWidth);

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

                    if (itemStyle.contains("font")){
                        nlohmann::json font = json::GetObject(itemStyle, "font");
                        if (!font.is_null()){
                            std::string fontStyle = json::GetString(font, "fontStyle", "");
                            std::string fontVariant = json::GetString(font, "fontVariant", "");
                            std::string fontWeight = json::GetString(font, "fontWeight", "");
                            std::string fontFamily = json::GetString(font, "fontFamily", "");
                            text->SetTextFont(fontStyle, fontVariant, fontWeight, fontFamily);
                        }
                    }

                    xg::util::BBox bbox = text->GetBBox(chart.GetCanvasContext());

                    axis->maxWidth = inner ? 0 : fmax(axis->maxWidth, static_cast<double>(bbox.width));
                    axis->maxHeight = inner ? 0 : fmax(axis->maxHeight, static_cast<double>(bbox.height));

                    axis->labels.push_back(std::move(text));
                }
            }
        }
        
        float maxWidth_ = json::GetNumber(labelCfg, "maxWidth", 0) * pixelRatio;
        // 标签在轴线外不，使用业务传入的宽度
        if (maxWidth_ >= 1 && !json::GetBool(labelCfg, "inner")) {
            axis->maxWidth = maxWidth_;
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
                float labelOffset = json::GetNumber(axis->labelCfg, "labelOffset") * chart.GetCanvasContext().GetDevicePixelRatio();
                newPadding[0] += axis->maxWidth + labelOffset;
                newPadding[1] += axis->maxHeight + labelOffset;
                newPadding[2] += axis->maxWidth + labelOffset;
                newPadding[3] += axis->maxHeight + labelOffset;
            }            
        });
        chart.UpdateLayout(newPadding);
    } else {
        std::for_each(axes.begin(), axes.end(), [&](std::unique_ptr<xg::axis::Axis> &axis) {
            float labelOffset = 0.f;
            if(axis->labelCfg.is_object()) {
                labelOffset = axis->labelCfg["labelOffset"];
            }
            labelOffset *= chart.GetCanvasContext().GetDevicePixelRatio();

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
    bool hideGrid = coord.GetType() == coord::CoordType::Polar && axis->labelCfg.is_null() && axis->lineCfg.is_null();
    // draw grid
    if(!hideGrid &&
       axis->gridCfg.is_object() &&
       !axis->verticalField.empty() &&
       !axis->gridPoints.empty()) {
        tracer->trace("%s", "draw grid line");
        float lineWidth = axis->gridCfg["lineWidth"].get<float>() * chart.GetCanvasContext().GetDevicePixelRatio();
        auto &type = axis->gridCfg["type"] ;
        auto position = axis->position;
        auto &stroke = json::GetString(axis->gridCfg, "stroke");
       
        std::vector<float> dash = xg::json::ParseDashArray(axis->gridCfg["dash"], context.GetDevicePixelRatio());
        
        //雷达图轴上的背景色
        std::vector<string> fill = GetFillColor(axis->gridCfg["fill"]);
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

        bool drawLine = axis->lineCfg.is_object();
        if(drawLine) {
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

void xg::axis::AxisController::DrawLine(XChart &chart, std::array<util::Point, 2> &&line, const nlohmann::json &lineCfg) {
    float lineWidth = lineCfg["lineWidth"].get<float>() * chart.GetCanvasContext().GetDevicePixelRatio();
    if(lineCfg["type"] == "line") {
        std::unique_ptr<xg::shape::Element> _line = xg::make_unique<xg::shape::Line>(line[0], line[1], lineWidth, lineCfg["color"]);
        this->container_->AddElement(std::move(_line));
    } else if(lineCfg["type"] == "dash") {
        std::vector<util::Point> _points = {line[0], line[1]};
        auto l = xg::make_unique<xg::shape::Polyline>(lineWidth, _points, false);
        l->SetStorkColor(lineCfg["color"]);
        l->SetZIndex(-1);
        std::vector<float> dash = xg::json::ParseDashArray(lineCfg["dash"], chart.GetCanvasContext().GetDevicePixelRatio());
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

    auto &coord = chart.GetCoord();
    size_t size = axis->labels.size();
    
    //textAlign 默认center textBaseline 默认bottom
    //pt默认对应的是字体中下方的锚点
    int index = 0;
    axis->labelBBoxes.clear();
    while(axis->labels.size() > 0) {
        auto text = std::move(axis->labels[0]);
        axis->labels.erase(axis->labels.begin());
        index++;

        bool isFirst = axis->labels.size() == (size - 1);
        bool isLast = axis->labels.size() == 0;
        
        const nlohmann::json &labelCfg = axis->GetLabelCfgForIndex(index);
        float labelMargin = 0.f;
        // 是否每个label都可设置labelMargin，默认只有首位两个label,true的化每个label都可使用
        bool labelMarginUseAll = false;
        if (labelCfg.is_object()) {
            labelMargin = labelCfg["labelMargin"];
            labelMarginUseAll = json::GetBool(labelCfg, "labelMarginUseAll", false);
        }
        labelMargin *= context.GetDevicePixelRatio();

        float labelOffset = 0.f;
        if (labelCfg.is_object()) {
            labelOffset = labelCfg["labelOffset"];
        }
        labelOffset *= context.GetDevicePixelRatio();

        double tickValue = text->ext["tickValue"];
        xg::util::Point pt = axis->GetOffsetPoint(tickValue, labelOffset);
        string dimType = axis->dimType;
        if(chart.GetCoord().IsTransposed()) {
            dimType = axis->dimType == "x" ? "y" : "x";
        }
        
        bool inner = json::GetBool(labelCfg, "inner");
        
        //等于把pt.y轴下移了5个dp，textbaseline变成了middle
        //这时候textAlign是center textBaseline是middle，等于pt对应了字体中心的锚点
        GetSidePoint(pt, inner ? 0 : 5 * chart.GetCanvasContext().GetDevicePixelRatio(), dimType);
        auto txtPoint = text->GetPoint();
        pt.x += txtPoint.x;
        pt.y += txtPoint.y;

        xg::util::BBox bbox = text->GetBBox(context);

        if(axis->position == "bottom") {
             if (labelMarginUseAll){
                pt.x += labelMargin;
            } else if(isFirst) {
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
        
        // 这里要重新计算bbox，但是老的逻辑一般都是GetBBox获取，axis的text在这里一直有问题
        xg::util::BBox bboxNew = text->CalculateBox(context);
        axis->labelBBoxes.push_back(std::move(bboxNew));
        
        this->container_->AddElement(std::move(text));
    }
}

void xg::axis::AxisController::Clear() {
    this->axes.clear();
    this->container_->Clear();
}
