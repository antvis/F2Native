#ifndef XG_GRAPHICS_UTIL_BBOX_H
#define XG_GRAPHICS_UTIL_BBOX_H

#include <math.h>
#include <vector>
#include "Point.h"

using namespace std;

namespace xg {
namespace util {

typedef struct BBox_ {
    //顺序固定
    float minX, maxX, minY, maxY, width, height, x, y;
} BBox;

class BBoxUtil {
  public:
    static const bool IsPointHitBBox(const util::Point &point, const util::BBox &bbox, const std::string &dimType) {
        // 柱图坐标轴点击，会使用这个函数，但是由于文本的bbox区域与点击区域无法对齐，因此放开文本垂直于坐标轴的空间（点击tick文本上方的柱子也会触发这个函数返回true）
        if (dimType == "x") {
            return point.x > bbox.minX && point.x < bbox.maxX;
        } else {
            return point.y > bbox.minY && point.y < bbox.maxY;
        }
    }
    
    static BBox GetBBoxFromLine(float x0, float y0, float x1, float y1, float lineWidth) {
        const float hafWidth = lineWidth / 2;
        return {fmin(x0, x1) - hafWidth, fmax(x0, x1) + hafWidth, fmin(y0, y1) - hafWidth, fmax(y0, y1) + hafWidth, .0f, .0f};
    }

    static BBox GetBBoxFromPoints(const std::vector<Point> &points, float lineWidth) {
        if(points.empty()) {
            return {0};
        }
        auto &p = points[0];
        auto left = p.x;
        auto right = p.x;
        auto top = p.y;
        auto bottom = p.y;

        for(std::size_t i = 1; i < points.size(); ++i) {
            auto &p1 = points[i];
            left = min(left, p1.x);
            right = max(right, p1.x);
            top = min(top, p1.y);
            bottom = max(bottom, p1.y);
        }

        const float hafWidth = lineWidth / 2;
        return {static_cast<float>(left - hafWidth),
                static_cast<float>(right + hafWidth),
                static_cast<float>(top - hafWidth),
                static_cast<float>(bottom + hafWidth),
                .0f,
                .0f};
    }
};

} // namespace util
} // namespace xg

#endif /* XG_GRAPHICS_UTIL_BBOX_H */
