#include "graphics/canvas/Coord.h"
#include "graphics/util/Matrix.h"
#include "graphics/util/Point.h"
#include "graphics/util/Vector2d.h"

#ifndef XG_GRAPHICS_COORD_CARTESIAN_H
#define XG_GRAPHICS_COORD_CARTESIAN_H

namespace xg {
namespace canvas {
namespace coord {

// 笛卡尔积坐标系
class Cartesian : public AbstractCoord {
  public:
    // @param start -- 左下角坐标
    // @param end -- 右上角坐标
    // @param transposed -- 坐标系是否翻转
    Cartesian(util::Point start, util::Point end, bool transposed = false) : AbstractCoord() {
        xAxis_.Reset(start.x, end.x);
        yAxis_.Reset(start.y, end.y);
        center_.Reset(xAxis_.x + (xAxis_.y - xAxis_.x) / 2, yAxis_.x + (yAxis_.y - yAxis_.x) / 2);
        util::MatrixUtil::Reset(&this->matrix_);
        transposed_ = transposed;
    }

    // @param point -- 度量坐标值[0~1]
    // @return  实际坐标值
    util::Point ConvertPoint(util::Point point) const override {
        double x = xAxis_.x + (xAxis_.y - xAxis_.x) * (transposed_ ? point.y : point.x);
        double y = yAxis_.x + (yAxis_.y - yAxis_.x) * (transposed_ ? point.x : point.y);

        util::Vector2D v;
        util::Vector2DUtil::Reset(&v, x, y);
        util::Vector2DUtil::TransformMat2D(&v, v, this->matrix_);

        return util::Point{v[0], v[1]};
    }

    // @param point -- 实际坐标值
    // @return 度量坐标值
    util::Point InvertPoint(util::Point point) const override {
        double x = (point.x - xAxis_.x) / (xAxis_.y - xAxis_.x);
        double y = (point.y - yAxis_.x) / (yAxis_.y - yAxis_.x);
        if(this->transposed_) {
            return std::move(util::Point(y, x));
        } else {
            return std::move(util::Point(x, y));
        }
    }

    virtual void Reset(util::Point start, util::Point end) noexcept override {
        xAxis_.Reset(start.x, end.x);
        yAxis_.Reset(start.y, end.y);
        center_.Reset(xAxis_.x + (xAxis_.y - xAxis_.x) / 2, yAxis_.x + (yAxis_.y - yAxis_.x) / 2);
        util::MatrixUtil::Reset(&this->matrix_);
    }

    virtual CoordType GetType() const noexcept override { return CoordType::Cartesian; }

    float GetWidth() const noexcept override { return xAxis_.y - xAxis_.x; }

    double GetRadius() const noexcept override { return 0; }

    util::Point GetXAxis() noexcept override { return xAxis_; }

    util::Point GetYAxis() noexcept override { return yAxis_; }

    bool IsContains(double x, double y) noexcept override {
        return (x >= xAxis_.x && x <= xAxis_.y && y <= yAxis_.x && y >= yAxis_.y);
    }

  private:
    util::Point xAxis_; // x 轴的坐标 [x 起点， x 终点]
    util::Point yAxis_; // y 轴的坐标 [y 起点， y 终点]
    util::Matrix matrix_;
};
} // namespace coord
} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_COORD_CARTESIAN_H
