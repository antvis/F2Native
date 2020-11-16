#include "graphics/XChart.h"
#include "graphics/canvas/Cartesian.h"
#include "graphics/canvas/Polar.h"
#include "graphics/geom/shape/Area.h"
#include "graphics/geom/shape/Interval.h"
#include "graphics/geom/shape/Line.h"
#include "graphics/util/Point.h"
#include <cstring>
#include <utils/common.h>
#include <utils/xtime.h>

using namespace xg;
using namespace std;

XChart::XChart(const std::string &name, double width, double height, double ratio) : chartName_(name) {
    width_ = width * ratio;
    height_ = height * ratio;
    ratio_ = ratio;
    this->logTracer_ = new utils::Tracer(name + "@Chart");
    this->logTracer_->trace("create xchart: %s %lf %lf %lf ", chartName_.c_str(), width_, height_, ratio_);

    geomShapeFactory_ = xg::make_unique<geom::shape::GeomShapeFactory>();

    // 初始化度量控制器
    scaleController_ = new scale::ScaleController();
    this->logTracer_->trace("%s", "new ScaleController instance.");
    // 画布
    this->canvas_ = new canvas::Canvas();
    this->logTracer_->trace("%s", "new canvas instance.");
    // 布局层级
    this->InitLayout();

    axisController_ = new axis::AxisController();
    this->logTracer_->trace("%s", "new axisController instance.");
    guideController_ = new guide::GuideController();
    this->logTracer_->trace("%s", "new guideControlelr instance.");
    legendController_ = new legend::LegendController();
    this->logTracer_->trace("%s", "new legendController instance");

    eventController_ = new event::EventController();
    interactionContext_ = new interaction::InteractionContext(this);
    this->logTracer_->trace("%s", "new eventController instance.");
}

XChart::~XChart() {
    actionListeners_.clear();
    interactions_.clear();

    XG_RELEASE_POINTER(canvas_);
    XG_RELEASE_POINTER(scaleController_);
    XG_RELEASE_POINTER(axisController_);
    XG_RELEASE_POINTER(guideController_);

    std::for_each(geoms_.begin(), geoms_.end(), [&](geom::AbstractGeom *geom) { delete geom; });
    geoms_.clear();

    XG_RELEASE_POINTER(tooltipController_);
    XG_RELEASE_POINTER(canvasContext_);
    XG_RELEASE_POINTER(interactionContext_);
    XG_RELEASE_POINTER(eventController_);
    XG_RELEASE_POINTER(logTracer_);
}

XChart &XChart::Source(const std::string &json) {
#if defined(DEBUG)
    nlohmann::json _data = nlohmann::json::parse(json);
#else
    nlohmann::json _data = nlohmann::json::parse(json, nullptr, false);
#endif
    this->logTracer_->trace("#Source dataSize: %lu", json.size());
    if(!_data.is_array()) {
        // 有问题先抛出来
        this->logTracer_->trace("#Source json is invalid");
        // throw std::runtime_error("json is invalid.");
        return *this;
    }

    this->data_ = _data;
    return *this;
}

XChart &XChart::Padding(double left, double top, double right, double bottom) {
    this->padding_[0] = left * ratio_;
    this->padding_[1] = top * ratio_;
    this->padding_[2] = right * ratio_;
    this->padding_[3] = bottom * ratio_;

    userPadding_ = padding_;
    return *this;
}

XChart &XChart::Margin(double left, double top, double right, double bottom) {
    this->margin_[0] = left * ratio_;
    this->margin_[1] = top * ratio_;
    this->margin_[2] = right * ratio_;
    this->margin_[3] = bottom * ratio_;
    return *this;
}

void XChart::UpdateLayout(std::array<double, 4> newPadding) {
    this->coord_->Reset(util::Point{margin_[0] + newPadding[0], margin_[1] + this->height_ - newPadding[3]},
                        util::Point{margin_[0] + this->width_ - newPadding[2], margin_[1] + newPadding[1]});
    this->padding_ = newPadding;
}

