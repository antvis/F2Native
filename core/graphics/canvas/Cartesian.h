#ifndef XG_GRAPHICS_COORD_CARTESIAN_H
#define XG_GRAPHICS_COORD_CARTESIAN_H

#include "Coord.h"
#include "../util/Matrix.h"
#include "../util/Point.h"
#include "../util/Vector2d.h"

namespace xg {
namespace canvas {
namespace coord {

// 笛卡尔积坐标系
class Cartesian : public AbstractCoord {
  public:
    // @param start -- 左下角坐标
    // @param end -- 右上角坐标
    // @param transposed -- 坐标系是否翻转
    Cartesian(util::Point start, util::Point end, bool transposed = false, nlohmann::json config = {}) : AbstractCoord() {
        xAxis_.Reset(start.x, end.x);
        yAxis_.Reset(start.y, end.y);
        center_.Reset(xAxis_.x + (xAxis_.y - xAxis_.x) / 2, yAxis_.x + (yAxis_.y - yAxis_.x) / 2);
        util::MatrixUtil::Reset(&this->matrix_);
        transposed_ = transposed;
        isConvertPointV2_ = json::GetBool(config, "_isConvertPointV2", false);
    }

    // @param point -- 度量坐标值[0~1]
    // @return  实际坐标值
    util::Point ConvertPoint(util::Point point) const override {
        if (this->isConvertPointV2_) {
            bool isXNan = false, isYNan = false;
            if (std::isnan(point.x)) {
                point.x = 0;
                isXNan = true;
            }
            if (std::isnan(point.y)) {
                point.y = 0;
                isYNan = true;
            }
            double x = xAxis_.x + (xAxis_.y - xAxis_.x) * (transposed_ ? point.y : point.x);
            double y = yAxis_.x + (yAxis_.y - yAxis_.x) * (transposed_ ? point.x : point.y);
            
            util::Vector2D v;
            util::Vector2DUtil::Reset(&v, x, y);
            util::Vector2DUtil::TransformMat2D(&v, v, this->matrix_);
            
            return util::Point{isXNan ? NAN : v[0], isYNan ? NAN : v[1]};
        } else {
            double x = xAxis_.x + (xAxis_.y - xAxis_.x) * (transposed_ ? point.y : point.x);
            double y = yAxis_.x + (yAxis_.y - yAxis_.x) * (transposed_ ? point.x : point.y);
            
            util::Vector2D v;
            util::Vector2DUtil::Reset(&v, x, y);
            util::Vector2DUtil::TransformMat2D(&v, v, this->matrix_);
            
            return util::Point{v[0], v[1]};
        }
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

    double GetWidth() const noexcept override { return xAxis_.y - xAxis_.x; }

    double GetRadius() const noexcept override { return 0; }

    util::Point GetXAxis() const noexcept override { return xAxis_; }

    util::Point GetYAxis() const noexcept override { return yAxis_; }
    
    //坐标系左下角的点
    inline util::Point GetStart() noexcept override { return util::Point(xAxis_.x, yAxis_.x);}
    
    //坐标系右上角的点
    inline util::Point GetEnd() noexcept override { return util::Point(xAxis_.y, yAxis_.y);}

    bool IsContains(double x, double y) noexcept override {
        return (x >= GetStart().x && x <= GetEnd().x && y >= GetEnd().y && y <= GetStart().y);
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
