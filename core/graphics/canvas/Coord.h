#ifndef XG_GRAPHICS_CANVAS_COORD_H
#define XG_GRAPHICS_CANVAS_COORD_H

#include "../util/Point.h"

/*
  |-----------------------------------end
  |                                     |
  |                                     |
  |                                     |
  |                                     |
  start----------------------------------
 */

namespace xg {
namespace canvas {
namespace coord {

enum class CoordType {
    Polar = 1,
    Cartesian,
};
// 坐标系
class AbstractCoord {
  public:
    // 度量坐标转换为实际坐标
    virtual util::Point ConvertPoint(util::Point point) const = 0;

    // 实际坐标转换为度量坐标(没什么用)
    virtual util::Point InvertPoint(util::Point point) const = 0;

    virtual ~AbstractCoord() = default;

    util::Point GetCenter() const noexcept { return center_; }

    virtual CoordType GetType() const noexcept = 0;

    void SetTransposed(bool transposed) noexcept { this->transposed_ = transposed; }

    bool IsTransposed() { return this->transposed_; }

    virtual void Reset(util::Point start, util::Point end) noexcept = 0;

    /// 返回轴的宽度
    virtual double GetWidth() const noexcept = 0;

    virtual double GetHeight() const noexcept { return abs(this->GetYAxis().x - this->GetYAxis().y); };

    /// 返回半径
    virtual double GetRadius() const noexcept { return 0.f; };

    virtual util::Point GetXAxis() const noexcept = 0;

    virtual util::Point GetYAxis() const noexcept = 0;
    
    
    
    //坐标系左下角的点
    virtual inline util::Point GetStart() noexcept = 0;
    
    //坐标系右上角的点
    virtual inline util::Point GetEnd() noexcept = 0;

    virtual bool IsContains(double x, double y) noexcept = 0;

  protected:
    util::Point center_; // 中心点
    bool transposed_;    // 是否翻转
    bool isConvertPointV2_ = false; // 是否使用优化nan处理逻辑的转换函数
};
} // namespace coord
} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_CANVAS_COORD_H
