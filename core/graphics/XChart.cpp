#include "XChart.h"
#include "animate/TimeLine.h"
#include "canvas/Cartesian.h"
#include "canvas/Polar.h"
#include "func/Func.h"
#include "geom/shape/Area.h"
#include "geom/shape/Candle.h"
#include "geom/shape/Interval.h"
#include "geom/shape/Line.h"
#include "geom/shape/Point.h"
#include "util/Point.h"
#include "util/json.h"
#include "../utils/common.h"
#include "../utils/xtime.h"

using namespace xg;
using namespace std;

XChart::XChart(const std::string &name, double width, double height, double ratio, bool showLog) : chartName_(name) {
    SetName(name);
    width_ = width * ratio;
    height_ = height * ratio;
    ratio_ = ratio;
    this->logTracer_ = new utils::Tracer(name + "@Chart", showLog);
    this->logTracer_->trace("create xchart: %s %lf %lf %lf ", chartId_.c_str(), width_, height_, ratio_);

    geomShapeFactory_ = xg::make_unique<geom::shape::GeomShapeFactory>();

    // 初始化度量控制器
    scaleController_ = new scale::ScaleController(this);
    this->logTracer_->trace("%s", "new ScaleController instance.");
    // 画布
    this->canvas_ = new canvas::Canvas();
    this->logTracer_->trace("%s", "new canvas instance.");
    // 布局层级
    this->InitLayout();

    axisController_ = new axis::AxisController(backLayout_->AddGroup());
    this->logTracer_->trace("%s", "new axisController instance.");
    guideController_ = new guide::GuideController(frontLayout_->AddGroup(), backLayout_->AddGroup(), GetLogTracer());
    this->logTracer_->trace("%s", "new guideController instance.");
    legendController_ = new legend::LegendController(backLayout_->AddGroup());
    this->logTracer_->trace("%s", "new legendController instance");

    eventController_ = new event::EventController();
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
    if (ownCanvasContext_) {
        XG_RELEASE_POINTER(canvasContext_);
    }
    XG_RELEASE_POINTER(interactionContext_);
    XG_RELEASE_POINTER(eventController_);
    XG_RELEASE_POINTER(logTracer_);

    backLayout_ = nullptr;
    midLayout_ = nullptr;
    frontLayout_ = nullptr;
    XG_RELEASE_POINTER(canvas_);
}

bool XChart::Parse(const std::string &dsl) {
    this->logTracer_->trace("#dsl dataSize: %lu", dsl.size());
    nlohmann::json _dsl = xg::json::ParseString(dsl);
    if (!_dsl.is_object()) {
        this->logTracer_->trace("#dsl json is invalid");
        return false;
    }
    return ParseObject(_dsl);
}
    
