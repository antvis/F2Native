#include "graphics/canvas/Coord.h"
#include "graphics/util/Matrix.h"
#include "graphics/util/Point.h"
#include "graphics/util/Vector2d.h"
#include <math.h>

#ifndef XG_GRAPHICS_COORD_POLAR_H
#define XG_GRAPHICS_COORD_POLAR_H

namespace xg {
namespace canvas {
namespace coord {

// 级坐标
class Polar : public AbstractCoord {
  public:
    // @param start -- 左下角坐标
    // @param end -- 右上角坐标
    // @param transposed -- 坐标系是否翻转
    Polar(util::Point start, util::Point end, bool transposed = false) : AbstractCoord() {
        transposed_ = transposed;
        Reset(start, end);
    }

    // @param point -- 度量坐标值[0~1]
    // @return  实际坐标值
    util::Point ConvertPoint(util::Point point) const override {

        double angle = xAxis_.x + (xAxis_.y - xAxis_.x) * (transposed_ ? point.y : point.x);
        double radius = yAxis_.x + (yAxis_.y - yAxis_.x) * (transposed_ ? point.x : point.y);

        double x = center_.x + std::cos(angle) * radius;
        double y = center_.y + std::sin(angle) * radius;

        util::Vector2D v;
        util::Vector2DUtil::Reset(&v, x, y);
        util::Vector2DUtil::TransformMat2D(&v, v, this->matrix_);

        return util::Point{v[0], v[1]};
    }

    // @param point -- 实际坐标值
    // @return 度量坐标值
    util::Point InvertPoint(util::Point point) const override {
        util::Matrix matrixM;
        util::Matrix m = { 1, 0, 0, 1, 0, 0};
        util::MatrixUtil::Rotate(&matrixM, m, xAxis_.x);
        util::Vector2D startV;
        util::Vector2D v = {1, 0};
        util::Vector2DUtil::TransformMat2D(&startV, v, matrixM);
        util::Vector2D pointV = {point.x - center_.x, point.y - center_.y};
        if(util::Vector2DUtil::Zero(pointV)) {
            return std::move(util::Point(0, 0));
        }
        double theta = util::Vector2DUtil::AngleTo(startV, pointV, xAxis_.y < xAxis_.x);
        if(abs(theta - M_PI * 2) < 0.001) {
            theta = 0;
        }
        double l = util::Vector2DUtil::Length(pointV);
        double percentX = theta / (xAxis_.y - xAxis_.x);
        percentX = xAxis_.y - xAxis_.x > 0 ? percentX : -percentX;
        double percentY = (l - yAxis_.x) / (yAxis_.y - yAxis_.x);
        if(this->transposed_) {
            return std::move(util::Point(percentY, percentX));
        } else {
            return std::move(util::Point(percentX, percentY));
        }
    }

    virtual void Reset(util::Point start, util::Point end) noexcept override {
        start_ = start;
        end_ = end;
        util::MatrixUtil::Reset(&this->matrix_);
        double width = abs(end.x - start.x);
        double height = abs(end.y - start.y);
        if(fabs(startAngle_ + M_PI) <= XG_EPS && fabs(endAngle_) <= XG_EPS) {
            circleRadius_ = fmin(width / 2, height);
            center_.Reset((start.x + end.x) / 2, start.y);
        } else {
            circleRadius_ = fmin(width, height) / 2;
            center_.Reset((start.x + end.x) / 2, (start.y + end.y) / 2);
        }
        xAxis_.Reset(startAngle_, endAngle_);
        yAxis_.Reset(0, circleRadius_);
    }

    virtual CoordType GetType() const noexcept override { return CoordType::Polar; }

    //是角度
    double GetWidth() const noexcept override { return xAxis_.y - xAxis_.x; }

    double GetRadius() const noexcept override { return circleRadius_; }

    //是角度
    util::Point GetXAxis() const noexcept override { return xAxis_; }

    //x是0，y是半径
    util::Point GetYAxis() const noexcept override { return yAxis_; }
    
    //坐标系左下角的点
    inline util::Point GetStart() noexcept override { return start_;}
    
    //坐标系右上角的点
    inline util::Point GetEnd() noexcept override { return end_;}

    bool IsContains(double x, double y) noexcept override {
        return (x >= GetStart().x && x <= GetEnd().x && y >= GetEnd().y && y <= GetStart().y);
    }

  private:
    util::Point start_;
    util::Point end_;
    util::Point xAxis_; // x 轴的坐标 [x 起点， x 终点]
    util::Point yAxis_; // y 轴的坐标 [y 起点， y 终点]
    util::Matrix matrix_;

  public:
    double startAngle_ = -M_PI / 2;
    double endAngle_ = M_PI * 3 / 2;
    double circleRadius_ = 0;
};
} // namespace coord
} // namespace canvas
} // namespace xg

#endif /* XG_GRAPHICS_COORD_POLAR_H */