XChart &XChart::Axis(const std::string &field, nlohmann::json config) {
    this->logTracer_->trace("#Axis field: %s config: %s", field.c_str(), config.dump().c_str());
    this->axisController_->SetFieldConfig(field, config);
    return *this;
}

XChart &XChart::Interaction(const std::string &type) {
    if(type == "pinch") {
        std::unique_ptr<interaction::InteractionBase> pinch = std::make_unique<interaction::Pinch>(this);
        interactions_.push_back(std::move(pinch));
    } else if(type == "pan") {
        std::unique_ptr<interaction::InteractionBase> pan = std::make_unique<interaction::Pan>(this);
        interactions_.push_back(std::move(pan));
    }
    return *this;
}

XChart &XChart::Coord(nlohmann::json config) {
    this->logTracer_->trace("#coord ");
    this->coordCfg_.merge_patch(config);
    bool _transposed = this->coordCfg_["transposed"];
    if(this->coord_ != nullptr) {
        this->coord_->SetTransposed(_transposed);
        // TODO repaint
    }
    return *this;
}
XChart &XChart::Legend(const std::string &field, nlohmann::json config) {
    this->logTracer_->trace("#Legend field: %s config: %s", field.c_str(), config.dump().c_str());
    this->legendController_->SetFieldConfig(field, config);
    return *this;
}

void XChart::OnTouchEvent(nlohmann::json cfg) {
    if(!cfg.is_object() || !cfg.contains("eventType") || !cfg.contains("points"))
        return;
    event::Event event;
    event.eventType = cfg["eventType"];
    nlohmann::json &_points = cfg["points"];
    for(int i = 0; i < _points.size(); ++i) {
        nlohmann::json &_point = _points[i];
        util::Point point{_point["x"], _point["y"]};
        event.points.push_back(std::move(point));
    }
    event.devicePixelRatio = ratio_;
    event.timeStamp = xg::CurrentTimestampAtMM();
    this->eventController_->OnTouchEvent(event);
}

XChart &XChart::Scale(const std::string &field, nlohmann::json config) {
    this->logTracer_->trace("#Scale field: %s config: %s", field.c_str(), config.dump().c_str());
    this->scaleController_->UpdateColConfig(field, config);
    return *this;
}

scale::AbstractScale &XChart::GetScale(const std::string &field) {
    return *this->scaleController_->CreateScale(field, this->data_, this->logTracer_, this->coord_).get();
}

geom::Line &XChart::Line() {
    this->logTracer_->trace("#Line %s", "create Geom@Line");
    geom::Line *line = new geom::Line(this->logTracer_);
    this->geomShapeFactory_->RegisterGeomShape(line->GetType(), xg::make_unique<xg::geom::shape::Line>());
    this->geoms_.push_back(line);
    return *line;
}

geom::Interval &XChart::Interval() {
    this->logTracer_->trace("#Interval %s", "create Geom@Interval");
    geom::Interval *interval = new geom::Interval(this->logTracer_);
    this->geomShapeFactory_->RegisterGeomShape(interval->GetType(), xg::make_unique<xg::geom::shape::Interval>());
    this->geoms_.push_back(interval);
    return *interval;
}

geom::Area &XChart::Area() {
    this->logTracer_->trace("#Area %s", "create Geom@Area");
    geom::Area *area = new geom::Area(this->logTracer_);
    this->geomShapeFactory_->RegisterGeomShape(area->GetType(), xg::make_unique<xg::geom::shape::Area>());
    this->geoms_.push_back(area);
    return *area;
}

