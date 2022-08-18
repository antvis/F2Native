#ifndef XG_GRAPHICS_XCHART_H
#define XG_GRAPHICS_XCHART_H

#if defined(__APPLE__)
#include "../ios/CoreGraphicsContext.h"
#include <TargetConditionals.h>
#endif

#if defined(ANDROID)
#include "android/BitmapCanvasContext.h"
#endif

#if defined(__EMSCRIPTEN__)
#include "webassembly/WebCanvasContext.h"
#endif

#if defined(WASI)
#include "wasi/WasiCanvasContext.h"
#endif

#include <cstring>
#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "animate/GeomAnimate.h"
#include "animate/TimeLine.h"
#include "axis/AxisController.h"
#include "canvas/Canvas.h"
#include "canvas/CanvasContext.h"
#include "canvas/Coord.h"
#include "event/EventController.h"
#include "func/Command.h"
#include "geom/Area.h"
#include "geom/Candle.h"
#include "geom/Interval.h"
#include "geom/Line.h"
#include "geom/Point.h"
#include "geom/shape/GeomShapeFactory.h"
#include "guide/GuideController.h"
#include "interaction/InteractionBase.h"
#include "interaction/InteractionContext.h"
#include "interaction/Pan.h"
#include "interaction/Pinch.h"
#include "legend/LegendController.h"
#include "scale/ScaleController.h"
#include "tooltip/TooltipController.h"
#include "util/json_data.h"
#include "../utils/Tracer.h"


#define XG_MEMBER_CALLBACK(funPtr) std::bind(&funPtr, this)
#define XG_MEMBER_CALLBACK_1(funPtr) std::bind(&funPtr, this, std::placeholders::_1)
#define XG_MEMBER_OWNER_CALLBACK(funPtr, owner) std::bind(&funPtr, owner)
#define XG_MEMBER_OWNER_CALLBACK_1(funPtr, owner) std::bind(&funPtr, owner, std::placeholders::_1)

#define XG_RELEASE_POINTER(ptr)                                                                                                \
    if(ptr != nullptr)                                                                                                         \
        delete ptr;                                                                                                            \
    ptr = nullptr;

#define ACTION_CHART_AFTER_INIT "ChartAfterInit"
#define ACTION_CHART_AFTER_GEOM_DRAW "ChartAfterGeomDraw"
#define ACTION_CHART_AFTER_RENDER "ChartAfterRender"
#define ACTION_CHART_CLEAR_INNER "ChartClearInner"
#define ACTION_CHART_BEFORE_CANVAS_DRAW "ChartBeforeCanvasDraw"
#define ACTION_CHART_AFTER_CANVAS_DESTROY "ChartAfterCanvasDestroy"

using namespace std;

namespace xg {

typedef std::function<void()> ChartActionCallback;

struct ChartConfig final {
    //当geom中有interval的时候自动调整max, min两个参数，使其中一个为0
    bool adjustScale = true;
    
    //当geom中有多个图形时，是否同步他们的最值
    //默认为false 因为shape为stack的时候不能设置sync
    bool syncY = false;
    
    //当geom中有interval的时候自动调整range参数， 使range = {0.1, 0.9}，让柱图在坐标系内
    bool adjustRange = true;
};

struct GrammerCfg final {
    string name;
    vector<float> padding, margin;
    XSourceArray source;
    coord::CoordCfg coord;
    legend::LegendCfg legend;
    tooltip::ToolTipCfg tooltip;
    animate::AnimateCfg animate;
    vector<axis::AxisCfg> axises;
    vector<geom::GeomCfg> geoms;
    vector<scale::ScaleCfg> scales;
    vector<guide::GuideCfg> guides;
    interaction::PanCfg pan;
    interaction::PinchCfg pinch;
};

class XChart {
    friend axis::AxisController;
    friend geom::AbstractGeom;
    friend geom::Line;
    friend geom::Interval;
    friend geom::Area;
    friend geom::Point;
    friend geom::Candle;
    friend legend::LegendController;
    friend event::EventController;
    friend tooltip::ToolTipController;
    friend interaction::Pan;
    friend interaction::Pinch;
    friend interaction::InteractionContext;
    friend animate::TimeLine;
    friend animate::GeomAnimate;

  public:
    XChart(const std::string &name, double width, double height, double ratio = 1.0, bool showLog = true);
    virtual ~XChart();

