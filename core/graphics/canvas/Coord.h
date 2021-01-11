#ifndef XG_GRAPHICS_CANVAS_COORD_H
#define XG_GRAPHICS_CANVAS_COORD_H

#include "graphics/util/Point.h"

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
    virtual float GetWidth() const noexcept = 0;

    virtual float GetHeight() noexcept { return this->GetYAxis().x - this->GetYAxis().y; }

    /// 返回半径
    virtual double GetRadius() const noexcept { return 0.f; };

    virtual util::Point GetXAxis() noexcept = 0;

    virtual util::Point GetYAxis() noexcept = 0;

    virtual bool IsContains(double x, double y) noexcept = 0;

  protected:
    util::Point center_; // 中心点
    bool transposed_;    // 是否翻转
};
} // namespace coord
} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_CANVAS_COORD_H