bool XChart::ParseObject(const nlohmann::json &dsl) {
    //线上已经使用了data作为key，这里兼容下
    const std::string sourceKey = dsl.contains("data") ? "data" :"source";
    if (!dsl.contains(sourceKey)) {
        this->logTracer_->trace("#Parse source key is not include");
        return false;
    }
    if (dsl[sourceKey].is_array()) {
        const auto &data = json::GetArray(dsl, sourceKey);
//        SourceObject(data);
    }
    //线上的老代码data部分是string
    else if(dsl[sourceKey].is_string()) {
        const auto &dataStr = json::GetString(dsl, sourceKey);
        const auto data = json::ParseString(dataStr);
//        SourceObject(data);
    }
    
    if (data_.size() == 0) {
        this->logTracer_->trace("#Parse data length is 0");
        return false;
    }

    const auto &name = json::GetString(dsl, "name");
    SetName(name);
    
    const auto &padding = json::GetArray(dsl, "padding");
    const auto &margin = json::GetArray(dsl, "margin");
    if (padding.is_array() && padding.size() >= 4) {
        Padding(padding[0], padding[1], padding[2], padding[3]);
    }
    
    if (margin.is_array() && margin.size() >= 4) {
        Margin(margin[0], margin[1], margin[2], margin[3]);
    }
    
    const auto &coord = json::GetObject(dsl, "coord");
//    CoordObject(coord);
    
    //一个chart只有一个legend
    const auto &legend = json::GetObject(dsl, "legend");
//    LegendObject(json::GetString(legend, "field"), legend);
    
    //可能是bool， 可能是object
    const auto &animate = json::Get(dsl, "animate");
//    AnimateObject(animate);
    
    const auto &axises = json::GetArray(dsl, "axises");
    for (auto it = axises.begin(); it != axises.end(); ++it) {
        const auto &config = json::GetObject(*it, "config");
        
        //兼容线上老的代码
        if (config.is_object()) {
//            AxisObject(json::GetString(*it, "field"), config);
        } else {
//            AxisObject(json::GetString(*it, "field"), *it);
        }
    }
    
    const auto &tooltip = json::GetObject(dsl, "tooltip");
//    TooltipObject(tooltip);
    
    const auto &scales = json::GetArray(dsl, "scales");
    for (auto it = scales.begin(); it != scales.end(); ++it) {
        //todo config是否要去掉
        const auto &config = json::GetObject(*it, "config");
        if (config.is_object()) {
//            ScaleObject(json::GetString(*it, "field"), config);
        } else {
//            ScaleObject(json::GetString(*it, "field"), *it);
        }
    }
    
    const auto &geoms = json::GetArray(dsl, "geoms");
    if (geoms.size() == 0) {
        this->logTracer_->trace("#Parse geoms size is invalid");
        return false;
    }
    for (auto it = geoms.begin(); it != geoms.end(); ++it) {
        const auto &type = json::GetString(*it, "type");
        const auto &position = json::GetString(*it, "position");
        
        if (type.empty() || position.empty()) {
            continue;
        }
        
        geom::AbstractGeom *geom = nullptr;
        if (type == "line") {
            geom = &Line();
        } else if (type == "interval") {
            geom = &Interval();
        } else if (type == "area") {
            geom = &Area();
        } else if (type == "point") {
            geom = &Point();
        } else if (type == "candle") {
            geom = &Candle();
        }
        
        if (!geom) {
            continue;
        }
        
        geom->Position(position);
        
        const auto &color = json::GetObject(*it , "color");
        if (color.is_object()) {
            geom->Color(json::GetString(color, "field"), json::GetArray(color, "attrs"));
        }

        const auto &fixedColor = json::GetString(*it , "fixedColor");
        if (!fixedColor.empty()) {
            geom->Color(fixedColor);
        }

        const auto &size = json::GetObject(*it , "size");
        if (size.is_object()) {
            geom->Size(json::GetString(size, "field"), json::GetArray(size, "attrs"));
        }

        const auto fixedSize = json::GetNumber(*it , "fixedSize", NAN);
        if (!std::isnan(fixedSize)) {
            geom->Size(fixedSize);
        }

        const auto &shape = json::GetObject(*it , "shape");
        if (shape.is_object()) {
            geom->Shape(json::GetString(shape, "field"), json::GetArray(shape, "attrs"));
        }

        const auto &fixedShape = json::GetString(*it , "fixedShape");
        if (!fixedShape.empty()) {
            geom->Shape(fixedShape);
        }

        const auto &adjust = json::GetString(*it, "adjust");
        if (!adjust.empty()) {
            geom->Adjust(adjust);
        }

        const auto &style = json::GetObject(*it, "style");
        if (style.is_object()) {
            geom->StyleObject(style);
        }
    }
    
    const auto &guides = json::GetArray(dsl, "guides");
    for (auto it = guides.begin(); it != guides.end(); ++it) {
        const auto &type = json::GetString(*it, "type");
        if (type.empty()) {
            continue;
        }
        
        if (type == "line") {
//            Guide().LineObject(*it);
        } else if (type == "text") {
//            Guide().TextObject(*it);
        } else if (type == "flag") {
//            Guide().FlagObject(*it);
        } else if (type == "background") {
//            Guide().BackgroundObject(*it);
        } else if (type == "image") {
//            Guide().ImageObject(*it);
        }
    }
    
    const auto &interactions = json::GetArray(dsl, "interactions");
    for (auto it = interactions.begin(); it != interactions.end(); ++it) {
        auto &type = json::GetString(*it, "type");
        if (type == "pinch") {
//            Interaction(json::GetString(*it, "type"), (interaction::PinchCfg)*it);
        } else if (type == "pan") {
//            Interaction(json::GetString(*it, "type"), (interaction::PanCfg)*it);
        }
        
    }
    return true;
}