    // emscript 编译需要
    XChart(const XChart &){};

    //设置源数据
    XChart &Source(const std::vector<XSourceItem> &data);

    //context设置
#if defined(ANDROID)
    ///方便降级 稳定后改回SetCanvasContext
    XChart &SetCanvasContext(void *context) {
        XG_RELEASE_POINTER(canvasContext_);
        canvasContext_ = new canvas::BitmapCanvasContext((jobject)context, static_cast<float>(ratio_));
        return *this;
    }
#endif

#if defined(__APPLE__)
    ///方便降级 稳定后改回SetCanvasContext
    /// context == CGContextRef
    XChart &SetCanvasContext(void *context) {
        XG_RELEASE_POINTER(canvasContext_);
        canvasContext_ = new canvas::CoreGraphicsContext(context, width_, height_, static_cast<float>(ratio_), nullptr);
        return *this;
    }
#endif

#if defined(__EMSCRIPTEN__)
    XChart &SetCanvasContext(const std::string &canvasName) {
        XG_RELEASE_POINTER(canvasContext_);
        canvasContext_ = new canvas::WebCanvasContext(canvasName, width_, height_, ratio_);
        return *this;
    }
#endif // __EMSCRIPTEN__

#if defined(WASI)
    XChart &SetCanvasContext(const std::string &canvasName) {
        XG_RELEASE_POINTER(canvasContext_);
        canvasContext_ = new canvas::WasiCanvasContext(canvasName, width_, height_, ratio_);
        return *this;
    }
#endif // WASI
    
    /// 设置chart的canvasConext，这里和上面的方法增加了一个ownCanvasContext_来区分谁创建的canvasContext
    /// @param canvasContext 绘制的context
    XChart &SetCanvasContext(canvas::CanvasContext *canvasContext) {
        ownCanvasContext_ = false;
        canvasContext_ = canvasContext;
        return *this;
    }

    //padding&margin设置
    XChart &Padding(double left = 0.f, double top = 0.f, double right = 0.0, double bottom = 0.);
    XChart &Margin(double left = 0.f, double top = 0.f, double right = 0.0, double bottom = 0.);

    guide::GuideController &Guide() { return *this->guideController_; }
    
    //会修改event 附加devicePixelRatio和timestamp信息
    bool OnTouchEvent(event::Event &event);

    geom::Line &Line();
    geom::Interval &Interval();
    geom::Area &Area();
    geom::Point &Point();
    geom::Candle &Candle();

    ///渲染是否成功, 渲染失败的原因有
    ///1.未设置canvasContext
    ///2.设置了canvasContext， 但canvasContext->isValid 不合法
    ///3.传进来的source不是array或者数据长度为0
    ///@return true 渲染成功 false 失败
    bool Render();

    /// 内部会清理对象，然后再次调用render对象渲染
    void Repaint();
    
    /// 直接调用canvasContext进行绘制
    void Redraw();

    /// 清除所有的配置项
    void Clear();
    
    /// 改变chart的大小，会清理绘制的内容，但不会清理配置，所以可以在changeSize后直接调用render方法
    /// @param width chart的宽度
    /// @param height chart的高度
    void ChangeSize(double width, double height);
    
    /// 改变数据源
    /// @param data vector数组
    void ChangeData(const std::vector<XSourceItem> &data);
    
    ///设置当geom中有interval的时候，是否调整max, min,两个参数, 使其中一个为0
    ///@param adjust 是否调整，默认是调整的，可设置false 关闭
    inline void AdjustScale(bool adjust) { config_.adjustScale = adjust; }
    
    ///是否同步多个y轴的最值，默认为false
    ///@param sync 可设置false关闭
    inline void SyncYScale(bool sync) { config_.syncY = sync; }
    
    ///设置当geom中有interval的时候，是否调整range两个参数, 使其为range = {0.1, 0.9}
    ///@param range 是否调整，默认是调整的，可设置false 关闭
    inline void AdjustRange(bool range) { config_.adjustRange = range; }
    
