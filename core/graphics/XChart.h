
#include "graphics/axis/AxisController.h"
#include "graphics/canvas/Canvas.h"
#include "graphics/canvas/CanvasContext.h"
#include "graphics/canvas/Coord.h"
#include "graphics/event/EventController.h"
#include "graphics/geom/Area.h"
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
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <utils/Tracer.h>
#include <vector>

#if defined(TARGET_STANDALONE)

#if defined(ANDROID)
#include "android/F2CanvasView.h"
#else
#endif

#elif defined(TARGET_ALIPAY)
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
    friend legend::LegendController;
    friend event::EventController;
    friend tooltip::ToolTipController;
    friend interaction::Pan;
    friend interaction::Pinch;
    friend interaction::InteractionContext;

  public:
    XChart(const std::string &name, double width, double height, double ratio = 1.0);
    ~XChart();

    XChart &Source(const std::string &json);

#if defined(TARGET_STANDALONE)
#if defined(ANDROID)
    XChart &SetCanvasContext(F2CanvasView *context) {
        canvasContext_ = new canvas::CanvasContext(context, static_cast<float>(ratio_), nullptr);
        return *this;
    }
#else
    XChart &SetCanvasContext(GCanvasContext *context) {
        canvasContext_ = new canvas::CanvasContext(context, static_cast<float>(ratio_), nullptr);
        return *this;
    }
#endif
#endif // TARGET_STANDALONE

#if defined(TARGET_ALIPAY)
    XChart &SetCanvasContext(ag::CanvasRenderingContext2D *context) {
        canvasContext_ = new canvas::CanvasContext(context, static_cast<float>(ratio_), nullptr);
        return *this;
    }
#endif // TARGET_ALIPAY

    XChart &Padding(double left = 0.f, double top = 0.f, double right = 0.0, double bottom = 0.);
    XChart &Margin(double left = 0.f, double top = 0.f, double right = 0.0, double bottom = 0.);

    XChart &Scale(const std::string &field, const std::string &json);

    XChart &Axis(const std::string &field, const std::string &json = "");

    XChart &Legend(const std::string &field, const std::string &json = "");

    guide::GuideController &Guide() { return *this->guideController_; }

    XChart &Interaction(const std::string &type, nlohmann::json config = {});

    XChart &Tooltip(const std::string &json = "");

    XChart &Coord(const std::string &json = "");

    bool OnTouchEvent(const std::string &json = "");

    geom::Line &Line();
    geom::Interval &Interval();
    geom::Area &Area();
    geom::Point &Point();

    void Render();

    void Repaint();

    void Clear();

    scale::AbstractScale &GetScale(const std::string &field);

    /// 暴露共有属性
    inline nlohmann::json &GetData() { return data_; }
    inline canvas::coord::AbstractCoord &GetCoord() { return *(coord_.get()); }
    inline void SetColDefs() {}

    inline std::string GetXScaleField() { return geoms_[0]->GetXScaleField(); }

    inline std::vector<std::string> getYScaleFields() {
        std::vector<std::string> fields;
        std::for_each(geoms_.begin(), geoms_.end(), [&](auto &geom) -> void { fields.push_back(geom->GetYScaleField()); });

        return fields;
    }

    inline const std::string GetChartName() { return this->chartName_; }

    inline const std::string GetChartId() { return chartId_; }

    inline utils::Tracer *GetLogTracer() const { return this->logTracer_; }

    inline const float GetRatio() { return ratio_; }

    inline const double GetWidth() const noexcept { return width_; }
    inline const double GetHeight() const noexcept { return height_; }

    // 计算某一项数据对应的坐标位置
    const util::Point GetPosition(const nlohmann::json &item);

    std::string GetRenderInfo() const;

    canvas::CanvasContext &GetCanvasContext() const { return *canvasContext_; }

    void AddMonitor(const std::string &action, ChartActionCallback callback) { actionListeners_[action].push_back(callback); }

    inline const std::array<double, 4> GetPadding() { return userPadding_; }
    inline const std::array<double, 4> GetMargin() { return margin_; }

  private:
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
        auto &callbacks = actionListeners_[action];
        std::for_each(callbacks.begin(), callbacks.end(), [&](ChartActionCallback &callback) -> void { callback(); });
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
    std::array<double, 4> padding_ = {{0, 0, 0, 0}};
    std::array<double, 4> userPadding_ = {{0, 0, 0, 0}};
    std::array<double, 4> margin_ = {{0, 0, 0, 0}};

    // 析构时要析构对应的 Geom 对象
    std::vector<std::unique_ptr<geom::AbstractGeom>> geoms_;

    std::string chartName_;
    double width_;
    double height_;
    double ratio_ = 1.0f;
    long renderDurationMM_ = 0;

    // 默认三层渲染
    shape::Group *backLayout_;
    shape::Group *midLayout_;
    shape::Group *frontLayout_;

    canvas::CanvasContext *canvasContext_ = nullptr;

    utils::Tracer *logTracer_ = nullptr;
    std::unique_ptr<geom::shape::GeomShapeFactory> geomShapeFactory_ = nullptr;

    nlohmann::json coordCfg_ = {{"type", "cartesian"}, {"transposed", false}};

    std::map<std::string, std::vector<ChartActionCallback>> actionListeners_{};
    std::vector<std::unique_ptr<xg::interaction::InteractionBase>> interactions_{};

    std::string chartId_;
};
} // namespace xg

#endif // XG_GRAPHICS_XCHART_H