XChart &XChart::Source(const std::vector<XSourceItem> &data) {
    this->data_ = data;
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
    return AxisObject(field, xg::json::ParseString(json));
}

XChart &XChart::Interaction(const std::string &type, const std::string &json) {
    if (type == "pinch") {
        return Interaction(type, (interaction::PinchCfg)xg::json::ParseString(json));
    } else if(type == "pan"){
        return Interaction(type, (interaction::PanCfg)xg::json::ParseString(json));
    }
    
    return * this;
}

XChart &XChart::Animate(const std::string &json) {
    return AnimateObject(xg::json::ParseString(json));
}

XChart &XChart::Legend(const std::string &field, const std::string &json) {
    return LegendObject(field, xg::json::ParseString(json));
}

bool XChart::OnTouchEvent(event::Event &event) {
    if(!canvasContext_ || !canvasContext_->IsValid()) {
        return false;
    }
    event.devicePixelRatio = ratio_;
    event.timeStamp = xg::CurrentTimestampAtMM();
    return this->eventController_->OnTouchEvent(event);
}

XChart &XChart::Scale(const std::string &field, const std::string &json) {
    return ScaleObject(field, xg::json::ParseString(json));
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
    if (!canvasContext_ || !canvasContext_->IsValid()) {
        return rst.dump();
    }

    scale::AbstractScale &scale = this->GetScale(field);
    std::vector<scale::Tick> ticks = scale.GetTicks(this);

    for(std::size_t i = 0; i < ticks.size(); ++i) {
        scale::Tick &tick = ticks[i];
        nlohmann::json item = {{"text", tick.text}, {"tickValue", tick.tickValue}, {"value", tick.value}};
        rst.push_back(item);
    }
    return rst.dump();
}

