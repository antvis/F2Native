#include "graphics/util/Point.h"
#include <math.h>

#ifndef XG_GRAPHICS_CANVAS_CONTAINER_H
#define XG_GRAPHICS_CANVAS_CONTAINER_H

namespace xg {
namespace canvas {

// ElementTree
/// 暂时用不上, canvas 直接继承自 Group。 如果需要用的话单独抽出 Plot 类
class Container {
  public:
    Container() {}

    // 视图容器
    // @param start -- 左上点
    // @param end   -- 右下点
    Container(util::Point start, util::Point end) { this->Reset(start, end); }

    void Reset(util::Point start, util::Point end) {
        start_ = start;
        end_ = end;

        double xMin = fmin(start.x, end.x);
        double xMax = fmax(start.x, end.x);
        double yMin = fmin(start.y, end.y);
        double yMax = fmax(start.y, end.y);

        tl_.Reset(xMin, yMin);
        tr_.Reset(xMax, yMin);
        bl_.Reset(xMin, yMax);
        br_.Reset(xMax, yMax);
        width_ = xMax - xMin;
        height_ = yMax - yMin;
    }

    bool IsInRange(util::Point point) { return tl_.x <= point.x && tr_.x >= point.x && tl_.y <= point.y && br_.y >= point.y; }

    // 往图层上添加元素
    void AddShape() {}

    // 往图层上添加元素组
    void AddGroup() {}

    double GetWidth() { return this->width_; }
    double GetHeight() { return this->height_; }

    // 左上点坐标
    util::Point GetTL() { return this->tl_; }
    // 右上角坐标
    util::Point GetTR() { return this->tr_; }
    // 左下角坐标
    util::Point GetBL() { return this->bl_; }
    // 右下角坐标
    util::Point GetBR() { return this->br_; }

  private:
    util::Point start_;
    util::Point end_;
    util::Point tl_;
    util::Point tr_;
    util::Point bl_;
    util::Point br_;
    double width_;
    double height_;
};
} // namespace canvas
} // namespace xg

#endif // XG_GRAPHICS_CANVAS_CONTAINER_H
