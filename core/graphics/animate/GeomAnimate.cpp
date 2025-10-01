#include "GeomAnimate.h"
#include "../XChart.h"
#include "../canvas/Polar.h"
#include "../shape/Rect.h"
#include <iostream>
#include <string>
#include <utils/StringUtil.h>
#include <unordered_map>
#include <map>

using namespace xg;

#pragma animate : actions


/// 整体动画分发
/// - Parameters:
///   - animateType: 动画类型
///   - timeLine: 动画控制器
///   - container: Group
///   - cfg: 动画配置
///   - coord: 坐标系
///   - zeroY: Y轴起点
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
    } else if(animateType == "GroupFadeIn") {
        GroupFadeIn(timeLine, container, cfg, coord);
    } else {
        GroupWaveIn(timeLine, container, cfg, coord, zeroY);
    }
}

/// 整体缩放动画
/// - Parameters:
///   - timeLine: 动画控制器
///   - container: Group
///   - cfg: 动画配置
///   - coord: 坐标系
///   - zeroY: Y轴起点
///   - type: 缩放动画的类型, x/y/xy
void animate::action::GroupScaleIn(animate::TimeLine *timeLine,
                                   shape::Group *container,
                                   nlohmann::json &cfg,
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
    AnimateState startState;
    endState.matrix = std::move(endMatrix);
    startState.matrix = clip->GetMatrix();
    container->clip_ = std::move(clip);
    animate::DoAnimation(timeLine, container->clip_.get(), startState, endState, cfg, [container]() -> void { container->clip_ = nullptr; });
}

