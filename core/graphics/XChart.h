
#include "graphics/animate/GeomAnimate.h"
#include "graphics/animate/TimeLine.h"
#include "graphics/axis/AxisController.h"
#include "graphics/canvas/Canvas.h"
#include "graphics/canvas/CanvasContext.h"
#include "graphics/canvas/Coord.h"
#include "graphics/event/EventController.h"
#include "graphics/func/Command.h"
#include "graphics/geom/Area.h"
#include "graphics/geom/Candle.h"
#include "graphics/geom/Interval.h"
#include "graphics/geom/Line.h"
#include "graphics/geom/Point.h"
#include "graphics/geom/shape/GeomShapeFactory.h"
#include "graphics/guide/GuideController.h"
#include "graphics/interaction/InteractionBase.h"
#include "graphics/interaction/InteractionContext.h"
#include "graphics/interaction/Pan.h"
#include "graphics/interaction/Pinch.h"
#include "graphics/legend/LegendController.h"
#include "graphics/scale/ScaleController.h"
#include "graphics/tooltip/TooltipController.h"
#include <algorithm>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <utils/Tracer.h>
#include <vector>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#include "ios/CoreGraphicsContext.h"
#endif

#if defined(ANDROID)
#include "android/BitmapCanvasContext.h"
#endif

#if defined(TARGET_ALIPAY)
#include "graphics/canvas/AlipayCanvasContext.h"
#include <AntGraphic/AntGraphic.h>
#endif

#ifndef XG_GRAPHICS_XCHART_H
#define XG_GRAPHICS_XCHART_H

#define XG_MEMBER_CALLBACK(funPtr) std::bind(&funPtr, this)
#define XG_MEMBER_CALLBACK_1(funPtr) std::bind(&funPtr, this, std::placeholders::_1)
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
    XChart(const std::string &name, double width, double height, double ratio = 1.0);
    ~XChart();

    XChart &Source(const std::string &json);

#if defined(ANDROID)
    ///方便降级 稳定后改回SetCanvasContext
    XChart &SetAndroidCanvasContext(jobject context) {
        XG_RELEASE_POINTER(canvasContext_);
        canvasContext_ = new canvas::BitmapCanvasContext(context, static_cast<float>(ratio_));
        return *this;
    }
#endif

#if defined(TARGET_ALIPAY)
    XChart &SetCanvasContext(ag::CanvasRenderingContext2D *context) {
        XG_RELEASE_POINTER(canvasContext_);
        canvasContext_ = new canvas::AliPayCanvasContext(context, static_cast<float>(ratio_), nullptr);
        return *this;
    }
#endif // TARGET_ALIPAY
    
#if defined(__APPLE__)
    ///方便降级 稳定后改回SetCanvasContext
    ///context == CGContextRef
    XChart &SetCoreGraphicsContext(void *context) {
        XG_RELEASE_POINTER(canvasContext_);
        canvasContext_ = new canvas::CoreGraphicsContext(context, width_, height_, static_cast<float>(ratio_), nullptr);
        return *this;
    }
#endif

    XChart &Padding(double left = 0.f, double top = 0.f, double right = 0.0, double bottom = 0.);
    XChart &Margin(double left = 0.f, double top = 0.f, double right = 0.0, double bottom = 0.);

    XChart &Scale(const std::string &field, const std::string &json);

    XChart &Axis(const std::string &field, const std::string &json = "");

    XChart &Legend(const std::string &field, const std::string &json = "");

    guide::GuideController &Guide() { return *this->guideController_; }

    XChart &Interaction(const std::string &type, nlohmann::json config = {});

    XChart &Tooltip(const std::string &json = "");

    XChart &Coord(const std::string &json = "");

    XChart &Animate(const std::string &json = "");

    bool OnTouchEvent(const std::string &json = "");

    geom::Line &Line();
    geom::Interval &Interval();
    geom::Area &Area();
    geom::Point &Point();
    geom::Candle &Candle();

    void Render();

    void Repaint();

    void Clear();

    std::size_t RequestAnimationFrame(func::Command *c, long delay = 16);

    scale::AbstractScale &GetScale(const std::string &field);

    /// 暴露共有属性
    inline nlohmann::json &GetData() { return data_; }
    inline canvas::coord::AbstractCoord &GetCoord() { return *(coord_.get()); }
    inline void SetColDefs() {}

    inline std::string GetXScaleField() { return geoms_[0]->GetXScaleField(); }

    std::vector<std::string> getYScaleFields();

    inline const std::string &GetChartName() { return this->chartName_; }

    inline const std::string &GetChartId() { return chartId_; }

    inline utils::Tracer *GetLogTracer() const { return this->logTracer_; }

    inline const double GetWidth() const noexcept { return width_; }
    inline const double GetHeight() const noexcept { return height_; }

    std::string GetScaleTicks(const std::string &field) noexcept;

    // 计算某一项数据对应的坐标位置
    const util::Point GetPosition(const nlohmann::json &item);

    // 计算某个位置对应的数据信息
    const std::string GetTooltipInfos(float touchX, float touchY, int geomIndex);

    std::string GetRenderInfo() const;

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

    inline void SetRequestFrameFuncId(std::string funcId) noexcept { this->requestFrameHandleId_ = funcId; }
    inline long long GetRenderDurationMM() const { return renderDurationMM_;}
    long GetRenderCount() const { return canvasContext_->GetRenderCount();}
  private:
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

    void Redraw();

    void NotifyAction(const std::string &action) {
        auto &chartCallbacks = chartActionListeners_[action];
        std::for_each(chartCallbacks.begin(), chartCallbacks.end(), [&](ChartActionCallback &callback) -> void { callback(); });

        auto &renderCallbacks = renderActionListeners_[action];
        std::for_each(renderCallbacks.begin(), renderCallbacks.end(), [&](ChartActionCallback &callback) -> void { callback(); });
    }

  private:
    bool rendered_ = false;
    nlohmann::json data_;
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

    utils::Tracer *logTracer_ = nullptr;
    std::unique_ptr<geom::shape::GeomShapeFactory> geomShapeFactory_ = nullptr;

    nlohmann::json coordCfg_ = {{"type", "cartesian"}, {"transposed", false}};

    std::map<std::string, std::vector<ChartActionCallback>> renderActionListeners_{}; // 针对每次渲染的监听事件， clear 时会清除
    std::map<std::string, std::vector<ChartActionCallback>> chartActionListeners_{}; // 针对 chart 实例的监听事件，只有在 chart 回收时才清除
    std::vector<std::unique_ptr<xg::interaction::InteractionBase>> interactions_{};

    std::string chartId_;
    std::string requestFrameHandleId_ = "";
    nlohmann::json animateCfg_ = false;
};
} // namespace xg

#endif // XG_GRAPHICS_XCHART_H
