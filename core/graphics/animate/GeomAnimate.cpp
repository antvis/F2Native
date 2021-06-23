#include "GeomAnimate.h"
#include "graphics/XChart.h"
#include "graphics/canvas/Polar.h"
#include "graphics/shape/Rect.h"

using namespace xg;

#pragma animate : actions

void animate::action::GroupScaleIn(animate::TimeLine *timeLine,
                                   shape::Group *container,
                                   nlohmann::json &cfg,
                                   canvas::coord::AbstractCoord *coord,
                                   util::Point zeroY,
                                   std::string type) {
    float width = coord->GetWidth();
    float height = coord->GetHeight();
    util::Point start = coord->GetXAxis();
    util::Point end = coord->GetYAxis();

    float x;
    float y;

    auto clip = std::make_unique<shape::Rect>(util::Point{start.x, end.y}, util::Size{width, height});

    if(type == "y") {
        x = x + width / 2;
        y = fmin(zeroY.y, start.y);
    } else if(type == "x") {
        x = fmax(zeroY.x, start.x);
        y = end.y + height / 2;
    } else if(type == "xy") {
        // if(coord->GetType() == coord::CoordType::Polar) {
        x = coord->GetCenter().x;
        y = coord->GetCenter().y;
        // } else {
        // }
    }

    util::Vector2D v{x, y};
    util::Matrix endMatrix = GetScaledShapeMatrix(clip.get(), &v, type);
    AnimateState endState;
    endState.matrix = std::move(endMatrix);
    container->clip_ = std::move(clip);
    animate::DoAnimation(timeLine, container->clip_.get(), endState, cfg, [container]() -> void { container->clip_ = nullptr; });
}

static void animate::action::GroupWaveIn(animate::TimeLine *timeLine,
                                         shape::Group *container,
                                         nlohmann::json &cfg,
                                         canvas::coord::AbstractCoord *coord,
                                         util::Point zeroY) {
    std::unique_ptr<shape::Shape> clip = nullptr;
    AnimateState endState;

    if(canvas::coord::CoordType::Polar == coord->GetType()) {
        canvas::coord::Polar *polar = static_cast<canvas::coord::Polar *>(coord);
        clip = xg::make_unique<xg::shape::Rect>(polar->GetCenter(), polar->circleRadius_, 0, polar->startAngle_, polar->endAngle_, 1.f);
        endState.angle = polar->endAngle_;
        clip->UpdateAttribute("endAngle", polar->startAngle_);
    } else {
        clip = std::make_unique<shape::Rect>(util::Point{coord->GetXAxis().x, coord->GetYAxis().y},
                                             util::Size{coord->GetWidth(), coord->GetHeight()});

        if(coord->IsTransposed()) {
            endState.height = coord->GetHeight();
            clip->UpdateAttribute("height", 0);
        } else {
            endState.width = coord->GetWidth();
            clip->UpdateAttribute("width", 0);
        }
    }
    container->clip_ = std::move(clip);
    animate::DoAnimation(timeLine, container->clip_.get(), endState, cfg, [container]() -> void { container->clip_ = nullptr; });
}

static void animate::action::DoGroupAnimate(std::string animateType,
                                            animate::TimeLine *timeLine,
                                            shape::Group *container,
                                            nlohmann::json &cfg,
                                            canvas::coord::AbstractCoord *coord,
                                            util::Point zeroY) {
    if(animateType == "GroupWaveIn") {
        GroupWaveIn(timeLine, container, cfg, coord, zeroY);
    } else if(animateType == "GroupScaleInX") {
        GroupScaleIn(timeLine, container, cfg, coord, zeroY, "x");
    } else if(animateType == "GroupScaleInY") {
        GroupScaleIn(timeLine, container, cfg, coord, zeroY, "y");
    } else if(animateType == "GroupScaleInXY") {
        GroupScaleIn(timeLine, container, cfg, coord, zeroY, "xy");
    } else {
        GroupWaveIn(timeLine, container, cfg, coord, zeroY);
    }
}

