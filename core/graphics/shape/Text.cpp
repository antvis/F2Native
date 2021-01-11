#include "graphics/shape/Text.h"
#include <utils/StringUtil.h>

xg::shape::Text::Text(const string &text, const Point &pt, const float fontSize, const string &strokeColor, const string &fillColor) {
    canFill_ = true;
    canStroke_ = true;
    type_ = "text";
    lineWidth_ = std::nan("0");
    point_ = pt;
    fontSize_ = fontSize;
    stroke_ = strokeColor;
    fill_ = fillColor;
    font_ = GetFontStyle();
    text_ = text;
    lineCount_ = 1;
    if(text.size() && text.find("\n") != string::npos) {
        StringUtil::Split(text, textArr_, '\n');
        lineCount_ = (int)textArr_.size();
    }
}

void xg::shape::Text::DrawInner(canvas::CanvasContext &context) const {
    if(std::isnan(point_.x) || std::isnan(point_.y)) {
        return;
    }

    if(HasFill()) {
        if(!std::isnan(fillOpacity_)) {
            context.SetGlobalAlpha(fillOpacity_);
        }
        if(textArr_.size()) {
            float spaceingY = GetSpacingY();
            float height = GetTextHeight();
            for(std::size_t i = 0; i < textArr_.size(); ++i) {
                float subY = point_.y + i * (spaceingY + fontSize_) - height + fontSize_;
                if(textBaseline_ == "middle") {
                    subY += height - fontSize_ - (height - fontSize_) / 2;
                } else if(textBaseline_ == "top") {
                    subY += height - fontSize_;
                }
                context.FillText(textArr_[i], point_.x, subY);
            }
        } else {
            context.FillText(text_, point_.x, point_.y);
        }
    }
}

BBox xg::shape::Text::CalculateBox(canvas::CanvasContext &context) const {
    if(text_.empty()) {
        return {static_cast<float>(std::nan("0"))};
    } else {
        const float height = GetTextHeight();
        const float width = GetTextWidth(context);
        Point point(point_.x, point_.y - height);
        if(textAlign_ == "end" || textAlign_ == "right") {
            point.x -= width;
        } else if(textAlign_ == "center") {
            point.x -= width / 2;
        }

        if(textBaseline_ == "top") {
            point.y += height;
        } else if(textBaseline_ == "middle") {
            point.y += height / 2;
        }
        return {static_cast<float>(point.x),
                static_cast<float>(point.x + width),
                static_cast<float>(point.y),
                static_cast<float>(point.y + height),
                width,
                height,
                static_cast<float>(point_.x),
                static_cast<float>(point_.y)};
    }
}

string xg::shape::Text::GetFontStyle() const {
    return fontStyle_ + " " + fontVariant_ + " " + fontWeight_ + " " + std::to_string(fontSize_) + "px " + fontFamily_;
}

float xg::shape::Text::GetSpacingY() const { return std::isnan(lineHeight_) ? fontSize_ * 0.14 : (lineHeight_ - fontSize_); }

float xg::shape::Text::GetTextHeight() const {
    if(!std::isnan(height_)) {
        return height_;
    }
    const float fontSize = fontSize_ * 1;
    if(lineCount_ > 1) {
        return fontSize * lineCount_ + GetSpacingY() * (lineCount_ - 1);
    }
    return fontSize;
}

float xg::shape::Text::GetTextWidth(canvas::CanvasContext &context) const {
    if(!std::isnan(width_)) {
        return width_;
    }

    context.SetFont(font_);
    if(textArr_.size()) {
        float maxWidth = 0;
        for_each(textArr_.begin(), textArr_.end(),
                 [&maxWidth, &context](const std::string &t) { maxWidth = std::max(maxWidth, context.MeasureTextWidth(t)); });
        return maxWidth;
    } else {
        return context.MeasureTextWidth(text_);
    }
}