    /// 注册业务测的回调函数，所有回调都会通过这个callback回调
    /// @param callback 回调函数
    inline void SetInvokeFunction(func::F2Function *callback) { invokeFunction_ = callback; }
    
    
    /// 调用callback
    /// @param functionId  回调函数的id
    /// @param param  参数，是一个json string
    inline const std::unordered_map<std::string, Any> InvokeFunction(const std::string &functionId, const std::string &param = "{}") {
        if(invokeFunction_) {
            return invokeFunction_->Execute(functionId, param);
        }else {
            return {};
        }
    }
    
    
    /// 设置动画的函数id
    /// @param funcId 回调函数的id
    inline void SetRequestFrameFuncId(const std::string &funcId) noexcept { this->requestFrameHandleId_ = funcId; }
    inline const std::string GetRequestFrameFuncId() noexcept { return this->requestFrameHandleId_; }
    
    /// 执行动画的回调函数
    /// @param c 动画的command
    std::size_t RequestAnimationFrame(func::Command *c, long delay = 16);

    scale::AbstractScale &GetScale(const std::string &field);

    /// 暴露共有属性
    inline vector<XSourceItem> &GetData() { return data_; }
    inline canvas::coord::AbstractCoord &GetCoord() { return *(coord_.get()); }
    inline void SetColDefs() {}

    inline std::string GetXScaleField() { return geoms_[0]->GetXScaleField(); }

    std::vector<std::string> getYScaleFields();

    inline const std::string &GetChartName() { return this->chartName_; }
    ///chart的唯一id chartName_+ uniqueId
    ///@return chartId_
    inline const std::string &GetChartId() { return this->chartId_; }

    inline utils::Tracer *GetLogTracer() const { return this->logTracer_; }

    inline const double GetWidth() const noexcept { return width_; }
    inline const double GetHeight() const noexcept { return height_; }

    std::vector<scale::Tick> GetScaleTicks(const std::string &field) noexcept;

    // 计算某一项数据对应的坐标位置
    const util::Point GetPosition(const XSourceItem &item);

    canvas::CanvasContext &GetCanvasContext() const { return *canvasContext_; }

    void AddMonitor(const std::string &action, ChartActionCallback callback, bool forChart = true) {
        if(forChart) {
            chartActionListeners_[action].push_back(callback);
        } else {
            renderActionListeners_[action].push_back(callback);
        }
    }

    inline const std::array<double, 4> GetPadding() { return userPadding_; }
    inline const std::array<double, 4> GetMargin() { return margin_; }

    /// bind long long to webassembly会报错，所以改成long,渲染时间用long表示也够用了
    inline long GetRenderDurationMM() const { return renderDurationMM_;}
    inline long GetRenderCmdCount() const { return canvasContext_ ? canvasContext_->GetRenderCount() : 0;}

    /// 返回所有的几何对象
    inline const std::vector<std::unique_ptr<geom::AbstractGeom>> &GetGeoms() const { return geoms_;}

    /*
     webassembly编译后如果不返回指针 不能使用chart.context().source()的连缀表达方式
     所以下面是额外的指针接口
     */
#if defined(__EMSCRIPTEN__)
    XChart *SetCanvasContextWasm(const std::string &canvasName) { return &SetCanvasContext(canvasName); }
    XChart *SourceWasm(const std::vector<XSourceItem> &json) { return &Source(json); }
    canvas::CanvasContext *GetCanvasContextWasm() const { return canvasContext_; }
    XChart *PaddingWasm(double left = 0.f, double top = 0.f, double right = 0.0, double bottom = 0.) {
        return &Padding(left, top, right, bottom);
    }
    XChart *MarginWasm(double left = 0.f, double top = 0.f, double right = 0.0, double bottom = 0.) {
        return &Margin(left, top, right, bottom);
    }
    XChart *ScaleWasm(const std::string &field, const ScaleCfg &json) { return &ScaleObject(field, json); }
    XChart *AxisWasm(const std::string &field, const axis::AxisCfg &json) { return &AxisObject(field, json); }
    XChart *LegendWasm(const std::string &field, const legend::LegendCfg &json) { return &LegendObject(field, json); }
    guide::GuideController *GuideWasm() { return &Guide(); }
    XChart *InteractionWasm(const std::string &type, const interaction::PanCfg &json) { return &Interaction(type, json); }
    XChart *TooltipWasm(const tooltip::ToolTipCfg &json) { return &TooltipObject(json); }
    XChart *CoordWasm(const CoordCfg &json) { return &CoordObject(json); }
    XChart *AnimateWasm(const animate::AnimateCfg &json) { return &AnimateObject(json); }
    geom::Line *LineWasm() { return &Line(); }
    geom::Interval *IntervalWasm() { return &Interval(); }
    geom::Area *AreaWasm() { return &Area(); }
    geom::Point *PointWasm() { return &Point(); }
    geom::Candle *CandleWasm() { return &Candle(); }
#endif
    