/// 整体波动入场动画，
/// 不同坐标系下效果不同，极坐标系是0～360度的动画，正常坐标系是从左往右的动画（宽度），转置之后，变成高度的动画（height）
/// - Parameters:
///   - timeLine: 动画控制器
///   - container: Group
///   - cfg: 动画配置
///   - coord: 坐标系
///   - zeroY: Y轴起点
static void animate::action::GroupWaveIn(animate::TimeLine *timeLine,
                                         shape::Group *container,
                                         nlohmann::json &cfg,
                                         canvas::coord::AbstractCoord *coord,
                                         util::Point zeroY) {
    std::unique_ptr<shape::Shape> clip = nullptr;
    AnimateState endState;
    AnimateState startState;
    
    if(canvas::coord::CoordType::Polar == coord->GetType()) {
        canvas::coord::Polar *polar = static_cast<canvas::coord::Polar *>(coord);
        clip = xg::make_unique<xg::shape::Rect>(polar->GetCenter(), polar->circleRadius_, 0, polar->startAngle_, polar->endAngle_, 1.f);
        startState.angle = polar->startAngle_;
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
    startState.matrix = clip->GetMatrix();
    container->clip_ = std::move(clip);
    animate::DoAnimation(timeLine, container->clip_.get(), startState, endState, cfg, [container]() -> void { container->clip_ = nullptr; });
}

static void animate::action::GroupFadeIn(animate::TimeLine *timeLine,
                                         shape::Group *container,
                                         nlohmann::json &cfg,
                                         canvas::coord::AbstractCoord *coord) {
    AnimateState endState;
    AnimateState startState;
    
    startState.alpha = 0;
    endState.alpha = 1;
    if (!std::isnan(container->GetFillOpacity())) {
        endState.alpha = container->GetFillOpacity();
    }
    
    animate::DoAnimation(timeLine, container, startState, endState, cfg, [container]() -> void {  });
}

/// Shape动画分发
/// - Parameters:
///   - animateType: 动画类型
///   - timeLine: 动画控制器
///   - shape: 执行动画对象Shape
///   - container: Shape所属Group
///   - cfg: 动画配置
///   - coord: 坐标系
///   - zeroY: Y轴起点
///   - bbox: Shape的bbox
static void animate::action::DoShapeAnimate(std::string animateType,
                                            animate::TimeLine *timeLine,
                                            shape::Element *shape,
                                            shape::Group *container,
                                            nlohmann::json &cfg,
                                            canvas::coord::AbstractCoord *coord,
                                            util::Point zeroY,
                                            xg::util::BBox &bbox) {
    if(animateType == "ShapeScaleInX") {
        ShapeScaleIn(timeLine, shape, container, cfg, coord, zeroY, "x", bbox);
    } else if(animateType == "ShapeScaleInY") {
        ShapeScaleIn(timeLine, shape, container, cfg, coord, zeroY, "y", bbox);
    } else if(animateType == "ShapeScaleInXY") {
        ShapeScaleIn(timeLine, shape, container, cfg, coord, zeroY, "xy", bbox);
    }else if(animateType == "ShapeWaveIn") {
        ShapeWaveIn(timeLine, shape, container, cfg, coord, zeroY, bbox);
    }
}

/// 单个Shape 缩放动画
/// - Parameters:
///   - timeLine: 动画控制器
///   - shape: Shape
///   - container: Shape 所属 Group
///   - cfg: 动画配置
///   - coord: 坐标系
///   - zeroY: Y轴起点
///   - type: 缩放动画的类型, x/y/xy
///   - bbox: Shape的bbox
static void animate::action::ShapeScaleIn(animate::TimeLine *timeLine,
                                          shape::Element *shape,
                                          shape::Group *container,
                                          nlohmann::json &cfg,
                                          canvas::coord::AbstractCoord *coord,
                                          util::Point zeroY,
                                          std::string type,
                                          xg::util::BBox &bbox) {
    //极坐标系的GetWidth是角度，所以这里用start和end单独计算下
    float width = abs(bbox.maxX - bbox.minX);
    float height = abs(bbox.maxY - bbox.minY);
    
    float x = 0;
    float y = 0;

    auto clip = std::make_unique<shape::Rect>(util::Point{bbox.minX, fmin(bbox.minY, bbox.maxY)}, util::Size{width, height});

    if(type == "y") {
        x = bbox.minX + width / 2;
        y = fmin(zeroY.y, fmax(bbox.minY, bbox.maxY));
    } else if(type == "x") {
        x = fmax(zeroY.x, fmax(bbox.minX, bbox.maxX));
        y = bbox.minY + height / 2;
    } else if(type == "xy") {
        if(coord->GetType() == coord::CoordType::Polar) {
            x = coord->GetCenter().x;
            y = coord->GetCenter().y;
        } else {
            x = bbox.x;
            y = bbox.y;
        }
    }

    util::Vector2D v{x, y};
    util::Matrix endMatrix = GetScaledShapeMatrix(clip.get(), &v, type);
    AnimateState endState;
    AnimateState startState;
    endState.matrix = std::move(endMatrix);
    startState.matrix = clip->GetMatrix();
    
    //单个Shape动画的id
    string identifier = StringUtil::GenerateRandomString(10);
    clip->identifier = identifier;
    container->shapeAnimationDictionary[clip->identifier] = std::move(clip);
//    auto it = container->dictionary.find(identifier);
    animate::DoAnimation(timeLine, container->shapeAnimationDictionary[identifier].get(), startState, endState, cfg, [container, identifier]() -> void {
        //动画结束回调
        bool isTheLastAnimFinish = true;
        container->endAnimationElement.push_back(identifier);
        for (const auto& pair : container->shapeAnimationDictionary) {
            const std::string& key = pair.first; // 获取动画id
            bool hasEndThis = std::find(container->endAnimationElement.begin(), container->endAnimationElement.end(), key) != container->endAnimationElement.end();
            if (!hasEndThis) {
                //如果注册id中有一个不在动画完成的vector中，则代表动画没有结束
                isTheLastAnimFinish = false;
                break;
            }
        }
        if (isTheLastAnimFinish) {
            //Group内所有Shape动画结束，清空动画
            container->shapeAnimationDictionary.clear();
            container->endAnimationElement.clear();
        }
    });
}

static void animate::action::ShapeWaveIn(animate::TimeLine *timeLine,
                                         shape::Element *shape,
                                         shape::Group *container,
                                         nlohmann::json &cfg,
                                         canvas::coord::AbstractCoord *coord,
                                         util::Point zeroY,
                                         xg::util::BBox &bbox) {
    //极坐标系的GetWidth是角度，所以这里用start和end单独计算下
    float width = abs(bbox.maxX - bbox.minX);
    float height = abs(bbox.maxY - bbox.minY);
    
    float x = 0;
    float y = 0;

    auto clip = std::make_unique<shape::Rect>(util::Point{bbox.minX, fmax(bbox.minY, bbox.maxY)}, util::Size{width, -height});
    
    AnimateState endState;
    AnimateState startState;
    
    if(canvas::coord::CoordType::Polar == coord->GetType()) {
//        canvas::coord::Polar *polar = static_cast<canvas::coord::Polar *>(coord);
//        clip = xg::make_unique<xg::shape::Rect>(polar->GetCenter(), polar->circleRadius_, 0, polar->startAngle_, polar->endAngle_, 1.f);
//        startState.angle = polar->startAngle_;
//        endState.angle = polar->endAngle_;
//        clip->UpdateAttribute("endAngle", polar->startAngle_);
    } else {
//        clip = std::make_unique<shape::Rect>(util::Point{coord->GetXAxis().x, coord->GetYAxis().y},
//                                             util::Size{coord->GetWidth(), coord->GetHeight()});
        
//        if(coord->IsTransposed()) {
//            endState.height = height;
//            clip->UpdateAttribute("height", 0);
//        } else {
//            endState.width = width;
//            clip->UpdateAttribute("width", 0);
//        }
        endState.height = -height;
        clip->UpdateAttribute("height", 0);
    }
    startState.matrix = clip->GetMatrix();
    
    //单个Shape动画的id
    string identifier = StringUtil::GenerateRandomString(10);
    clip->identifier = identifier;
    container->shapeAnimationDictionary[clip->identifier] = std::move(clip);
//    auto it = container->dictionary.find(identifier);
    animate::DoAnimation(timeLine, container->shapeAnimationDictionary[identifier].get(), startState, endState, cfg, [container, identifier]() -> void {
        //动画结束回调
        bool isTheLastAnimFinish = true;
        container->endAnimationElement.push_back(identifier);
        for (const auto& pair : container->shapeAnimationDictionary) {
            const std::string& key = pair.first; // 获取动画id
            bool hasEndThis = std::find(container->endAnimationElement.begin(), container->endAnimationElement.end(), key) != container->endAnimationElement.end();
            if (!hasEndThis) {
                //如果注册id中有一个不在动画完成的vector中，则代表动画没有结束
                isTheLastAnimFinish = false;
                break;
            }
        }
        if (isTheLastAnimFinish) {
            //Group内所有Shape动画结束，清空动画
            container->shapeAnimationDictionary.clear();
            container->endAnimationElement.clear();
        }
    });
    
//    container->clip_ = std::move(clip);
//    animate::DoAnimation(timeLine, container->clip_.get(), startState, endState, cfg, [container]() -> void { container->clip_ = nullptr; });
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
                                 shape::Element *shape,
                                 const AnimateState &startState,
                                 const animate::AnimateState &endState,
                                 nlohmann::json &cfg,
                                 std::function<void()> onEnd) {
//    animate::AnimateState startState;
//    startState.matrix = shape->GetMatrix();
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

    int stepDelay = 0;
    for(std::vector<std::unique_ptr<geom::AbstractGeom>>::iterator it = chart_->geoms_.begin(); it != chart_->geoms_.end(); ++it) {
        const std::string &geomType = (*it)->GetType();
        std::string animateType = animate::GeomAnimate::GetGeomAnimateDefaultCfg(geomType, chart_->coord_.get());
        /*
        默认动画类型：
        line:
            极坐标 GroupScaleInXY
            笛卡尔坐标 GroupWaveIn
        area:
            极坐标 GroupScaleInXY
            笛卡尔坐标 GroupWaveIn
        point:
            GroupScaleInXY
        interval:
            极坐标 GroupWaveIn
            笛卡尔坐标 转置 ? GroupScaleInX : GroupScaleInY
        其他:
            GroupWaveIn
         */
        nlohmann::json cfg = {
            {"animateType", animateType},
            {"erasing", "linear"},
            {"delay", 16},
            {"duration", 450}
        };
        
        nlohmann::json &geomCfg = (*it)->getAnimateCfg();
        if (!geomCfg.empty()) {
            cfg.merge_patch(geomCfg);
        }

        auto &yScale = chart_->GetScale((*it)->GetYScaleField());
        util::Point zeroY = chart_->GetCoord().ConvertPoint(util::Point{0, yScale.Scale((*it)->GetYMinValue(*chart_))});
        string cfgType = json::GetString(cfg, "animateType", animateType);
        std::vector<std::string> shapeTypes = {"ShapeScaleInX", "ShapeScaleInY", "ShapeScaleInXY", "ShapeWaveIn"};
        if (std::find(shapeTypes.begin(), shapeTypes.end(), cfgType) != shapeTypes.end()) {
            std::for_each((*it)->container_->children_.begin(), (*it)->container_->children_.end(), [&](const std::unique_ptr<Element> &e) {
                int stepDelayCfg = json::GetNumber(cfg, "shapeInterval", 0);
                stepDelay += stepDelayCfg;
                cfg["delay"] = stepDelay;
                xg::util::BBox bbox = e->GetBBox(*chart_->canvasContext_);
                animate::action::DoShapeAnimate(json::GetString(cfg, "animateType", animateType), timeLine_, e.get(), (*it)->container_, cfg, chart_->coord_.get(), zeroY, bbox);
            });
        } else {
            animate::action::DoGroupAnimate(json::GetString(cfg, "animateType", animateType), timeLine_, (*it)->container_, cfg, chart_->coord_.get(), zeroY);
        }
    }
}

void animate::GeomAnimate::Clear() {
    isUpdate_ = false;
    if (timeLine_) {
        timeLine_->Clear();
    }
}
