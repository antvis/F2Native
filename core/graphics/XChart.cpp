#include "graphics/XChart.h"
#include "graphics/animate/TimeLine.h"
#include "graphics/canvas/Cartesian.h"
#include "graphics/canvas/Polar.h"
#include "graphics/func/Func.h"
#include "graphics/geom/shape/Area.h"
#include "graphics/geom/shape/Candle.h"
#include "graphics/geom/shape/Interval.h"
#include "graphics/geom/shape/Line.h"
#include "graphics/geom/shape/Point.h"
#include "graphics/util/Point.h"
#include "graphics/util/json.h"
#include <cstring>
#include <utils/common.h>
#include <utils/xtime.h>

using namespace xg;
using namespace std;

XChart::XChart(const std::string &name, double width, double height, double ratio) : chartName_(name) {
    this->chartId_ = chartName_ + "_" + std::to_string(CurrentTimestampAtMM());
    width_ = width * ratio;
    height_ = height * ratio;
    ratio_ = ratio;
    this->logTracer_ = new utils::Tracer(name + "@Chart");
    this->logTracer_->trace("create xchart: %s %lf %lf %lf ", chartId_.c_str(), width_, height_, ratio_);

    geomShapeFactory_ = xg::make_unique<geom::shape::GeomShapeFactory>();

    // 初始化度量控制器
    scaleController_ = new scale::ScaleController();
    this->logTracer_->trace("%s", "new ScaleController instance.");
    // 画布
    this->canvas_ = new canvas::Canvas(this);
    this->logTracer_->trace("%s", "new canvas instance.");
    // 布局层级
    this->InitLayout();

    axisController_ = new axis::AxisController(backLayout_->AddGroup());
    this->logTracer_->trace("%s", "new axisController instance.");
    guideController_ = new guide::GuideController(frontLayout_->AddGroup(), backLayout_->AddGroup());
    this->logTracer_->trace("%s", "new guideController instance.");
    legendController_ = new legend::LegendController(backLayout_->AddGroup());
    this->logTracer_->trace("%s", "new legendController instance");

    eventController_ = new event::EventController();
    interactionContext_ = new interaction::InteractionContext(this);
    this->logTracer_->trace("%s", "new eventController instance.");

    geomAnimate_ = new animate::GeomAnimate(this);
    this->logTracer_->trace("%s", "new GeomAnimate instance.");
}

XChart::~XChart() {
    func::FunctionManager::GetInstance().Clear(this->GetChartId());

    chartActionListeners_.clear();
    renderActionListeners_.clear();
    interactions_.clear();

    XG_RELEASE_POINTER(scaleController_);
    XG_RELEASE_POINTER(axisController_);
    XG_RELEASE_POINTER(guideController_);
    XG_RELEASE_POINTER(geomAnimate_);

    geoms_.clear();

    XG_RELEASE_POINTER(tooltipController_);
    XG_RELEASE_POINTER(canvasContext_);
    XG_RELEASE_POINTER(interactionContext_);
    XG_RELEASE_POINTER(eventController_);
    XG_RELEASE_POINTER(logTracer_);

    backLayout_ = nullptr;
    midLayout_ = nullptr;
    frontLayout_ = nullptr;
    XG_RELEASE_POINTER(canvas_);
}

