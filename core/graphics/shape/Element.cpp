#include "graphics/shape/Element.h"
#include "utils/common.h"

#pragma mark static
bool xg::shape::Element::IsUnchanged(const Matrix &m) {
    return xg::IsEqual(m[0], 1.) && xg::IsEqual(m[1], 0.) && xg::IsEqual(m[2], 0.) && xg::IsEqual(m[3], 1.) &&
           xg::IsEqual(m[4], 0.) && xg::IsEqual(m[5], 0);
}

#pragma mark public
xg::shape::Element::Element() { InitTransform(); }

void xg::shape::Element::Draw(canvas::CanvasContext &context) const {
    if(IsDestroyed()) {
        return;
    }

    if(IsVisible()) {
        //clip之前需要看是否需要有子shape的多段切割
//        SetContext(context);
        if (NeedShapeClip(context, [&](string value) {
            context.Save();
            DoClipByIdentifier(context, value);
            ResetContext(context);
            ResetTransform(context);
            DrawInner(context);
            RestoreContext(context);
        })) {
            //
        } else {
            SetContext(context);
            //Group 或者 Shape
            //如果是Group的话，继续调用链路
            DrawInner(context);
            RestoreContext(context);
        }
    }
}

void xg::shape::Element::Destroy() {
    if(IsDestroyed()) {
        return;
    }
    RemoveFromParent();
    destroyed_ = true;
}

void xg::shape::Element::Remove() {
    if(IsDestroyed()) {
        Destroy();
    } else {
        RemoveFromParent();
    }
}

void xg::shape::Element::Translate(float x, float y) { util::MatrixUtil::Translate(&this->matrix_, matrix_, {x, y}); }

void xg::shape::Element::MoveTo(float x, float y) {
    this->Translate(x - this->point_.x, y - this->point_.y);
    this->point_ = {x, y};
}

void xg::shape::Element::Scale(float sx, float sy) { util::MatrixUtil::Scale(&this->matrix_, matrix_, {sx, sy}); }

void xg::shape::Element::Rotate(float rad) { util::MatrixUtil::Rotate(&this->matrix_, matrix_, rad); }

void xg::shape::Element::Apply(Vector2D *v) { util::Vector2DUtil::TransformMat2D(v, *v, matrix_); }

void xg::shape::Element::Transform(const std::vector<TransformAction> &actions) { MatrixUtil::Transform(&matrix_, actions); }

void xg::shape::Element::Alpha(float alpha) { fillOpacity_ = alpha; }

#pragma mark private
void xg::shape::Element::InitTransform() {
    matrix_ = {1, 0, 0, 1, 0, 0};
}

void xg::shape::Element::ResetTransform(canvas::CanvasContext &context) const {
    if(!MatrixUtil::IsUnchanged(matrix_)) {
        context.SetTransform(matrix_[0], matrix_[1], matrix_[2], matrix_[3], matrix_[4], matrix_[5]);
    }
}

void xg::shape::Element::SetContext(canvas::CanvasContext &context) const {
    context.Save();
    DoClip(context);
    ResetContext(context);
    ResetTransform(context);
}

void xg::shape::Element::RestoreContext(canvas::CanvasContext &context) const { context.Restore(); }

void xg::shape::Element::ResetContext(canvas::CanvasContext &context) const {
    if(!IsGroup()) {
        if(HasStroke()) {
            context.SetStrokeStyle(strokeStyle_);
        }
        if(HasFill()) {
            context.SetFillStyle(fillStyle_);
        }

        if(!font_.empty()) {
            context.SetFont(font_);
        }

        if(!textAlign_.empty()) {
            context.SetTextAlign(textAlign_);
        }

        if(!textBaseline_.empty()) {
            context.SetTextBaseline(textBaseline_);
        }

        if(!std::isnan(lineWidth_)) {
            context.SetLineWidth(lineWidth_);
        }
        
        if(!lineCap_.empty()) {
            context.SetLineCap(lineCap_);
        }
        
        if(!lineJoin_.empty()) {
            context.SetLineJoin(lineJoin_);
        }

        // line dash set
    }
}
