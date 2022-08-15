#include "GeomAnimate.h"
#include "../XChart.h"
#include "../canvas/Polar.h"
#include "../shape/Rect.h"

using namespace xg;

#pragma animate : actions

void animate::action::GroupScaleIn(animate::TimeLine *timeLine,
                                   shape::Group *container,
                                   const AnimateCfg &cfg,
                                   canvas::coord::AbstractCoord *coord,
                                   util::Point zeroY,
                                   std::string type) {
 
    util::Point start = coord->GetStart();
    util::Point end = coord->GetEnd();
    
    //极坐标系的GetWidth是角度，所以这里用start和end单独计算下
    float width = abs(start.x - end.x);
    float height = abs(start.y - end.y);

    float x = 0;
    float y = 0;

    auto clip = std::make_unique<shape::Rect>(util::Point{start.x, end.y}, util::Size{width, height});

    if(type == "y") {
        x = start.x + width / 2;
        y = fmin(zeroY.y, start.y);
    } else if(type == "x") {
        x = fmax(zeroY.x, start.x);
        y = start.y + height / 2;
    } else if(type == "xy") {
        if(coord->GetType() == coord::CoordType::Polar) {
            x = coord->GetCenter().x;
            y = coord->GetCenter().y;
        } else {
            x = (start.x + end.x) / 2;
            y = (start.y + end.y) / 2;
        }
    }

    util::Vector2D v{x, y};
    util::Matrix endMatrix = GetScaledShapeMatrix(clip.get(), &v, type);
    AnimateState endState;
    endState.matrix = std::move(endMatrix);
    container->clip_ = std::move(clip);
    animate::DoAnimation(timeLine, container->clip_.get(), endState, cfg, [container]() -> void { container->clip_ = nullptr; });
}

void animate::action::GroupScaleInX(animate::TimeLine *timeLine, shape::Group *container, const AnimateCfg &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY) {
    GroupScaleIn(timeLine, container, cfg, coord, zeroY, "x");
}

void animate::action::GroupScaleInY(animate::TimeLine *timeLine, shape::Group *container, const AnimateCfg &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY) {
    GroupScaleIn(timeLine, container, cfg, coord, zeroY, "y");
}

void animate::action::GroupScaleInXY(animate::TimeLine *timeLine, shape::Group *container, const AnimateCfg &cfg, canvas::coord::AbstractCoord *coord, util::Point zeroY) {
    GroupScaleIn(timeLine, container, cfg, coord, zeroY, "xy");
}

void animate::action::GroupWaveIn(animate::TimeLine *timeLine,
                                         shape::Group *container,
                                         const AnimateCfg &cfg,
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

void animate::action::DoGroupAnimate(std::string animateType,
                                            animate::TimeLine *timeLine,
                                            shape::Group *container,
                                            const AnimateCfg &cfg,
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
util::Matrix animate::GetScaledShapeMatrix(shape::Shape *shape, util::Vector2D *v, std::string direct) {
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
    return scaledMatrix;
}

void animate::DoAnimation(animate::TimeLine *timeLine_,
                                 shape::Shape *shape,
                                 const animate::AnimateState &endState,
                                 const AnimateCfg &cfg,
                                 std::function<void()> onEnd) {
    animate::AnimateState startState;
    startState.matrix = shape->GetMatrix();
    // todo 补充 startState 属性
    //    startState.engle = shape-> // read rect startAngle.
    animate::AnimInfo animInfo = animate::CreateAnimInfo(shape, startState, endState, cfg.delay, cfg.duration, cfg.erasing);
    animInfo.onEnd = std::move(onEnd);
    timeLine_->PushAnim(std::move(animInfo));
}

#pragma GeomAnimate

std::string animate::GeomAnimate::GetGeomAnimateDefaultCfg(const std::string &geomType, canvas::coord::AbstractCoord *coord) {
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

    if(!animateCfg_.enable) {
        return;
    }

    for(std::vector<std::unique_ptr<geom::AbstractGeom>>::iterator it = chart_->geoms_.begin(); it != chart_->geoms_.end(); ++it) {
        const std::string &geomType = (*it)->GetType();

        animateCfg_.animate = animate::GeomAnimate::GetGeomAnimateDefaultCfg(geomType, chart_->coord_.get());
        animateCfg_.erasing = "linear";
        animateCfg_.delay = 16;
        animateCfg_.duration = 450;
        
        //todo merge传入的动画配置 需要区分每一类geom的动画 
//        if(chart_->animateCfg_.is_object()) {
//            cfg.merge_patch(chart_->animateCfg_);
//        }

        // geomCfg
        auto &yScale = chart_->GetScale((*it)->GetYScaleField());
        util::Point zeroY = chart_->GetCoord().ConvertPoint(util::Point{0, yScale.Scale((*it)->GetYMinValue(*chart_))});
        animate::action::DoGroupAnimate(animateCfg_.animate, timeLine_, (*it)->container_, animateCfg_, chart_->coord_.get(), zeroY);
    }
}

void animate::GeomAnimate::Clear() {
    isUpdate_ = false;
    if (timeLine_) {
        timeLine_->Clear();
    }
}
