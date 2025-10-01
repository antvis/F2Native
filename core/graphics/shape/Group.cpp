#include "graphics/shape/Group.h"
#include "graphics/util/Vector2d.h"
#include <float.h>

using namespace xg::util;

void xg::shape::Group::DrawInner(canvas::CanvasContext &context) const {
    std::for_each(children_.begin(), children_.end(), [&](const std::unique_ptr<Element> &e) { e->Draw(context); });
}

const BBox &xg::shape::Group::GetBBox(canvas::CanvasContext &context) {
    float minX = FLT_MAX;
    float maxX = FLT_MIN;
    float minY = FLT_MAX;
    float maxY = FLT_MIN;
    for(std::size_t i = 0; i < children_.size(); ++i) {
        const std::unique_ptr<Element> &child = children_[i];
        if(!child->IsVisible()) {
            continue;
        }

        const BBox &box = child->CalculateBox(context);
        if(std::isnan(box.minX)) {
            continue;
        }

        Vector2D leftTop{box.minX, box.minY};
        Vector2D leftBottom{box.minX, box.maxY};
        Vector2D rightTop{box.maxX, box.minY};
        Vector2D rightBottom{box.maxX, box.maxY};

        Vector2DUtil::TransformMat2D(&leftTop, leftTop, matrix_);
        Vector2DUtil::TransformMat2D(&leftBottom, leftBottom, matrix_);
        Vector2DUtil::TransformMat2D(&rightTop, rightTop, matrix_);
        Vector2DUtil::TransformMat2D(&rightBottom, rightBottom, matrix_);

        minX = std::min({static_cast<float>(leftTop[0]), static_cast<float>(leftBottom[0]), static_cast<float>(rightTop[0]),
                         static_cast<float>(rightBottom[0]), minX});
        maxX = std::max({static_cast<float>(leftTop[0]), static_cast<float>(leftBottom[0]), static_cast<float>(rightTop[0]),
                         static_cast<float>(rightBottom[0]), maxX});
        minY = std::min({static_cast<float>(leftTop[1]), static_cast<float>(leftBottom[1]), static_cast<float>(rightTop[1]),
                         static_cast<float>(rightBottom[1]), minY});
        minY = std::max({static_cast<float>(leftTop[1]), static_cast<float>(leftBottom[1]), static_cast<float>(rightTop[1]),
                         static_cast<float>(rightBottom[1]), maxY});
    }
    bbox_.minX = minX;
    bbox_.maxX = maxX;
    bbox_.minY = minY;
    bbox_.maxY = maxY;
    bbox_.width = maxX - minX;
    bbox_.height = maxY - minY;
    bbox_.x = minX;
    bbox_.y = minY;
    return bbox_;
}

void xg::shape::Group::Destroy() {
    Element::Destroy();
    children_.clear();
}

void xg::shape::Group::DoClip(canvas::CanvasContext &context) const {
    if(clip_ != nullptr && clip_.get() != nullptr) {
        clip_->ResetTransform(context);
        clip_->CreatePath(context);
        context.Clip();
    }
}

void xg::shape::Group::DoClipByIdentifier(canvas::CanvasContext &context, string identifier) const {
    auto it = shapeAnimationDictionary.find(identifier);
    if (it != shapeAnimationDictionary.end()) {
        it->second->ResetTransform(context);
        it->second->CreatePath(context);
        context.Clip();
    }
}

bool xg::shape::Group::NeedShapeClip(canvas::CanvasContext &, const std::function<void(string)>& callback) const {
    if (!shapeAnimationDictionary.empty()) {
        //到这说明这个group的子shape有动画
        //所以要根据shape，分别去clip，再去draw
        for (auto it = shapeAnimationDictionary.begin(); it != shapeAnimationDictionary.end(); ++it) {
            //回调给Element，把子动画的identifier传出去，外面拿到之后找到clip区域
            callback(it->first);
        }
        return true;
    }
    return false;
}

void xg::shape::Group::Translate(float x, float y) {
    if(!this->IsVisible())
        return;
    std::for_each(children_.begin(), children_.end(),
                  [&](std::unique_ptr<Element> &element) -> void { element->Translate(x, y); });
}

void xg::shape::Group::Rotate(float rad) {
    if(!this->IsVisible())
        return;
    std::for_each(children_.begin(), children_.end(), [&](std::unique_ptr<Element> &element) -> void { element->Rotate(rad); });
}

void xg::shape::Group::Scale(float sx, float sy) {
    if(!this->IsVisible())
        return;
    std::for_each(children_.begin(), children_.end(), [&](std::unique_ptr<Element> &element) -> void { element->Scale(sx, sy); });
}

void xg::shape::Group::MoveTo(float x, float y) {
    if(!this->IsVisible())
        return;
    std::for_each(children_.begin(), children_.end(), [&](std::unique_ptr<Element> &element) -> void { element->MoveTo(x, y); });
}

void xg::shape::Group::Apply(Vector2D *v) {
    if(!this->IsVisible())
        return;
    std::for_each(children_.begin(), children_.end(), [&](std::unique_ptr<Element> &element) -> void { element->Apply(v); });
}

void xg::shape::Group::Alpha(float alpha) {
    if(!this->IsVisible())
        return;
    std::for_each(children_.begin(), children_.end(), [&](std::unique_ptr<Element> &element) -> void { element->Alpha(alpha); });
}

xg::shape::Group *xg::shape::Group::AddGroup() {
    auto g = std::make_unique<xg::shape::Group>();
    xg::shape::Group *gp = g.get();
    this->AddElement(std::move(g));
    return gp;
}

void xg::shape::Group::UpdateAttribute(std::string attrName, double val) {
    if(attrName == "alpha") {
        this->Alpha(val);
    } 
}