bool XChart::Render() {
    this->logTracer_->trace("#Render %s", "start render");
    if(canvasContext_ == nullptr) {
        this->logTracer_->trace("error: %s", "canvasContext is nullptr, render end.");
        return false;
    }

    if(!canvasContext_->IsValid()) {
        this->logTracer_->trace("error: %s", "canvasContext is not valid, render end.");
        return false;
    }

    if(this->data_.size() == 0) {
        this->logTracer_->trace("error: %s", "data is not array or size is zero, render end.");
        return false;
    }

    auto startTimeStamp = xg::CurrentTimestampAtMM();

    if(!rendered_) {
        // 1. init Coord
        this->logTracer_->trace("%s padding: [%lf, %lf, %lf, %lf]", "call InitCoord", padding_[0], padding_[1], padding_[2], padding_[3]);
        this->InitCoord();
        logTracer_->trace("XChart#Performance InitCoord %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));
        // 2. init Geoms

        this->logTracer_->trace("%s", "foreach geom init");
        std::for_each(geoms_.begin(), geoms_.end(), [this](auto &geom) -> void { geom->Init(this); });
        logTracer_->trace("XChart#Performance GeomInit %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));
        
        //调整interval中的min和max值
        if (config_.adjustScale) {
            scaleController_->AdjustScale();
            logTracer_->trace("XChart#Performance AdjustScale %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));
        }

        //调整interval中的min和max值
        if (config_.syncY) {
            scaleController_->SyncYScale();
            logTracer_->trace("XChart#Performance SyncYScale %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));
        }
        
        //调整interval中的range的值
        if (config_.adjustRange) {
            scaleController_->AdjustRange();
            logTracer_->trace("XChart#Performance AdjustRange %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));
        }
        
        rendered_ = true;

        this->NotifyAction(ACTION_CHART_AFTER_INIT);
        logTracer_->trace("XChart#Performance NotifyAction ACTION_CHART_AFTER_INIT %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));
    }
    // 3. legend render
    this->logTracer_->trace("%s", "legendController render");
    auto legendTimeStamp = xg::CurrentTimestampAtMM();
    legendController_->Render(*this);
    logTracer_->trace("XChart#Performance LegendRender %lums", (xg::CurrentTimestampAtMM() - legendTimeStamp));

    // 4. render Axis
    this->logTracer_->trace("%s", "draw axies");
    auto axisTimeStamp = xg::CurrentTimestampAtMM();
    axisController_->DrawAxes(this, *canvasContext_);
    logTracer_->trace("XChart#Performance DrawAxes %lums", (xg::CurrentTimestampAtMM() - axisTimeStamp));

    // 5. geom paint
    this->logTracer_->trace("%s", "foreach geom paint");

    std::for_each(geoms_.begin(), geoms_.end(), [this](auto &geom) { geom->Paint(this); });
    logTracer_->trace("XChart#Performance GeomPaint %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));
    // 6. guide render
    this->NotifyAction(ACTION_CHART_AFTER_GEOM_DRAW);

    // 6.1 guide render
    this->logTracer_->trace("%s", "guideController render");
    guideController_->Render(*this, *canvasContext_);
    logTracer_->trace("XChart#Performance GuideRender %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));

    // 7. children sort
    this->logTracer_->trace("%s", "canvas#sort");
    this->canvas_->Sort();
    logTracer_->trace("XChart#Performance CanvasSort %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));

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
    this->canvas_->Draw(GetCanvasContext());
    this->NotifyAction(ACTION_CHART_AFTER_RENDER);
    logTracer_->trace("XChart#Performance CanvasDraw %lums", (xg::CurrentTimestampAtMM() - startTimeStamp));
    renderDurationMM_ = xg::CurrentTimestampAtMM() - startTimeStamp;

    long count = this->canvasContext_->GetRenderCount();
    this->logTracer_->trace("%s %s renderCount: %ld, duration: %lums", chartId_.c_str(), "canvas#endDraw", count, renderDurationMM_);
    return true;
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
    if(tooltipController_) {
        tooltipController_->Clear();
    }
    
    if (interactionContext_) {
        interactionContext_->Clear();
    }
    
    ClearInner();
    this->geoms_.clear();
    this->geomShapeFactory_->Clear();
    renderActionListeners_.clear();
    interactions_.clear();
    this->data_ = {};
    this->rendered_ = false;
    requestFrameHandleId_ = "";
    this->geomAnimate_->Clear();
}

std::size_t XChart::RequestAnimationFrame(func::Command *c, long delay) {
    if(!canvasContext_ || !canvasContext_->IsValid() || this->requestFrameHandleId_.empty()) {
        delete c;
        return 0;
    }
    GetLogTracer()->trace("#RequestAnimationFrame handleID: %s", requestFrameHandleId_.data());
    if(!requestFrameHandleId_.empty()) {
        long p = reinterpret_cast<long>(c);
        nlohmann::json data = {{"command", p}, {"delay", delay}};
        InvokeFunction(requestFrameHandleId_, data.dump());
        return p;
    } else {
        delete c;
        return 0;
    }
}

XChart &XChart::Tooltip(const std::string &json) {
    return TooltipObject(xg::json::ParseString(json));
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
    bool _transposed = coordConfig_.transposed;
    const std::string &type = coordConfig_.type;
    util::Point start = util::Point{this->margin_[0] + this->padding_[0], this->margin_[1] + this->height_ - this->padding_[3]};
    util::Point end = util::Point{this->margin_[0] + this->width_ - this->padding_[2], this->margin_[1] + this->padding_[1]};
    if(type == "polar") {
        this->coord_ = xg::make_unique<canvas::coord::Polar>(start, end, _transposed);
    } else {
        this->coord_ = xg::make_unique<canvas::coord::Cartesian>(start, end, _transposed);
    }
}

const util::Point XChart::GetPosition(const XSourceItem &item) {
    if(!canvasContext_ || !canvasContext_->IsValid() || scaleController_->Empty()) {
        return util::Point{0, 0};
    }

    std::string xField = this->GetXScaleField();
    std::string yField = this->getYScaleFields()[0];

    if(!item.count(xField) || !item.count(yField)) {
        return util::Point{0, 0};
    }

    double x = this->GetScale(xField).Scale(item.find(xField)->second);
    double y = this->GetScale(yField).Scale(item.find(yField)->second);
    util::Point ret = this->GetCoord().ConvertPoint(util::Point{x, y});
    return ret;
}

void XChart::ClearInner() {

    std::for_each(geoms_.begin(), geoms_.end(), [this](auto &geom) { geom->Clear(); });

    this->legendController_->ClearInner();
    this->guideController_->ClearInner();

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
    if(!canvasContext_->IsValid()) {
        return;
    }
    auto startTimeStamp = xg::CurrentTimestampAtMM();

    // 7.-1 beforeDraw
    //    this->logTracer_->trace("%s", "before Redraw and call canvas clearRect");
    //    canvasContext_->ClearRect(margin_[0], margin_[1], width_, height_);

    this->legendController_->Redraw(*this);

    // 6. canvas draw
    this->logTracer_->trace("%s", "canvas#start Redraw");
    this->canvasContext_->Reset();
    this->canvas_->Draw(GetCanvasContext());

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
                    std::vector<scale::Tick> ticks = scale.GetTicks(this);

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

    
#pragma mark -
XChart &XChart::ScaleObject(const std::string &field, const ScaleCfg &config) {
    this->logTracer_->trace("#Scale field: %s config: %s", field.c_str(), config.type.c_str());
    this->scaleController_->UpdateColConfig(field, config);
    return *this;
}

XChart &XChart::AxisObject(const std::string &field, const axis::AxisCfg &config) {
    this->logTracer_->trace("#Axis field: %s config: %b", field.c_str(), config.hidden);
    this->axisController_->SetFieldConfig(field, config);
    return *this;
}

XChart &XChart::LegendObject(const std::string &field, const legend::LegendCfg &config) {
    this->logTracer_->trace("#Legend field: %s config: %s", field.c_str(), config.position.c_str());
    this->legendController_->SetFieldConfig(field, config);
    return *this;
}

XChart &XChart::Interaction(const std::string &type, const interaction::PinchCfg &config) {
    if (!this->interactionContext_) {
        interactionContext_ = new interaction::InteractionContext(this);
    }
    std::unique_ptr<interaction::InteractionBase> pinch = std::make_unique<interaction::Pinch>(this);
    interactions_.push_back(std::move(pinch));
    this->interactionContext_->SetTypeConfig(type, config);
    return *this;
}

XChart &XChart::Interaction(const std::string &type, const interaction::PanCfg &config) {
    if (!this->interactionContext_) {
        interactionContext_ = new interaction::InteractionContext(this);
    }
    std::unique_ptr<interaction::InteractionBase> pan = std::make_unique<interaction::Pan>(this);
    interactions_.push_back(std::move(pan));
    this->interactionContext_->SetTypeConfig(type, config);
    return *this;
}

XChart &XChart::TooltipObject(const tooltip::ToolTipCfg &config) {
    if(this->tooltipController_ == nullptr) {
        this->tooltipController_ = new tooltip::ToolTipController(this);

        this->tooltipController_->AddMonitor(
            XG_MEMBER_OWNER_CALLBACK(legend::LegendController::OnToolTipMarkerItemsChanged, this->legendController_));
    }

    this->tooltipController_->Init(config);
    return *this;
}

XChart &XChart::CoordObject(const CoordCfg &config) {
    this->logTracer_->trace("#coord ");
    coordConfig_ = config;
    if(this->coord_ != nullptr) {
        this->coord_->SetTransposed(coordConfig_.transposed);
    }
    return *this;
}

XChart &XChart::AnimateObject(const animate::AnimateCfg &config) {
    geomAnimate_->SetAnimateConfig(config);
    return *this;
}

void XChart::ChangeSize(double width, double height) {
    width_ = width * ratio_;
    height_ = height * ratio_;
    canvasContext_->ChangeSize(width, height);
    ClearInner();
}

void XChart::ChangeData(const std::vector<XSourceItem> &data) {
    Source(data);
    
    //清理度量
    scaleController_->Clear();
    
    //清理几何标记 里面会关联数据
    std::for_each(geoms_.begin(), geoms_.end(), [this](auto &geom) -> void { geom->ClearInner(); });
    
    //清除画布
    ClearInner();
    
    //标记重新渲染
    rendered_ = false;
}