void XChart::Render() {
    this->logTracer_->trace("#Render %s", "start render");
    if(canvasContext_ == nullptr) {
        this->logTracer_->trace("error: %s", "canvasContext is nullptr, render end.");
        return;
    }

#ifdef ANDROID
    if(!canvasContext_->IsValid()) {
        this->logTracer_->trace("error: %s", "canvasContext is not valid, render end.");
        return;
    }
#endif
    if(!this->data_.is_array() || this->data_.size() == 0) {
        this->logTracer_->trace("error: %s", "data is not array or size is zero, render end.");
        return;
    }

    long startTimeStamp = xg::CurrentTimestampAtMM();

    if(!rendered_) {
        // 1. init Coord
        this->logTracer_->trace("%s padding: [%lf, %lf, %lf, %lf]", "call InitCoord", padding_[0], padding_[1], padding_[2], padding_[3]);
        this->InitCoord();
        // 2. init Geoms

        this->logTracer_->trace("%s", "foreach geom init");
        std::for_each(geoms_.begin(), geoms_.end(), [this](geom::AbstractGeom *geom) -> void { geom->Init(this); });
        rendered_ = true;

        this->NotifyAction(ACTION_CHART_AFTER_INIT);
    }
    // 3. legend render
    this->logTracer_->trace("%s", "legendController render");
    legendController_->Render(*this, backLayout_);

    // 4. render Axis
    this->logTracer_->trace("%s", "draw axies");
    axisController_->DrawAxes(this, backLayout_, *canvasContext_);

    // 5. geom paint
    this->logTracer_->trace("%s", "foreach geom paint");
    std::for_each(geoms_.begin(), geoms_.end(), [this](geom::AbstractGeom *geom) { geom->Paint(this, this->midLayout_); });
    // 6. guide render
    this->NotifyAction(ACTION_CHART_AFTER_GEOM_DRAW);

    // 6.1 guide render
    this->logTracer_->trace("%s", "guideController render");
    guideController_->Render(*this, this->frontLayout_, *canvasContext_);

    // 7. children sort
    this->logTracer_->trace("%s", "canvas#sort");
    this->canvas_->Sort();

    // 7.-1 beforeDraw
    this->logTracer_->trace("%s", "beforeDraw and call canvas clearRect");
    canvasContext_->ClearRect(margin_[0], margin_[1], width_, height_);

    // 6. canvas draw
    this->logTracer_->trace("%s", "canvas#startDraw");
    this->canvasContext_->Reset();
    this->canvas_->Draw(*canvasContext_);
    this->NotifyAction(ACTION_CHART_AFTER_RENDER);

    renderDurationMM_ = xg::CurrentTimestampAtMM() - startTimeStamp;

    long count = this->canvasContext_->GetRenderCount();
    this->logTracer_->trace("%s renderCount: %ld, duration: %lu", "canvas#endDraw", count, renderDurationMM_);
}

void XChart::Repaint() {
    if(!rendered_)
        return;

    // TODO legend clear
    ClearInner();
    Render();
}

