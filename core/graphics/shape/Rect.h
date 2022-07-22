#ifndef XG_GRAPHICS_SHAPE_RECT_H
#define XG_GRAPHICS_SHAPE_RECT_H

#include "Shape.h"

namespace xg {
namespace shape {

class Rect : public Shape {
  public:
    Rect(const util::Point &point, const util::Size &size) : Shape(), size_(size) { this->point_ = point; }

    Rect(const util::Point &point, const util::Size &size, const std::string &fillColor) : Shape(), size_(size) {
        this->point_ = point;
        SetFillStyle(fillColor);
    }

    Rect(const util::Point &point, const util::Size &size, const std::string &strokeColor, float lineWidth)
        : Shape(), size_(size) {
        this->point_ = point;
        SetStorkColor(strokeColor);
        lineWidth_ = lineWidth;
    }

    Rect(const util::Point &point, const util::Size &size, const std::string &fillColor, const std::string &strokeColor, float lineWidth)
        : Shape(), size_(size) {
        this->point_ = point;
      
        SetFillStrokeStyle(strokeColor, fillColor);
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
        lineWidth_ = lineWidth;
        SetFillStrokeStyle(strokeColor, fillColor);
    }

    util::BBox CalculateBox(canvas::CanvasContext &context) const override;

    virtual void UpdateAttribute(std::string attrName, double val) override;
    
    void SetRoundings(std::array<float, 4> roundings);
    
    void SetDashLine(const std::vector<float> &params) { this->dash_ = params; }

  protected:
    void CreatePath(canvas::CanvasContext &context) const override;

  private:
    bool HasRounding() const {
        bool rst = false;
        for(std::size_t i = 0; i < 4; ++i) {
            if(roundings_[i] > XG_EPS) {
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
    std::array<float, 4> roundings_ = {0, 0, 0, 0};

    std::vector<float> dash_;
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_RECT_H */