#pragma aniamte static methods.
static util::Matrix animate::GetScaledShapeMatrix(shape::Shape *shape, util::Vector2D *v, std::string direct) {
    shape->Apply(v);
    double x = (*v)[0];
    double y = (*v)[1];
    util::Matrix scaledMatrix;
    if(direct == "x") {
        shape->Transform({
            {"t", util::Vector2D{x, y}},    //
            {"s", util::Vector2D{0.01, 1}}, //
            {"t", util::Vector2D{-x, -y}}   //
        });

        scaledMatrix = shape->GetMatrix();
        util::MatrixUtil::Transform(&scaledMatrix, {
                                                       {"t", util::Vector2D{x, y}},   //
                                                       {"s", util::Vector2D{100, 1}}, //
                                                       {"t", util::Vector2D{-x, -y}}, //
                                                   });
    } else if(direct == "y") {
        shape->Transform({
            {"t", util::Vector2D{x, y}},    //
            {"s", util::Vector2D{1, 0.01}}, //
            {"t", util::Vector2D{-x, -y}}   //
        });
        scaledMatrix = shape->GetMatrix();
        util::MatrixUtil::Transform(&scaledMatrix, {
                                                       {"t", util::Vector2D{x, y}},   //
                                                       {"s", util::Vector2D{1, 100}}, //
                                                       {"t", util::Vector2D{-x, -y}}, //
                                                   });
    } else if(direct == "xy") {
        shape->Transform({
            {"t", util::Vector2D{x, y}},       //
            {"s", util::Vector2D{0.01, 0.01}}, //
            {"t", util::Vector2D{-x, -y}}      //
        });
        scaledMatrix = shape->GetMatrix();
        util::MatrixUtil::Transform(&scaledMatrix, {
                                                       {"t", util::Vector2D{x, y}},     //
                                                       {"s", util::Vector2D{100, 100}}, //
                                                       {"t", util::Vector2D{-x, -y}},   //
                                                   });
    }
    return std::move(scaledMatrix);
}

static void animate::DoAnimation(animate::TimeLine *timeLine_,
                                 shape::Shape *shape,
                                 const animate::AnimateState &endState,
                                 nlohmann::json &cfg,
                                 std::function<void()> onEnd) {
    animate::AnimateState startState;
    startState.matrix = shape->GetMatrix();
    // todo 补充 startState 属性
    //    startState.engle = shape-> // read rect startAngle.
    animate::AnimInfo animInfo = animate::CreateAnimInfo(shape, startState, endState, cfg);
    animInfo.onEnd = std::move(onEnd);
    timeLine_->PushAnim(std::move(animInfo));
}

#pragma GeomAnimate
std::string animate::GeomAnimate::GetGeomAnimateDefaultCfg(std::string geomType, canvas::coord::AbstractCoord *coord) {
    bool isPolar = coord->GetType() == canvas::coord::CoordType::Polar;
    if(geomType == "line") {
        if(isPolar) {
            return "GroupScaleInXY";
        } else {
            return "GroupWaveIn";
        }
    } else if(geomType == "area") {
        if(isPolar) {
            return "GroupScaleInXY";
        } else {
            return "GroupWaveIn";
        }
    } else if(geomType == "point") {
        return "GroupScaleInXY";
    } else if(geomType == "interval") {
        if(isPolar) {
            return "GroupWaveIn";
        } else {
            return coord->IsTransposed() ? "GroupScaleInX" : "GroupScaleInY";
        }
    } else {
        return "GroupWaveIn";
    }
}

animate::GeomAnimate::GeomAnimate(XChart *chart) {
    this->chart_ = chart;
    this->timeLine_ = new animate::TimeLine(chart);
    chart_->AddMonitor(ACTION_CHART_BEFORE_CANVAS_DRAW, XG_MEMBER_CALLBACK(animate::GeomAnimate::OnBeforeCanvasDraw));
}

animate::GeomAnimate::~GeomAnimate() { XG_RELEASE_POINTER(timeLine_); }

void animate::GeomAnimate::OnBeforeCanvasDraw() {
    if(isUpdate_)
        return;
    // 各种元素的动画
    // 1. axis shapes
    // 2. guide shapes
    // 3. 自定义 shape
    // 4. geom 整体
    // 先只处理整个的 geom

    isUpdate_ = true;

    if(chart_->animateCfg_.is_boolean() && chart_->animateCfg_ == false) {
        return;
    }

    for(std::vector<std::unique_ptr<geom::AbstractGeom>>::iterator it = chart_->geoms_.begin(); it != chart_->geoms_.end(); ++it) {
        std::string type = (*it)->GetType();

        std::string animateType = animate::GeomAnimate::GetGeomAnimateDefaultCfg(type, chart_->coord_.get());
        nlohmann::json cfg = {
            {"animate", animateType}, //
            {"erasing", "linear"},    //
            {"delay", 16},            //
            {"duration", 450}         //
        };
        if(chart_->animateCfg_.is_object() && chart_->animateCfg_.contains(type)) {
            cfg.merge_patch(chart_->animateCfg_[type]);
        }

        // geomCfg
        auto &yScale = chart_->GetScale((*it)->GetYScaleField());
        util::Point zeroY = chart_->GetCoord().ConvertPoint(util::Point{0, yScale.Scale((*it)->GetYMinValue(*chart_))});
        animate::action::DoGroupAnimate(cfg["animate"], timeLine_, (*it)->container_, cfg, chart_->coord_.get(), zeroY);
    }
}