    /*
     为了能解析DSL，需要提供一些便捷方法
    */
//    bool Parse(const std::string &dsl);
    bool ParseObject(const GrammerCfg &dsl);
//    XChart &SourceObject(const nlohmann::json &data);
    XChart &ScaleObject(const std::string &field, const ScaleCfg &json);
    XChart &AxisObject(const std::string &field, const axis::AxisCfg &json);
    XChart &LegendObject(const std::string &field, const legend::LegendCfg &json);
    XChart &Interaction(const std::string &type, const interaction::PinchCfg &json);
    XChart &Interaction(const std::string &type, const interaction::PanCfg &json);
    XChart &TooltipObject(const tooltip::ToolTipCfg &json);
    XChart &CoordObject(const CoordCfg &json);
    XChart &AnimateObject(const animate::AnimateCfg &json);

  private:
    inline XChart &SetName(const std::string &name) {
        this->chartId_ = name + "_" + std::to_string(CreateChartId());
        return  *this;
    }
    
    static long long CreateChartId() {
        static std::atomic<long long> id(1);
        return id.fetch_add(+1);
    }

    // 初始化布局边界和三层视图容器
    void InitLayout();
    // 初始化轴管理器
    // void InitAixsController();

    // 初始化坐标系
    void InitCoord();
    // 初始化图元
    //    void InitGeoms();

    void UpdateLayout(std::array<double, 4> newPadding);

    std::map<std::string, std::vector<legend::LegendItem>> GetLegendItems();

    void ClearInner();

    void NotifyAction(const std::string &action) {
        auto &chartCallbacks = chartActionListeners_[action];
        std::for_each(chartCallbacks.begin(), chartCallbacks.end(), [&](ChartActionCallback &callback) -> void { callback(); });

        auto &renderCallbacks = renderActionListeners_[action];
        std::for_each(renderCallbacks.begin(), renderCallbacks.end(), [&](ChartActionCallback &callback) -> void { callback(); });
    }

  protected:
    
    bool rendered_ = false;
    std::vector<XSourceItem> data_;
    std::unique_ptr<canvas::coord::AbstractCoord> coord_ = nullptr;
    scale::ScaleController *scaleController_ = nullptr;
    canvas::Canvas *canvas_ = nullptr;
    axis::AxisController *axisController_ = nullptr;
    guide::GuideController *guideController_ = nullptr;
    event::EventController *eventController_ = nullptr;
    tooltip::ToolTipController *tooltipController_ = nullptr;
    interaction::InteractionContext *interactionContext_ = nullptr;
    legend::LegendController *legendController_ = nullptr;
    animate::GeomAnimate *geomAnimate_ = nullptr;
    std::array<double, 4> padding_ = {{0, 0, 0, 0}};
    std::array<double, 4> userPadding_ = {{0, 0, 0, 0}};
    std::array<double, 4> margin_ = {{0, 0, 0, 0}};

    // 析构时要析构对应的 Geom 对象
    std::vector<std::unique_ptr<geom::AbstractGeom>> geoms_;

    std::string chartName_;
    double width_;
    double height_;
    double ratio_ = 1.0f;
    long long renderDurationMM_ = 0;

    // 默认三层渲染
    shape::Group *backLayout_;
    shape::Group *midLayout_;
    shape::Group *frontLayout_;

    canvas::CanvasContext *canvasContext_ = nullptr;
    bool ownCanvasContext_ = true;

    utils::Tracer *logTracer_ = nullptr;
    std::unique_ptr<geom::shape::GeomShapeFactory> geomShapeFactory_ = nullptr;

    std::map<std::string, std::vector<ChartActionCallback>> renderActionListeners_{}; // 针对每次渲染的监听事件， clear 时会清除
    std::map<std::string, std::vector<ChartActionCallback>> chartActionListeners_{}; // 针对 chart 实例的监听事件，只有在 chart 回收时才清除
    std::vector<std::unique_ptr<xg::interaction::InteractionBase>> interactions_{};

    std::string chartId_;
    std::string requestFrameHandleId_ = "";
    func::F2Function *invokeFunction_ = nullptr;
    ChartConfig config_;
    CoordCfg coordConfig_;
};
} // namespace xg

#endif // XG_GRAPHICS_XCHART_H