XChart &XChart::Source(const std::string &json) {
    nlohmann::json _data = xg::json::ParseString(json);
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

XChart &XChart::Axis(const std::string &field, const std::string &json) {
    nlohmann::json config = xg::json::ParseString(json);
    this->logTracer_->trace("#Axis field: %s config: %s", field.c_str(), config.dump().c_str());
    this->axisController_->SetFieldConfig(field, config);
    return *this;
}

XChart &XChart::Interaction(const std::string &type, nlohmann::json config) {
    if(type == "pinch") {
        std::unique_ptr<interaction::InteractionBase> pinch = std::make_unique<interaction::Pinch>(this);
        interactions_.push_back(std::move(pinch));
    } else if(type == "pan") {
        std::unique_ptr<interaction::InteractionBase> pan = std::make_unique<interaction::Pan>(this);
        interactions_.push_back(std::move(pan));
    }
    this->interactionContext_->SetTypeConfig(type, config);
    return *this;
}

XChart &XChart::Coord(const std::string &json) {
    nlohmann::json config = xg::json::ParseString(json);
    this->logTracer_->trace("#coord ");
    if(config.is_object()) {
        this->coordCfg_.merge_patch(config);
    }
    bool _transposed = this->coordCfg_["transposed"];
    if(this->coord_ != nullptr) {
        this->coord_->SetTransposed(_transposed);
        // TODO repaint
    }
    return *this;
}

XChart &XChart::Animate(const std::string &json) {
    this->animateCfg_ = xg::json::ParseString(json);
    return *this;
}

XChart &XChart::Legend(const std::string &field, const std::string &json) {
    nlohmann::json config = xg::json::ParseString(json);
    this->logTracer_->trace("#Legend field: %s config: %s", field.c_str(), config.dump().c_str());
    this->legendController_->SetFieldConfig(field, config);
    return *this;
}

bool XChart::OnTouchEvent(const std::string &json) {
    nlohmann::json cfg = xg::json::ParseString(json);
    if(!cfg.is_object() || !cfg.contains("eventType") || !cfg.contains("points"))
        return false;
    event::Event event;
    event.eventType = cfg["eventType"];
    nlohmann::json &_points = cfg["points"];
    for(std::size_t i = 0; i < _points.size(); ++i) {
        nlohmann::json &_point = _points[i];
        util::Point point{_point["x"], _point["y"]};
        event.points.push_back(std::move(point));
    }
    event.devicePixelRatio = ratio_;
    event.timeStamp = xg::CurrentTimestampAtMM();
    this->logTracer_->trace("#onTouchEvent: %s", json.data());
    return this->eventController_->OnTouchEvent(event);
}

XChart &XChart::Scale(const std::string &field, const std::string &json) {
    nlohmann::json config = xg::json::ParseString(json);
    this->logTracer_->trace("#Scale field: %s config: %s", field.c_str(), config.dump().c_str());
    this->scaleController_->UpdateColConfig(field, config);
    return *this;
}

scale::AbstractScale &XChart::GetScale(const std::string &field) {
    return *this->scaleController_->CreateScale(field, this->data_, this->logTracer_, this->coord_).get();
}

geom::Line &XChart::Line() {
    this->logTracer_->trace("#Line %s", "create Geom@Line");
    auto line = std::make_unique<geom::Line>(midLayout_->AddGroup(), logTracer_);
    geom::Line *p = line.get();
    this->geomShapeFactory_->RegisterGeomShape(line->GetType(), xg::make_unique<xg::geom::shape::Line>());
    this->geoms_.push_back(std::move(line));
    return *p;
}

geom::Interval &XChart::Interval() {
    this->logTracer_->trace("#Interval %s", "create Geom@Interval");
    auto interval = std::make_unique<geom::Interval>(midLayout_->AddGroup(), logTracer_);
    geom::Interval *p = interval.get();
    this->geomShapeFactory_->RegisterGeomShape(interval->GetType(), xg::make_unique<xg::geom::shape::Interval>());
    this->geoms_.push_back(std::move(interval));
    return *p;
}

geom::Area &XChart::Area() {
    this->logTracer_->trace("#Area %s", "create Geom@Area");
    auto area = std::make_unique<geom::Area>(midLayout_->AddGroup(), logTracer_);
    geom::Area *p = area.get();
    this->geomShapeFactory_->RegisterGeomShape(area->GetType(), xg::make_unique<xg::geom::shape::Area>());
    this->geoms_.push_back(std::move(area));
    return *p;
}

geom::Point &XChart::Point() {
    this->logTracer_->trace("#Point %s", "create Geom@Point");
    auto point = std::make_unique<geom::Point>(midLayout_->AddGroup(), logTracer_);
    geom::Point *p = point.get();
    this->geomShapeFactory_->RegisterGeomShape(point->GetType(), xg::make_unique<xg::geom::shape::Point>());
    this->geoms_.push_back(std::move(point));
    return *p;
}

geom::Candle &XChart::Candle() {
    this->logTracer_->trace("#Candle %s", "create Geom@Candle");
    auto candle = std::make_unique<geom::Candle>(midLayout_->AddGroup(), logTracer_);
    geom::Candle *p = candle.get();
    this->geomShapeFactory_->RegisterGeomShape(candle->GetType(), xg::make_unique<xg::geom::shape::Candle>());
    this->geoms_.push_back(std::move(candle));
    return *p;
}

std::vector<std::string> XChart::getYScaleFields() {
    std::vector<std::string> _fields;

    std::for_each(geoms_.begin(), geoms_.end(), [&](auto &geom) -> void {
        std::string yField = geom->GetYScaleField();

        auto it = std::find(_fields.begin(), _fields.end(), yField);
        if(it == _fields.end()) {
            _fields.push_back(yField);
        }
    });
    return _fields;
}

std::string XChart::GetScaleTicks(const std::string &field) noexcept {
    nlohmann::json rst = {};
    scale::AbstractScale &scale = this->GetScale(field);
    std::vector<scale::Tick> ticks = scale.GetTicks();

    for(std::size_t i = 0; i < ticks.size(); ++i) {
        scale::Tick &tick = ticks[i];
        nlohmann::json item = {{"text", tick.text}, {"tickValue", tick.tickValue}, {"value", tick.value}};
        rst.push_back(item);
    }
    return rst.dump();
}

void XChart::Render() {
    this->logTracer_->trace("#Render %s", "start render");
    if(canvasContext_ == nullptr) {
        this->logTracer_->trace("error: %s", "canvasContext is nullptr, render end.");
        return;
    }

    if(!canvasContext_->IsValid()) {
        this->logTracer_->trace("error: %s", "canvasContext is not valid, render end.");
        return;
    }

    if(!this->data_.is_array() || this->data_.size() == 0) {
        this->logTracer_->trace("error: %s", "data is not array or size is zero, render end.");
        return;
    }

    auto startTimeStamp = xg::CurrentTimestampAtMM();

    if(!rendered_) {
        // 1. init Coord
        this->logTracer_->trace("%s padding: [%lf, %lf, %lf, %lf]", "call InitCoord", padding_[0], padding_[1], padding_[2], padding_[3]);
        this->InitCoord();
        // 2. init Geoms

        this->logTracer_->trace("%s", "foreach geom init");
        std::for_each(geoms_.begin(), geoms_.end(), [this](auto &geom) -> void { geom->Init(this); });
        rendered_ = true;

        this->NotifyAction(ACTION_CHART_AFTER_INIT);
    }
    // 3. legend render
    this->logTracer_->trace("%s", "legendController render");
    legendController_->Render(*this);

    // 4. render Axis
    this->logTracer_->trace("%s", "draw axies");
    axisController_->DrawAxes(this, *canvasContext_);

    // 5. geom paint
    this->logTracer_->trace("%s", "foreach geom paint");

    std::for_each(geoms_.begin(), geoms_.end(), [this](auto &geom) { geom->Paint(this); });
    // 6. guide render
    this->NotifyAction(ACTION_CHART_AFTER_GEOM_DRAW);

    // 6.1 guide render
    this->logTracer_->trace("%s", "guideController render");
    guideController_->Render(*this, *canvasContext_);

    // 7. children sort
    this->logTracer_->trace("%s", "canvas#sort");
    this->canvas_->Sort();

    // 图元元素限定在屏幕区域内绘制。当绘制元素很多时(K线)，这个操作耗时太大，先不执行这个操作。
    //     midLayout_->clip_ = std::make_unique<shape::Rect>(util::Point{coord_->GetXAxis().x, coord_->GetYAxis().y},
    //                                                       util::Size{coord_->GetWidth(), coord_->GetHeight()});
    //     util::Matrix  matrix = {0.029800, 0.000000, 0.000000, 1.000000, 104.251022, 0.000000};
    //     midLayout_->clip_->SetMatrix(matrix);

    this->canvas_->ChangeSize(margin_[0], margin_[1], width_, height_);

    // 6. canvas draw
    this->logTracer_->trace("%s", "canvas#startDraw");
    this->canvasContext_->Reset();
    this->NotifyAction(ACTION_CHART_BEFORE_CANVAS_DRAW);
    this->canvas_->Draw();
    this->NotifyAction(ACTION_CHART_AFTER_RENDER);

    renderDurationMM_ = xg::CurrentTimestampAtMM() - startTimeStamp;

    long count = this->canvasContext_->GetRenderCount();
    this->logTracer_->trace("%s renderCount: %ld, duration: %lums", "canvas#endDraw", count, renderDurationMM_);
}

void XChart::Repaint() {
    this->logTracer_->trace("%s", "chart repaint");
    if(!rendered_)
        return;

    // TODO legend clear
    ClearInner();
    Render();
}

void XChart::Clear() {
    guideController_->Clear();
    scaleController_->Clear();
    ClearInner();
    this->geoms_.clear();
    this->geomShapeFactory_->Clear();
    renderActionListeners_.clear();
    interactions_.clear();
    this->data_ = {};
    this->rendered_ = false;
    requestFrameHandleId_ = "";
}

std::size_t XChart::RequestAnimationFrame(func::Command *c, long delay) {
    GetLogTracer()->trace("#RequestAnimationFrame handleID: %s", requestFrameHandleId_.data());
    if(this->requestFrameHandleId_.empty()) {
        delete c;
        return 0;
    }
    func::F2Function *method = func::FunctionManager::GetInstance().Find(requestFrameHandleId_);
    GetLogTracer()->trace("#RequestAnimationFrame method: %p", method);
    if(method != nullptr) {
        long p = reinterpret_cast<long>(c);
        method->Execute({{"command", p}, {"delay", delay}});
        return p;
    } else {
        delete c;
        return 0;
    }
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

XChart &XChart::Tooltip(const std::string &json) {
    nlohmann::json cfg = xg::json::ParseString(json);
    if(this->tooltipController_ == nullptr) {
        this->tooltipController_ = new tooltip::ToolTipController(this);

        this->tooltipController_->AddMonitor(
            XG_MEMBER_OWNER_CALLBACK_1(legend::LegendController::OnToolTipMarkerItemsChanged, this->legendController_));
    }

    this->tooltipController_->Init(cfg);
    return *this;
}

void XChart::InitLayout() {
    backLayout_ = canvas_->AddGroup();
    backLayout_->SetZIndex(0);

    midLayout_ = canvas_->AddGroup();
    midLayout_->SetZIndex(10);

    frontLayout_ = canvas_->AddGroup();
    frontLayout_->SetZIndex(20);
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
    if(scaleController_->empty() || !item.is_object() || item.size() < 2) {
        return util::Point{0, 0};
    }

    std::string xField = this->GetXScaleField();
    std::string yField = this->getYScaleFields()[0];

    if(!item.contains(xField) || !item.contains(yField)) {
        return util::Point{0, 0};
    }

    double x = this->GetScale(xField).Scale(item[xField]);
    double y = this->GetScale(yField).Scale(item[yField]);
    util::Point ret = this->GetCoord().ConvertPoint(util::Point{x, y});
    return ret;
}

void XChart::ClearInner() {

    std::for_each(geoms_.begin(), geoms_.end(), [this](auto &geom) { geom->Clear(); });

    this->legendController_->ClearInner();

    this->padding_ = userPadding_;

    if(this->coord_ != nullptr) {
        util::Point start = util::Point{this->margin_[0] + this->padding_[0], this->margin_[1] + this->height_ - this->padding_[3]};
        util::Point end = util::Point{this->margin_[0] + this->width_ - this->padding_[2], this->margin_[1] + this->padding_[1]};

        this->coord_->Reset(start, end);
    }

    NotifyAction(ACTION_CHART_CLEAR_INNER);

    axisController_->Clear();
    if(canvasContext_ != nullptr) {
        canvasContext_->Reset();
    }
}

void XChart::Redraw() {
    auto startTimeStamp = xg::CurrentTimestampAtMM();

    // 7.-1 beforeDraw
    //    this->logTracer_->trace("%s", "before Redraw and call canvas clearRect");
    //    canvasContext_->ClearRect(margin_[0], margin_[1], width_, height_);

    this->legendController_->Redraw(*this);

    // 6. canvas draw
    this->logTracer_->trace("%s", "canvas#start Redraw");
    this->canvasContext_->Reset();
    this->canvas_->Draw();

    renderDurationMM_ = xg::CurrentTimestampAtMM() - startTimeStamp;

    long count = this->canvasContext_->GetRenderCount();
    this->logTracer_->trace("%s renderCount: %ld, duration: %lums", "canvas#endRedraw", count, renderDurationMM_);
}

std::map<std::string, std::vector<legend::LegendItem>> XChart::GetLegendItems() {

    std::map<std::string, std::vector<legend::LegendItem>> legendItems;

    std::for_each(geoms_.begin(), geoms_.end(), [&](auto &geom) -> void {
        if(geom->ContainsAttr(attr::AttrType::Color)) {
            const unique_ptr<attr::AttrBase> &_attr = geom->GetColor();
            if(!_attr->GetFields().empty()) {
                const attr::Color &colorAttr = static_cast<const attr::Color &>(*_attr.get());

                scale::AbstractScale &scale = GetScale(colorAttr.GetFields()[0]);

                if(scale::IsCategory(scale.GetType())) {
                    const scale::Category &cat = static_cast<Category &>(scale);
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
