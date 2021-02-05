#ifndef XG_GRAPHICS_SHAPE_RECT_H
#define XG_GRAPHICS_SHAPE_RECT_H

#include "graphics/shape/Shape.h"
#include "graphics/util/BBox.h"

namespace xg {
namespace shape {

class Rect : public Shape {
  public:
    Rect(const util::Point &point, const util::Size &size) : Shape(), size_(size) { this->point_ = point; }

    Rect(const util::Point &point, const util::Size &size, const std::string &fillColor) : Shape(), size_(size) {
        this->point_ = point;
        fillStyle_ = util::CanvasFillStrokeStyle(fillColor);
    }

    Rect(const util::Point &point, const util::Size &size, const std::string &strokeColor, float lineWidth)
        : Shape(), size_(size) {
        this->point_ = point;
        strokeStyle_ = util::CanvasFillStrokeStyle(strokeColor);
        lineWidth_ = lineWidth;
    }

    Rect(const util::Point &point, const util::Size &size, const std::string &fillColor, const std::string &strokeColor, float lineWidth)
        : Shape(), size_(size) {
        this->point_ = point;
        if(!fillColor.empty()) {
            fillStyle_ = util::CanvasFillStrokeStyle(fillColor);
        }
        if(!strokeColor.empty()) {
            strokeStyle_ = util::CanvasFillStrokeStyle(strokeColor);
        }
        lineWidth_ = lineWidth;
    }

    Rect(const util::Point &point, const double radius, const double radius0, const double startAngle, const double endAngle, float lineWidth)
        : Shape(), radius_(radius), radius0_(radius0), startAngle_(startAngle), endAngle_(endAngle) {

        this->point_ = point;
        lineWidth_ = lineWidth;
    }

    Rect(const util::Point &point,
         const double radius,
         const double radius0,
         const double startAngle,
         const double endAngle,
         const std::string &fillColor,
         float lineWidth,
         const std::string &strokeColor)
        : Shape(), radius_(radius), radius0_(radius0), startAngle_(startAngle), endAngle_(endAngle) {
        this->point_ = point;

        if(!fillColor.empty()) {
            fillStyle_ = util::CanvasFillStrokeStyle(fillColor);
        }
        if(!strokeColor.empty()) {
            strokeStyle_ = util::CanvasFillStrokeStyle(strokeColor);
        }
        lineWidth_ = lineWidth;
    }

    util::BBox CalculateBox(canvas::CanvasContext &context) const override;

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;

  private:
    bool HasRounding() const {
        bool rst = false;
        for(std::size_t i = 0; i < 4; ++i) {
            if(roundings[i] > XG_EPS) {
                rst = true;
                break;
            }
        }
        return rst;
    }

  public:
    util::Size size_;
    double radius_ = 0.f;
    double startAngle_ = 0.;
    double endAngle_ = 0.;
    double radius0_ = 0.f;
    // 圆角 [ tl, tr, bl, br ]
    float roundings[4] = {0, 0, 0, 0};
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_RECT_H */
