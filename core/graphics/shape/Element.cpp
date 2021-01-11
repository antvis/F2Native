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
        SetContext(context);
        DrawInner(context);
        RestoreContext(context);
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

#pragma mark private
void xg::shape::Element::InitTransform() { matrix_ = {1, 0, 0, 1, 0, 0}; }

void xg::shape::Element::ResetTransform(canvas::CanvasContext &context) const {
    if(!IsUnchanged(matrix_)) {
        context.SetTransform(matrix_[0], matrix_[1], matrix_[2], matrix_[3], matrix_[4], matrix_[5]);
    }
}

void xg::shape::Element::SetContext(canvas::CanvasContext &context) const {
    context.Save();
    ResetContext(context);
    ResetTransform(context);
}

void xg::shape::Element::RestoreContext(canvas::CanvasContext &context) const { context.Restore(); }

void xg::shape::Element::ResetContext(canvas::CanvasContext &context) const {
    if(!IsGroup()) {
        if(!stroke_.empty()) {
            context.SetStrokeStyle(stroke_);
        }
        if(!fill_.empty()) {
            context.SetFillStyle(fill_);
        }

        if(!font_.empty()) {
            context.SetFont(font_);
        }

        if(!textAlign_.empty()) {
            //            GTextAlign align = context.TextAlign();
            //            if(textAlign_ == "start")
            //                align = TEXT_ALIGN_START;
            //            else if(textAlign_ == "end")
            //                align = TEXT_ALIGN_END;
            //            else if(textAlign_ == "left")
            //                align = TEXT_ALIGN_LEFT;
            //            else if(textAlign_ == "center")
            //                align = TEXT_ALIGN_CENTER;
            //            else if(textAlign_ == "right")
            //                align = TEXT_ALIGN_RIGHT;
            context.SetTextAlign(textAlign_);
        }

        if(!textBaseline_.empty()) {
            //            GTextBaseline baseline = context.TextBaseline();
            //            if(textBaseline_ == "alphabetic")
            //                baseline = TEXT_BASELINE_ALPHABETIC;
            //            else if(textBaseline_ == "middle")
            //                baseline = TEXT_BASELINE_MIDDLE;
            //            else if(textBaseline_ == "top")
            //                baseline = TEXT_BASELINE_TOP;
            //            else if(textBaseline_ == "hanging")
            //                baseline = TEXT_BASELINE_HANGING;
            //            else if(textBaseline_ == "bottom")
            //                baseline = TEXT_BASELINE_BOTTOM;
            //            else if(textBaseline_ == "ideographic")
            //                baseline = TEXT_BASELINE_IDEOGRAPHIC;
            context.SetTextBaseline(textBaseline_);
        }

        if(!std::isnan(lineWidth_)) {
            context.SetLineWidth(lineWidth_);
        }

        // line dash set
    }
}