std::string XChart::GetRenderInfo() const {
    nlohmann::json info;

    // 渲染执行执行数量
#ifdef ANDROID
    if(canvasContext_ == nullptr || !canvasContext_->IsValid()) {
#else
    if(canvasContext_ == nullptr) {
#endif
        info["cmdCount"] = -1; // < 0 表示 canvas 环境有问题
    } else {
        info["cmdCount"] = canvasContext_->GetRenderCount();
    }

    info["duration"] = renderDurationMM_;
    return info.dump();
}

XChart &XChart::Tooltip(const nlohmann::json &cfg) {
    if(this->tooltipController_ == nullptr) {
        this->tooltipController_ = new tooltip::ToolTipController(this);

        this->tooltipController_->AddMonitor(
            XG_MEMBER_OWNER_CALLBACK_1(legend::LegendController::OnToolTipMarkerItemsChanged, this->legendController_));
    }

    this->tooltipController_->Init(cfg);
    return *this;
}

void XChart::InitLayout() {
    std::unique_ptr<shape::Group> layout_back = xg::make_unique<shape::Group>();
    backLayout_ = layout_back.get();
    layout_back->SetZIndex(0);
    this->canvas_->AddElement(std::move(layout_back));

    std::unique_ptr<shape::Group> layout_mid = xg::make_unique<shape::Group>();
    midLayout_ = layout_mid.get();
    layout_mid->SetZIndex(10);
    this->canvas_->AddElement(std::move(layout_mid));

    std::unique_ptr<shape::Group> layout_front = xg::make_unique<shape::Group>();
    frontLayout_ = layout_front.get();
    layout_front->SetZIndex(20);
    this->canvas_->AddElement(std::move(layout_front));
}

void XChart::InitCoord() {
    bool _transposed = coordCfg_["transposed"];
    const std::string &type = coordCfg_["type"];
    util::Point start = util::Point{this->margin_[0] + this->padding_[0], this->margin_[1] + this->height_ - this->padding_[3]};
    util::Point end = util::Point{this->margin_[0] + this->width_ - this->padding_[2], this->margin_[1] + this->padding_[1]};
    if(type == "polar") {
        this->coord_ = xg::make_unique<canvas::coord::Polar>(start, end, _transposed);
    } else {
        this->coord_ = xg::make_unique<canvas::coord::Cartesian>(start, end, _transposed);
    }
}

const util::Point XChart::GetPosition(const nlohmann::json &item) {
    if(!item.is_object() || item.size() < 2) {
        return util::Point{0, 0};
    }

    std::string xField = this->GetXScaleField();
    std::string yField = this->getYScaleFields()[0];

    if(!item.contains(xField) || !item.contains(yField)) {
        return util::Point{0, 0};
    }

    double x = this->GetScale(xField).Scale(xField);
    double y = this->GetScale(yField).Scale(yField);
    util::Point ret = this->GetCoord().ConvertPoint(util::Point{x, y});

    return ret;
}

void XChart::ClearInner() {
    backLayout_->Clear();
    midLayout_->Clear();
    frontLayout_->Clear();

    this->legendController_->ClearInner();

    this->padding_ = userPadding_;

    if(this->coord_ != nullptr) {
        util::Point start = util::Point{this->margin_[0] + this->padding_[0], this->margin_[1] + this->height_ - this->padding_[3]};
        util::Point end = util::Point{this->margin_[0] + this->width_ - this->padding_[2], this->margin_[1] + this->padding_[1]};

        this->coord_->Reset(start, end);
    }

    NotifyAction(ACTION_CHART_CLEAR_INNER);

    axisController_->Clear();
}

std::map<std::string, std::vector<legend::LegendItem>> XChart::GetLegendItems() {

    std::map<std::string, std::vector<legend::LegendItem>> legendItems;

    std::for_each(geoms_.begin(), geoms_.end(), [&](geom::AbstractGeom *geom) -> void {
        if(geom->ContainsAttr(attr::AttrType::Color)) {
            const unique_ptr<attr::AttrBase> &_attr = geom->GetColor();
            if(!_attr->GetFields().empty()) {
                const attr::Color &colorAttr = static_cast<const attr::Color &>(*_attr.get());

                scale::AbstractScale &scale = GetScale(colorAttr.GetFields()[0]);

                if(scale::IsCategory(scale.GetType())) {
                    const Category &cat = static_cast<Category &>(scale);
                    std::vector<scale::Tick> ticks = scale.GetTicks();

                    std::vector<legend::LegendItem> fieldItems;

                    std::for_each(ticks.begin(), ticks.end(), [&](const scale::Tick &tick) -> void {
                        legend::LegendItem item;
                        item.name = tick.text;
                        item.field = cat.GetField();
                        item.marker.fill = colorAttr.GetColor(cat.Transform(tick.tickValue));
                        fieldItems.push_back(std::move(item));
                    });
                    legendItems[cat.field] = std::move(fieldItems);
                }
            }
        }
    });

    return legendItems;
}
